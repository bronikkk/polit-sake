#ifndef PRISONERSLISTMODEL_H
#define PRISONERSLISTMODEL_H

#include <QStringListModel>

class PrisonersListModel : public QStringListModel
{
private:
    static constexpr auto colorChecked = "#ffffb2"; // Yellow
    static constexpr auto colorUnchecked = "#ffffff"; // White

    QSet<QPersistentModelIndex> checkedItems;

public:
    PrisonersListModel(QObject *parent, QMap<QString, QString> &prisonersToFacilities);
    PrisonersListModel(QObject *parent, const QStringList &strings);

    int getSize() const;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
};

#endif // PRISONERSLISTMODEL_H
