#include "prisonerslistmodel.h"

#include "politsake.h"

#include <QColor>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequestFactory>

namespace {

QString appendToPrisonersList(QStringList &prisonersList, const QString &recordsOffset,
                              QSet<QString> &uniquePrisoners, QMap<QString, QString> &lettersAddresses)
{
    QString fullURL = PolitSake::prisonersListURL;
    if (!recordsOffset.isEmpty()) {
        fullURL += "?offset=" + recordsOffset;
    }

    QNetworkRequestFactory exampleApi{ {fullURL} };

    // Fill common headers
    QHttpHeaders commonHeaders;
    exampleApi.setCommonHeaders(commonHeaders);

    QByteArray bearerToken = QString{PolitSake::airTableToken}.toLatin1();
    exampleApi.setBearerToken(bearerToken);

    QNetworkAccessManager networkAccessManager;
    QNetworkReply *reply = networkAccessManager.get(exampleApi.createRequest());

    QEventLoop loop;
    loop.connect(&networkAccessManager, SIGNAL(finished(QNetworkReply *)), SLOT(quit()));
    loop.exec();

    if (reply->error()) {
        return {};
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    QJsonObject prisonersObject = document.object();

    auto prisonersRecords = prisonersObject["records"];

    if (!prisonersRecords.isArray()) {
        return {};
    }

    auto prisonersArray = prisonersRecords.toArray();

    for (auto prisoner = prisonersArray.cbegin(); prisoner != prisonersArray.cend(); ++prisoner) {
        auto prisonerFields = (*prisoner)[QString("fields")];

        auto letterAddress =
            prisonerFields[QString("Текущее место отбывания наказания")];
        if (!letterAddress.isString()) {
            continue;
        }

        QString letterAddressString = letterAddress.toString();
        if (letterAddressString.isEmpty()) {
            continue;
        }

        if (letterAddressString.startsWith("\"")) {
            letterAddressString.removeFirst();
        }

        if (letterAddressString.startsWith(",")) {
            letterAddressString.removeFirst();
        }

        int verticalBarIndex = letterAddressString.indexOf("| ФКУ");
        if (-1 != verticalBarIndex) {
            letterAddressString = letterAddressString.remove(0, verticalBarIndex + 1);
        }

        letterAddressString = letterAddressString.trimmed();

        if (letterAddressString.endsWith("\"")) {
            letterAddressString.removeLast();
        }

        if (letterAddressString.endsWith(",")) {
            letterAddressString.removeLast();
        }

        verticalBarIndex = letterAddressString.lastIndexOf(" |");
        if (-1 != verticalBarIndex) {
            letterAddressString = letterAddressString.remove(verticalBarIndex,
                                                             letterAddressString.length() - verticalBarIndex);
        }

        letterAddressString = letterAddressString.trimmed();

        auto initials = prisonerFields[QString("ФИО")];
        if (initials.isString()) {
            QString initialsString = initials.toString();

            if (uniquePrisoners.contains(initialsString)) {
                continue;
            }

            uniquePrisoners.insert(initialsString);
            prisonersList.append(initialsString);

            lettersAddresses[initialsString] = letterAddressString;
        }
    }

    auto offsetObject = prisonersObject["offset"];
    if (!offsetObject.isString()) {
        return {};
    }

    return offsetObject.toString();
}

}

PrisonersListModel::PrisonersListModel(QObject *parent,
                                       QMap<QString, QString> &letterAddresses) : QStringListModel{parent}
{
    QStringList prisonersList;

    QSet<QString> uniquePrisoners;

    QString recordsOffset;

    do {
        recordsOffset = appendToPrisonersList(prisonersList, recordsOffset, uniquePrisoners,
                                              letterAddresses);
    } while (!recordsOffset.isEmpty());

    prisonersList.sort();

    setStringList(prisonersList);
}

PrisonersListModel::PrisonersListModel(const QStringList &strings,
                                       QObject *parent) : QStringListModel{strings, parent}
{

}

int PrisonersListModel::getSize() const
{
    return static_cast<int>(checkedItems.size());
}

Qt::ItemFlags PrisonersListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStringListModel::flags(index);
    if (index.isValid()) {
        return defaultFlags | Qt::ItemIsUserCheckable;
    }
    return defaultFlags;
}

QVariant PrisonersListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant{};
    }

    if (role == Qt::CheckStateRole) {
        return checkedItems.contains(index) ? Qt::Checked : Qt::Unchecked;
    } else if (role == Qt::BackgroundRole) {
        return checkedItems.contains(index) ? QColor{colorChecked} :
               QColor{colorUnchecked};
    }

    return QStringListModel::data(index, role);
}

bool PrisonersListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::CheckStateRole) {
        return false;
    }

    if (value == Qt::Checked) {
        checkedItems.insert(index);
    } else {
        checkedItems.remove(index);
    }

    emit dataChanged(index, index);
    return true;
}

void PrisonersListModel::save()
{
}
