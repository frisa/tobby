#include <QtCore/QList>
#include <QDebug>

#include "treeitemviewmodel.h"

TreeItemViewModel::TreeItemViewModel(QModelIndex sourceIndex,
                                     QList<TreeItemViewModel *> &flattenedTree,
                                     QMap<QModelIndex, bool> &expandedMap,
                                     QMap<QModelIndex, bool> &hiddenMap):
                    TreeItemViewModel(nullptr, sourceIndex, flattenedTree, proxyModel_, expandedMap, hiddenMap)
{

}

TreeItemViewModel::TreeItemViewModel(TreeItemViewModel *parent,
                                     QModelIndex sourceIndex,
                                     QList<TreeItemViewModel *> &flattenedTree,
                                     QAbstractProxyModel *model, QMap<QModelIndex, bool> &expandedMap,
                                     QMap<QModelIndex, bool> &hiddenMap):
                                        parent_(parent),
                                        proxyModel_(model),
                                        sourceIndex_(sourceIndex),
                                        flattenedTree_(flattenedTree),
                                        isExpanded_(false),
                                        expandedMap_(expandedMap),
                                        hiddenMap_(hiddenMap)
{
    if (parent) {
        isHidden_ = parent->isCollapsed() || parent->isHidden();;
        indent_ = parent->indent() + 1;
    }
    else {
        isHidden_ = false;
        indent_ = 0;
        flattenedTree.append(this);
    }

    isExpanded_ = expandedMap.value(sourceIndexAcrossProxyChain(sourceIndex), isExpanded_);
}

QPersistentModelIndex TreeItemViewModel::sourceIndexAcrossProxyChain(const QModelIndex &proxyIndex) const
{
    QAbstractProxyModel* proxyModel = proxyModel_;
    QAbstractProxyModel* nextSubProxyModel = qobject_cast<QAbstractProxyModel*>(proxyModel->sourceModel());
    QModelIndex sourceIndex = proxyIndex;
    while (nextSubProxyModel != nullptr) {
        proxyModel = nextSubProxyModel;
        sourceIndex = proxyModel->mapToSource(sourceIndex);
        nextSubProxyModel = qobject_cast<QAbstractProxyModel*>(proxyModel->sourceModel());
    }
    return QPersistentModelIndex(sourceIndex);
}

TreeItemViewModel *TreeItemViewModel::parent() const
{
    return parent_;
}

int TreeItemViewModel::indexOfChild(const TreeItemViewModel *child) const
{
    if (childItems_.contains(const_cast<TreeItemViewModel*>(child)))
    {
        return childItems_.indexOf(const_cast<TreeItemViewModel*>(child));
    }
    else
    {
        return -1;
    }
}

QModelIndex TreeItemViewModel::sourceIndex() const
{
    return sourceIndex_;
}

TreeItemViewModel *TreeItemViewModel::addChild(QModelIndex index)
{
    TreeItemViewModel* child = new TreeItemViewModel(
            this, index, flattenedTree_, proxyModel_, expandedMap_, hiddenMap_);

    int insertPoint = getLastChildRow() + 1;
    flattenedTree_.insert(insertPoint, child);
    childItems_.append(child);

    return child;
}

TreeItemViewModel *TreeItemViewModel::insertChild(int row, QModelIndex index)
{
    if (childItems_.count() > row) {
        TreeItemViewModel* child = new TreeItemViewModel(
                this, index, flattenedTree_, proxyModel_, expandedMap_, hiddenMap_);
        int insertPoint = childItems_[row]->getLastChildRow();
        flattenedTree_.insert(insertPoint, child);
        childItems_.insert(row, child);
        return child;
    }
    else
    {
        addChild(index);
        return nullptr;
    }
}

void TreeItemViewModel::setExpanded(bool expanded)
{
    isExpanded_ = expanded;
    expandedMap_[sourceIndexAcrossProxyChain(sourceIndex_)] = expanded;

    QModelIndex proxyIndex = proxyModel_->mapFromSource(sourceIndex());
    emit proxyModel_->dataChanged(proxyIndex, proxyIndex);

    if (proxyModel_->sourceModel()->canFetchMore(sourceIndex()))
        proxyModel_->sourceModel()->fetchMore(sourceIndex());

    for(TreeItemViewModel* child: childItems_)
        child->setHidden(!isExpanded_);
}

void TreeItemViewModel::setHidden(bool hidden)
{
    isHidden_ = hidden;
    hiddenMap_[sourceIndex_] = hidden;

    QModelIndex proxyIndex = proxyModel_->mapFromSource(sourceIndex());
    emit proxyModel_->dataChanged(proxyIndex, proxyIndex);

    for(TreeItemViewModel* child: childItems_)
        child->setHidden(isHidden_ || !isExpanded_);
}

int TreeItemViewModel::row()
{
    return flattenedTree_.indexOf(this);
}

int TreeItemViewModel::indent() const
{
    return indent_;
}

bool TreeItemViewModel::isExpanded() const
{
    return isExpanded_;
}

bool TreeItemViewModel::isCollapsed()
{
    return !isExpanded_;
}

bool TreeItemViewModel::isHidden() const
{
    return isHidden_;
}

bool TreeItemViewModel::hasChildren() const
{
    return proxyModel_->sourceModel()->hasChildren(sourceIndex());
}

int TreeItemViewModel::getLastChildRow()
{
    if (childItems_.count() > 0)
    {
        TreeItemViewModel *lastChild = childItems_.last();
        return lastChild->getLastChildRow();
    }
    else
    {
        return row();
    }
}
