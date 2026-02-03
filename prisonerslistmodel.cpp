#include "prisonerslistmodel.h"

#include "politsake.h"
#include "prisonerslistview.h"

#include <QColor>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequestFactory>

namespace {

static constexpr auto arrestLocationField =
    "Текущее место меры пресечения";

static constexpr auto initialsField = "ФИО";

static constexpr auto prisonLocationField =
    "Текущее место отбывания наказания";

bool isAnonymous(QString name)
{
    return name == "Пожелал остаться анонимным"
           || name.startsWith("Неизвестное лицо");
}

QString appendToPrisonersList(QStringList &prisonersList, const QString &recordsOffset,
                              QSet<QString> &uniquePrisoners, QMap<QString, QString> &prisonersToFacilities)
{
    static QRegularExpression badQuotationMarks{"»«.*"};

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
    loop.connect(&networkAccessManager, SIGNAL(finished(QNetworkReply*)), SLOT(quit()));
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

    const auto prisonersArray = prisonersRecords.toArray();

    for (const auto &prisoner : prisonersArray) {
        auto prisonerFields = prisoner[QString("fields")];

        auto letterAddress = prisonerFields[prisonLocationField];

        QString letterAddressString;
        if (!letterAddress.isString() || (letterAddressString = letterAddress.toString()).isEmpty()) {
            letterAddress = prisonerFields[arrestLocationField];
            if (!letterAddress.isString()) {
                continue;
            }

            letterAddressString = letterAddress.toString();
            if (letterAddressString.isEmpty()) {
                continue;
            }
        }

        auto initials = prisonerFields[initialsField];

        if (!initials.isString()) {
            continue;
        }

        QString initialsString = initials.toString();

        if (isAnonymous(initialsString)) {
            continue;
        }

        if (uniquePrisoners.contains(initialsString)) {
            continue;
        }

        letterAddressString = letterAddressString.split('\n').first();

        if (letterAddressString.startsWith(",")) {
            letterAddressString.removeFirst();
        }

        letterAddressString = letterAddressString.trimmed();

        if (letterAddressString.startsWith("\"")) {
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

        letterAddressString.replace("(\"\"", "(«");
        letterAddressString.replace("\"\")", "»)");

        letterAddressString.replace("\u00A0", " ");

        letterAddressString.replace(badQuotationMarks, "»");

        uniquePrisoners.insert(initialsString);
        prisonersList.append(initialsString);

        prisonersToFacilities[initialsString] = letterAddressString;
    }

    auto offsetObject = prisonersObject["offset"];
    if (!offsetObject.isString()) {
        return {};
    }

    return offsetObject.toString();
}

}

PrisonersListModel::PrisonersListModel(QWidget *parent,
                                       PrisonersListView *listView,
                                       QMap<QString, QString> &prisonersToFacilities) : QStringListModel{parent}
{
    QStringList prisonersList;

    QSet<QString> uniquePrisoners;

    QString recordsOffset;

    do {
        recordsOffset = appendToPrisonersList(prisonersList, recordsOffset, uniquePrisoners,
                                              prisonersToFacilities);

        prisonersList.sort();

        setStringList(prisonersList);

        // TODO: Fix MVC antipattern
        listView->setModel(this);

    } while (!recordsOffset.isEmpty());
}

PrisonersListModel::PrisonersListModel(QWidget *parent,
                                       const QStringList &strings) : QStringListModel{strings, parent}
{}

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
