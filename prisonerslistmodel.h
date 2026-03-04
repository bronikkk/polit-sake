#ifndef PRISONERSLISTMODEL_H
#define PRISONERSLISTMODEL_H

#include <QListView>
#include <QStringListModel>

class PrisonersListModel : public QStringListModel
{
public:
    PrisonersListModel(QWidget *parent, QListView *listView,
                       QMap<QString, QString> &prisonersToFacilities);

    int getCheckedItemsNumber() const;

    int getTotalItemsNumber() const;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    static constexpr auto colorChecked = "#ffffb2"; // Yellow
    static constexpr auto colorUnchecked = "#ffffff"; // White

    QSet<QPersistentModelIndex> checkedItems;

    int totalItemsNumber = 0;
};

#endif // PRISONERSLISTMODEL_H
