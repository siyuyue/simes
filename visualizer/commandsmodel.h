// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#ifndef COMMANDSMODEL_H
#define COMMANDSMODEL_H

#include <QAbstractTableModel>

struct Command
{
    double time;
    QString type;       // should be one of "set", "get", "sim" or "finish"
    QString targetName;
    QString propertyName;
    QString propertyValue;
};

class CommandsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CommandsModel(QObject *parent = 0);

    virtual int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    int size();
    Command * commandAt(int row);
    void insertBack(Command &cmd);
    void clear();
    
signals:
    
public slots:

private:
    QList<Command> commands;
};

#endif // COMMANDSMODEL_H
