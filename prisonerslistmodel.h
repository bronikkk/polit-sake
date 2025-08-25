#ifndef PRISONERSLISTMODEL_H
#define PRISONERSLISTMODEL_H
#include <QStringListModel>

class PrisonersListModel : public QStringListModel
{
private:
    static constexpr auto colorChecked = "#ffffb2";
    static constexpr auto colorUnchecked = "#ffffff";

    QSet<QPersistentModelIndex> checkedItems;

public:
    PrisonersListModel(QObject *parent = nullptr);
    PrisonersListModel(const QStringList &strings, QObject *parent = nullptr);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void save();
};

#endif // PRISONERSLISTMODEL_H
