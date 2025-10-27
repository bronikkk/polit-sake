#include "penitentiarydatabase.h"

#include <QFile>
#include <QMessageBox>
#include <QStringListModel>
#include <QVector>
#include <QXmlStreamReader>

PenitentiaryDatabase::PenitentiaryDatabase(QWidget *parent, QString filename) : QListView{parent}
{
    QFile *databaseFile = new QFile{filename};

    if (!databaseFile->open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Penitentiary database is not available");
        return;
    }

    QXmlStreamReader *xmlReader = new QXmlStreamReader{databaseFile};
    xmlReader->readNext();

    QStringList facilities;

    QVector<QString> names;
    QString location;
    QString state;
    QString zip;

    while (!(xmlReader->atEnd())) {
        if (xmlReader->readNext() == QXmlStreamReader::StartElement) {
            if (xmlReader->name() == "facility") {
                names.clear();
                location.clear();
                state.clear();
                zip.clear();
            } else if (xmlReader->name() == "name") {
                names.emplaceBack(xmlReader->readElementText());
            } else if (xmlReader->name() == "location") {
                location = xmlReader->readElementText();
            } else if (xmlReader->name() == "state") {
                state = xmlReader->readElementText();
            } else if (xmlReader->name() == "zip") {
                zip = xmlReader->readElementText();
            }
        } else if (xmlReader->name() == "facility") {
            bool nameAlreadyAdded = false;

            for (const auto &name : names) {
                addresses[name] = Address{location, state, zip};

                if (!nameAlreadyAdded) {
                    facilities.emplaceBack(name);

                    nameAlreadyAdded = true;
                }
            }
        }
    }

    if (xmlReader->hasError()) {
        QMessageBox::critical(this, "Error", "Penitentiary database is corrupted: " + xmlReader->errorString());
    }

    databaseFile->close();

    QStringListModel *listModel = new QStringListModel{this};

    facilities.sort();
    listModel->setStringList(facilities);

    setModel(listModel);
}

PenitentiaryDatabase::Address PenitentiaryDatabase::getAddressForPenitentiary(QString name) const
{
    auto iter = addresses.find(name);
    return iter != addresses.end() ? *iter : PenitentiaryDatabase::Address{};
}
