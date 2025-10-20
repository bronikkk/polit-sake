#ifndef PRISONERSLISTVIEW_H
#define PRISONERSLISTVIEW_H

#include <QListView>
#include "prisonerslistmodel.h"

class PrisonersListView : public QListView
{
public:
    PrisonersListView(QWidget *parent, QMap<QString, QString> &lettersAddresses);

    int getModelSize() const;

private:
    PrisonersListModel *prisonersListModel;
};

#endif // PRISONERSLISTVIEW_H
