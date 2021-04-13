#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H

#include <QList>
#include <QAbstractProxyModel>
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

    TreeViewModel(QObject* parent= nullptr);

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
    void onLayoutChanged();
    void onSourceDataChanged(QModelIndex topLeft, QModelIndex bottomRight);
    void onRowsInserted(const QModelIndex& parent, int first, int last);
    void onRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destinationParent, int destinationRow);
    void onRowsRemoved(const QModelIndex& parent, int first, int last);

private:
    int flatten(QAbstractItemModel *model, QModelIndex parent = QModelIndex(), TreeItemViewModel* parentNode= nullptr);
    void toggleIsExpanded(int row, bool isExpanded);
    void doResetModel(QAbstractItemModel *sourceModel);
    TreeItemViewModel* findItemByIndex(const QModelIndex &sourceIndex) const;
    QList<TreeItemViewModel*> flattenedTree_;
    QMap<QModelIndex, bool> expandedMap_;
    QMap<QModelIndex, bool> hiddenMap;
};

#endif // TREEVIEWMODEL_H
