// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#ifndef DERIVEDATTRIBUTES_H
#define DERIVEDATTRIBUTES_H

#include <QAbstractTableModel>

class DerivedAttributes : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DerivedAttributes(QObject *parent = 0);

    virtual int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    QString nameAt(int row);
    QString valueAt(int row);
    void insertBack(QString name, QString value);
    
signals:
    
public slots:
    
private:
    QList<QString> names;
    QList<QString> values;
};

#endif // DERIVEDATTRIBUTES_H
