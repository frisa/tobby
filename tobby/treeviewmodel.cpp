#include "treeviewmodel.h"

TreeViewModel::TreeViewModel(QObject *parent)
{
    //NOP
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

void TreeViewModel::onLayoutChanged()
{
    qDebug() << "onLayoutChanged";
    doResetModel(sourceModel());
}

void TreeViewModel::onSourceDataChanged(QModelIndex topLeft, QModelIndex bottomRight)
{
    qDebug() << "onSourceDataChanged";
    emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight));
}

void TreeViewModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    TreeItemViewModel* parentNode = findItemByIndex(parent);

    qDebug() << "onRowsInserted" << parent.data() << first << last;

    int firstRow = 0;
    int lastRow = 0;

    for (int row = first; row < last + 1; ++row) {
        // QModelIndex childIndex = parent.child(row, 0);
        QModelIndex childIndex= index(row,0,parent);
        TreeItemViewModel* n = parentNode->insertChild(row, childIndex);
        if (row == first)
            firstRow = n->row();
        if (row == last)
            lastRow = n->row();
    }
    beginInsertRows(QModelIndex(), firstRow, lastRow);
    endInsertRows();
}

void TreeViewModel::onRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destinationParent, int destinationRow)
{
    qDebug() << "onRowsMoved";
}

void TreeViewModel::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    qDebug() << "onRowsRemoved";
}

int TreeViewModel::flatten(QAbstractItemModel *model, QModelIndex parent, TreeItemViewModel *parentNode)
{
    if (parentNode == nullptr) {
        qDeleteAll(flattenedTree_);
        flattenedTree_.clear();
    }

    if (model == nullptr)
        return 0;

    auto rows = model->rowCount(parent);

    for(int rowIndex = 0; rowIndex < rows; ++rowIndex) {
        QModelIndex index = model->index(rowIndex, 0, parent);
        TreeItemViewModel* node = nullptr;
        if (parentNode)
            node = parentNode->addChild(index);
        else
            node = new TreeItemViewModel(parentNode, index, flattenedTree_, this, expandedMap_, hiddenMap);

        if (node->hasChildren())
            flatten(model, index, node);

        if (node->isExpanded())
            node->setExpanded(node->isExpanded());
    }
}

void TreeViewModel::toggleIsExpanded(int row, bool isExpanded)
{
    flattenedTree_[row]->setExpanded(isExpanded);
}

void TreeViewModel::doResetModel(QAbstractItemModel *sourceModel)
{
    beginResetModel();
    flatten(sourceModel);
    endResetModel();
}

TreeItemViewModel *TreeViewModel::findItemByIndex(const QModelIndex &sourceIndex) const
{
    for (TreeItemViewModel* n: flattenedTree_) {
        if (n->sourceIndex() == sourceIndex)
            return n;
    }
    return nullptr;
}

