#ifndef PRISONERSLISTVIEW_H
#define PRISONERSLISTVIEW_H

#include <QListView>
#include "prisonerslistmodel.h"

class PrisonersListView : public QListView
{
public:
    PrisonersListView(QWidget *parent = nullptr);

    int getSize() const;

private:
    PrisonersListModel *prisonersListModel;
};

#endif // PRISONERSLISTVIEW_H
