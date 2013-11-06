// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "derivedattributes.h"

DerivedAttributes::DerivedAttributes(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int	DerivedAttributes::rowCount( const QModelIndex & parent) const
{
    if( parent.isValid() )
        return 0;
    return names.size() + 1;
}

int	DerivedAttributes::columnCount( const QModelIndex & parent) const
{
    if( parent.isValid() )
        return 0;
    return 2;
}

QVariant DerivedAttributes::headerData( int section, Qt::Orientation orientation, int role) const
{
    if( role == Qt::DisplayRole )
    {
        if( orientation == Qt::Horizontal )
            if( section == 0 )
            {
                return QString("Name");
            }
            else if(section == 1)
            {
                return QString("Value");
            }
    }
    return QVariant();
}

QVariant DerivedAttributes::data( const QModelIndex & index, int role) const
{
    if( role != Qt::DisplayRole )
        return QVariant();

    if( index.row() == names.size() )
        return QString("...");
    else if( index.row() > names.size() || index.row() < 0 )
        return QVariant();

    if( index.column() == 0 )
    {
        return names[index.row()];
    }
    else
    {
        return values[index.row()];
    }
}

Qt::ItemFlags DerivedAttributes::flags ( const QModelIndex & index ) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

bool DerivedAttributes::setData ( const QModelIndex & index, const QVariant & value, int role)
{
    // When last row ("...") is modified, a new entry is added to the list
    if( index.row() == names.size() )
    {
        names.push_back(QString(" "));
        values.push_back(QString(" "));
    }

    if( index.row() > names.size() )
    {
        return false;
    }

    if( value.toString().isEmpty() )
    {
        names.removeAt(index.row());
        values.removeAt(index.row());
    }
    else
    {
        if( index.column() == 0 )
        {
            names[index.row()] = value.toString();
        }
        else
        {
            values[index.row()] = value.toString();
        }
    }
    layoutChanged();

    return true;
}

QString DerivedAttributes::nameAt(int row)
{
    if( row >= names.size() )
        return QString();
    return names[row];
}

QString DerivedAttributes::valueAt(int row)
{
    if( row >= values.size() )
        return QString();
    return values[row];
}

void DerivedAttributes::insertBack(QString name, QString value)
{
    names.push_back(name);
    values.push_back(value);
}
