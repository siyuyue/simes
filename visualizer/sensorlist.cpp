// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "sensorlist.h"

SensorList::SensorList(QObject *parent) :
    QAbstractListModel(parent)
{
}

Qt::ItemFlags SensorList::flags( const QModelIndex & index ) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

int SensorList::rowCount( const QModelIndex & parent ) const
{
    // Last row displays "..."
    return sensors.size() + 1;
}

QVariant SensorList::data( const QModelIndex & index, int role ) const
{
    if( role != Qt::DisplayRole )
        return QVariant();

    // Last row displays "...", else part is needed to prevent crashes
    if( index.row() == sensors.size() )
        return QString("...");
    else if( index.row() > sensors.size() || index.row() < 0 )
        return QVariant();

    return sensors[index.row()];
}

bool SensorList::setData( const QModelIndex & index, const QVariant & value, int role )
{
    // When last row ("...") is modified, a new entry is added to the list
    if( index.row() == sensors.size() )
    {
        sensors.push_back(QString());
    }

    if( index.row() > sensors.size() )
    {
        return false;
    }

    if( value.toString().isEmpty() )
        sensors.removeAt(index.row());
    else
        sensors[index.row()] = value.toString();

    layoutChanged();

    return true;
}

int SensorList::size()
{
    return sensors.size();
}

QString SensorList::sensorAt(int row)
{
    if( row >= sensors.size() )
        return QString();
    return sensors[row];
}

void SensorList::insertBack(QString sensor)
{
    sensors.push_back(sensor);
    layoutChanged();;
}
