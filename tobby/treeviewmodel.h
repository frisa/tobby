#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H

#include <QAbstractProxyModel>
#include <QList>
#include "treeitemviewmodel.h"

class TreeViewModel : public QAbstractProxyModel
{
public:

    enum TreeRoles {
        Indentation = Qt::UserRole + 1,
        HasChildren,
        IsExpanded,
        Hidden
    };

    TreeViewModel();
    TreeViewModel(QObject* parent= nullptr);

    // QAbstactProxyModel implementation
    void setSourceModel(QAbstractItemModel *sourceModel) override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;

    QModelIndex index(int row, int column=0, const QModelIndex &parent=QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    QVariant data(const QModelIndex &proxyIndex, int role) const override;
    bool setData(const QModelIndex &proxyIndex, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

private slots:
    void onLayoutChanged()
    {
        qDebug() << "onLayoutChanged";
        doResetModel(sourceModel());
    };

    void onSourceDataChanged(QModelIndex topLeft, QModelIndex bottomRight)
    {
        qDebug() << "onSourceDataChanged";
        emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight));
    }

    void onRowsInserted(const QModelIndex& parent, int first, int last)
    {
        TreeItemViewModel* parentNode = findItemByIndex(parent);

        qDebug() << "onRowsInserted" << parent.data() << first << last;

        int firstRow = 0;
        int lastRow = 0;

        for (int row = first; row < last + 1; ++row) {
            // QModelIndex childIndex = parent.child(row, 0);
            QModelIndex childIndex = index(row,0,parent);
            TreeItemViewModel* n = parentNode->insertChild(row, childIndex);
            if (row == first)
                firstRow = n->row();
            if (row == last)
                lastRow = n->row();
        }
        beginInsertRows(QModelIndex(), firstRow, lastRow);
        endInsertRows();
    }

    void onRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destinationParent,
                     int destinationRow)
    {
        qDebug() << "onRowsMoved";
    }

    void onRowsRemoved(const QModelIndex& parent, int first, int last)
    {
        qDebug() << "onRowsRemoved";
    }

private:
    int flatten(QAbstractItemModel *model, QModelIndex parent = QModelIndex(), TreeItemViewModel* parentNode= nullptr);
    void toggleIsExpanded(int row, bool isExpanded);
    void doResetModel(QAbstractItemModel *sourceModel);
    TreeItemViewModel* findItemByIndex(const QModelIndex &sourceIndex) const;
    QList<TreeItemViewModel*> flattenedTree_;
    QMap<QModelIndex, bool> expandedMap_;
    QMap<QModelIndex, bool> hiddenMap;
//    TreeItemViewModel* rootItem_ = nullptr;
};

#endif // TREEVIEWMODEL_H
