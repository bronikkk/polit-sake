#include "penitentiarydatabase.h"

#include <QFile>
#include <QMessageBox>
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

    databaseFile->close();
}
