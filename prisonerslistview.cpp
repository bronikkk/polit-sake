#include "prisonerslistview.h"

PrisonersListView::PrisonersListView(QWidget *parent,
                                     QMap<QString, QString> &lettersAddresses) : QListView{parent}
{
    prisonersListModel = new PrisonersListModel{this, lettersAddresses};
    setModel(prisonersListModel);
}

int PrisonersListView::getModelSize() const
{
    return prisonersListModel->getSize();
}
