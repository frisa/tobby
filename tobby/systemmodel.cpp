#include "systemmodel.h"

SystemModel::SystemModel()
{

}

SystemModel::~SystemModel()
{

}

string SystemModel::getPCName()
{
    return "jmeno PC";
}

QModelIndex SystemModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex SystemModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int SystemModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int SystemModel::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant SystemModel::data(const QModelIndex &index, int role) const
{
    return 0;
}
