#include "treeviewmodel.h"

TreeViewModel::TreeViewModel()
{

}

void TreeViewModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    QAbstractProxyModel::setSourceModel(sourceModel);

    doResetModel(sourceModel);

    if (sourceModel != nullptr) {
        connect(sourceModel, &QAbstractItemModel::dataChanged, this, &TreeViewModel::onSourceDataChanged);
        connect(sourceModel, &QAbstractItemModel::rowsInserted, this, &TreeViewModel::onRowsInserted);
        connect(sourceModel, &QAbstractItemModel::rowsRemoved, this, &TreeViewModel::onRowsRemoved);
        connect(sourceModel, &QAbstractItemModel::rowsMoved, this, &TreeViewModel::onRowsMoved);
        connect(sourceModel, &QAbstractItemModel::layoutChanged, this, &TreeViewModel::onLayoutChanged);
    }
}

QModelIndex TreeViewModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid() || !flattenedTree_.count() > proxyIndex.row())
        return QModelIndex();
    return flattenedTree_[proxyIndex.row()]->sourceIndex();
}

QModelIndex TreeViewModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    TreeItemViewModel* n = findItemByIndex(sourceIndex);
    if (n == nullptr)
        return QModelIndex();
    return createIndex(n->row(), 0);
}

int TreeViewModel::columnCount(const QModelIndex &parent) const
{
    return sourceModel()->columnCount(mapToSource(parent));
}

int TreeViewModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return flattenedTree_.count();
    return 0;
}

QModelIndex TreeViewModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();
    else
        return createIndex(row, column);
}

QModelIndex TreeViewModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QVariant TreeViewModel::data(const QModelIndex &proxyIndex, int role) const
{
    switch (role) {
        case Indentation:
            return flattenedTree_[proxyIndex.row()]->indent();
        case HasChildren:
            return flattenedTree_[proxyIndex.row()]->hasChildren();
        case IsExpanded:
            return flattenedTree_[proxyIndex.row()]->isExpanded();
        case Hidden:
            return flattenedTree_[proxyIndex.row()]->isHidden();
        default:
            return QAbstractProxyModel::data(proxyIndex, role);
    }
}

bool TreeViewModel::setData(const QModelIndex &proxyIndex, const QVariant &value, int role)
{
    switch (role) {
        case IsExpanded:
            toggleIsExpanded(proxyIndex.row(), value.toBool());
            break;
        default:
            QAbstractProxyModel::setData(proxyIndex, value, role);
    }
}

QHash<int, QByteArray> TreeViewModel::roleNames() const
{
    auto names = QAbstractItemModel::roleNames();
    names[Indentation] = "indentation";
    names[HasChildren] = "hasChildren";
    names[IsExpanded] = "isExpanded";
    names[Hidden] = "hidden";
    return names;
}
