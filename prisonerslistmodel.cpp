#include "prisonerslistmodel.h"
#include <QColor>

PrisonersListModel::PrisonersListModel(QObject *parent) : QStringListModel{parent}
{

}

PrisonersListModel::PrisonersListModel(const QStringList &strings,
                                       QObject *parent) : QStringListModel{strings, parent}
{

}

int PrisonersListModel::getSize() const
{
    return static_cast<int>(checkedItems.size());
}

Qt::ItemFlags PrisonersListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStringListModel::flags(index);
    if (index.isValid()) {
        return defaultFlags | Qt::ItemIsUserCheckable;
    }
    return defaultFlags;
}

QVariant PrisonersListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant{};
    }

    if (role == Qt::CheckStateRole) {
        return checkedItems.contains(index) ? Qt::Checked : Qt::Unchecked;
    } else if (role == Qt::BackgroundRole) {
        return checkedItems.contains(index) ? QColor{colorChecked} :
               QColor{colorUnchecked};
    }

    return QStringListModel::data(index, role);
}

bool PrisonersListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::CheckStateRole) {
        return false;
    }

    if (value == Qt::Checked) {
        checkedItems.insert(index);
    } else {
        checkedItems.remove(index);
    }

    emit dataChanged(index, index);
    return true;
}

void PrisonersListModel::save()
{
}
