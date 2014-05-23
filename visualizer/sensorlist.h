// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#ifndef SENSORLIST_H
#define SENSORLIST_H

#include <QAbstractListModel>

class SensorList : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SensorList(QObject *parent = 0);
    virtual Qt::ItemFlags flags( const QModelIndex & index ) const;
    virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    int size();
    QString sensorAt(int row);
    void insertBack(QString sensor);
    
signals:
    
public slots:

private:
    QList<QString> sensors;
};

#endif // SENSORLIST_H
