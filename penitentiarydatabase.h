#ifndef PENITENTIARYDATABASE_H
#define PENITENTIARYDATABASE_H

#include <QListView>
#include <QMap>
#include <QString>

class PenitentiaryDatabase : public QListView
{
public:
    PenitentiaryDatabase(QWidget *parent, QString filename = ":/data/penitentiary.xml");

private:
    QMap<QString, QString> locations;
};

#endif // PENITENTIARYDATABASE_H
