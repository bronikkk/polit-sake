#include "facilitieslistview.h"

FacilitiesListView::FacilitiesListView(QWidget *parent,
                                       const PenitentiaryDatabase &penitentiaryDatabase) : QListView{parent}
{
    QStringList facilities = penitentiaryDatabase.getPenitentiaries();

}
