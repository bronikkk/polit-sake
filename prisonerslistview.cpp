#include "prisonerslistview.h"
#include "prisonerslistmodel.h"

PrisonersListView::PrisonersListView(QWidget *parent) : QListView{parent}
{
    PrisonersListModel *model = new PrisonersListModel(this);

    QStringList prisonersList;

    prisonersList.append("Ivanov Ivan Ivanovich");
    prisonersList.append("Martynova Martyna Martynovna");
    prisonersList.append("Sidorov Sidor Sidorovich");

    model->setStringList(prisonersList);

    setModel(model);
}
