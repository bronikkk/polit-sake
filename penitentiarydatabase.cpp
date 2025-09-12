#include "penitentiarydatabase.h"

#include <QFile>
#include <QMessageBox>
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

    QVector<QString> names;
    QString location;
    QString state;
    QString zip;

    while (!(xmlReader->atEnd())) {
        if (xmlReader->readNext() == QXmlStreamReader::StartElement) {
            if (xmlReader->name() == "amenity") {
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
        } else if (xmlReader->name() == "amenity") {
            for (auto &name : names) {
                addresses[name] = Address{location, state, zip};
            }
        }
    }

    if (xmlReader->hasError()) {
        QMessageBox::critical(this, "Error", "Penitentiary database is corrupted");
    }

    databaseFile->close();
}

PenitentiaryDatabase::Address PenitentiaryDatabase::getAddressForPenitentiary(QString name) const
{
    auto iter = addresses.find(name);
    return iter != addresses.end() ? *iter : PenitentiaryDatabase::Address{};
}
