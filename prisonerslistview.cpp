#include "prisonerslistview.h"

PrisonersListView::PrisonersListView(QWidget *parent) : QListView{parent}
{
    QStringList prisonersList;

    prisonersList.append("Ivanov Ivan Ivanovich");
    prisonersList.append("Martynova Martyna Martynovna");
    prisonersList.append("Sidorov Sidor Sidorovich");

    prisonersListModel = new PrisonersListModel(this);
    prisonersListModel->setStringList(prisonersList);

    setModel(prisonersListModel);
}

int PrisonersListView::getSize() const
{
    return prisonersListModel->getSize();
}
