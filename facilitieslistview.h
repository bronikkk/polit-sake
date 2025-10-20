#ifndef FACILITIESLISTVIEW_H
#define FACILITIESLISTVIEW_H

#include <QListView>

#include <penitentiarydatabase.h>

class FacilitiesListView : public QListView
{
public:
    FacilitiesListView(QWidget *parent, const PenitentiaryDatabase &penitentiaryDatabase);
};

#endif // FACILITIESLISTVIEW_H
