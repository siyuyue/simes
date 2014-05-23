// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "commandsmodel.h"

CommandsModel::CommandsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int	CommandsModel::rowCount( const QModelIndex & parent ) const
{
    if( parent.isValid() )
        return 0;
    return commands.size() + 1;
}

int	CommandsModel::columnCount( const QModelIndex & parent ) const
{
    if( parent.isValid() )
        return 0;
    return 5;
}

QVariant CommandsModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( role == Qt::DisplayRole )
    {
        if( orientation == Qt::Horizontal )
            if( section == 0 )
            {
                return QString("Time");
            }
            else if(section == 1)
            {
                return QString("Type");
            }
            else if(section == 2)
            {
                return QString("Target");
            }
            else if(section == 3)
            {
                return QString("Property");
            }
            else if(section == 4)
            {
                return QString("Value");
            }
    }
    return QVariant();
}

Qt::ItemFlags CommandsModel::flags( const QModelIndex & index ) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant CommandsModel::data( const QModelIndex & index, int role ) const
{
    if( role != Qt::DisplayRole )
        return QVariant();

    if( index.row() == commands.size() )
        return QString("...");
    else if( index.row() > commands.size() || index.row() < 0 )
        return QVariant();

    if( index.column() == 0 )
    {
        return commands[index.row()].time;
    }
    else if( index.column() == 1 )
    {
        return commands[index.row()].type;
    }
    else if( index.column() == 2 )
    {
        return commands[index.row()].targetName;
    }
    else if( index.column() == 3 )
    {
        return commands[index.row()].propertyName;
    }
    else
    {
        return commands[index.row()].propertyValue;
    }
}

bool CommandsModel::setData( const QModelIndex & index, const QVariant & value, int role )
{
    // When last row ("...") is modified, a new entry is added to the list
    if( index.row() == commands.size() )
    {
        commands.push_back(Command());
        commands[index.row()].time = 0;
    }

    if( index.row() > commands.size() )
    {
        return false;
    }

    if( index.column() == 0 && value.toString().isEmpty() )
    {
        commands.removeAt(index.row());
    }
    else
    {
        if( index.column() == 0 )
        {
            commands[index.row()].time = value.toDouble();
        }
        else if( index.column() == 1 )
        {
            commands[index.row()].type = value.toString();
        }
        else if( index.column() == 2 )
        {
            commands[index.row()].targetName = value.toString();
        }
        else if( index.column() == 3 )
        {
            commands[index.row()].propertyName = value.toString();
        }
        else
        {
            commands[index.row()].propertyValue = value.toString();
        }
    }
    layoutChanged();

    return true;
}

int CommandsModel::size()
{
    return commands.size();
}

Command * CommandsModel::commandAt(int row)
{
    if( row < commands.size() )
        return &(commands[row]);
    return NULL;
}

void CommandsModel::insertBack(Command &cmd)
{
    commands.push_back(cmd);
}

void CommandsModel::clear()
{
    commands.clear();
    layoutChanged();
}
