// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#ifndef HEESGRAPHICSITEM_H
#define HEESGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QString>
#include "derivedattributes.h"
#include "sensorlist.h"
#include "arrowitem.h"

enum ItemType{
    SOURCE, BANK, LOAD, CTI, CONVERTER, MANAGER
};

class ArrowItem;

// Model for HEESItemWidget
class HEESGraphicsItem : public QGraphicsPolygonItem
{
public:
    HEESGraphicsItem(int t);
    ~HEESGraphicsItem();

    DerivedAttributes * myAttributes();
    SensorList * mySensors();
    ItemType myType();

    void setLeftItem(HEESGraphicsItem *item);
    void setRightItem(HEESGraphicsItem *item);
    const HEESGraphicsItem* getLeftItem();
    const HEESGraphicsItem* getRightItem();
    void addConverter(HEESGraphicsItem *converter);
    void removeConverter(HEESGraphicsItem *converter);
    void removeAllConverters();
    void portItemRemoved(HEESGraphicsItem *item);
    void updateArrowLocation();
    QString getPortAName();
    QString getPortBName();
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

public:
    QString name;
    QString derivedType;

private:
    ItemType type;
    DerivedAttributes attributes;
    SensorList sensors;

    HEESGraphicsItem *portAItem;
    HEESGraphicsItem *portBItem;

    ArrowItem *leftArrow;
    ArrowItem *rightArrow;
    QList<HEESGraphicsItem*> connectedConverters;
    bool willBeRemoved;
};

#endif // HEESGRAPHICSITEM_H
