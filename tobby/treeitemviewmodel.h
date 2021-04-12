#ifndef TREEITEMVIEWMODEL_H
#define TREEITEMVIEWMODEL_H

#include <QtCore/QAbstractProxyModel>
#include <QtCore/QModelIndex>


class TreeItemViewModel
{
public:
    /**
     * Creates a root item.
     *
     * @param sourceIndex sourceIndex of the model
     * @param flattenedTree Flat representation of the tree
     * @param proxyModel The proxy model that contains the items.
     */
    TreeItemViewModel(QModelIndex sourceIndex, QList<TreeItemViewModel*>& flattenedTree,
                      QMap<QModelIndex, bool>& expandedMap, QMap<QModelIndex, bool>& hiddenMap);

    TreeItemViewModel(TreeItemViewModel* parent, QModelIndex sourceIndex, QList<TreeItemViewModel*>& flattenedTree, QAbstractProxyModel* model,
                      QMap<QModelIndex, bool>& expandedMap, QMap<QModelIndex, bool>& hiddenMap);

    QPersistentModelIndex sourceIndexAcrossProxyChain(const QModelIndex& proxyIndex) const;
    TreeItemViewModel* parent() const;
    int indexOfChild(const TreeItemViewModel* child) const;
    QModelIndex sourceIndex() const;
    TreeItemViewModel* addChild(QModelIndex index);
    TreeItemViewModel* insertChild(int row, QModelIndex index);
    void setExpanded(bool expanded);
    void setHidden(bool hidden);
    int row();
    int indent() const;
    bool isExpanded() const;
    bool isCollapsed();
    bool isHidden() const;
    bool hasChildren() const;
    int getLastChildRow();

private:
    QPersistentModelIndex sourceIndex_;
    int indent_;
    bool isExpanded_;
    bool isHidden_;

    TreeItemViewModel* parent_;
    QAbstractProxyModel* proxyModel_;
    QList<TreeItemViewModel*>& flattenedTree_;
    QList<TreeItemViewModel*> childItems_;
    QMap<QModelIndex, bool>& expandedMap_;
    QMap<QModelIndex, bool>& hiddenMap_;
};

#endif // TREEITEMVIEWMODEL_H
