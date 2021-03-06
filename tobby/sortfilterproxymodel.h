#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel
{
public:
    SortFilterProxyModel();
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // SORTFILTERPROXYMODEL_H
