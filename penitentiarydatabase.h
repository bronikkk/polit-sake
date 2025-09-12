#ifndef PENITENTIARYDATABASE_H
#define PENITENTIARYDATABASE_H

#include <QListView>
#include <QMap>
#include <QString>

class PenitentiaryDatabase : public QListView
{
public:
    PenitentiaryDatabase(QWidget *parent, QString filename = ":/data/penitentiary.xml");

    struct Address {
        QString location;
        QString state;
        QString zip;
    };

    Address getAddressForPenitentiary(QString name) const;

private:
    QMap<QString, Address> addresses;
};

#endif // PENITENTIARYDATABASE_H
