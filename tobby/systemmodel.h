#ifndef SYSTEMMODEL_H
#define SYSTEMMODEL_H
#include <QAbstractItemModel>

#include <string>

using namespace std;


class SystemModel: public QAbstractItemModel
{
public:
    SystemModel();
    ~SystemModel();
    string getPCName();

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // SYSTEMMODEL_H
