// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#ifndef ARROWITEM_H
#define ARROWITEM_H

#include "heesgraphicsitem.h"
#include <QGraphicsLineItem>

class HEESGraphicsItem;

class ArrowItem : public QGraphicsLineItem
{
public:
    explicit ArrowItem(bool isLeftPort, QGraphicsItem *parent);
    ~ArrowItem();

    void setEndItem(HEESGraphicsItem* item);

public:
    void updatePosition();

private:
    HEESGraphicsItem *startItem;
    HEESGraphicsItem *endItem;
    qreal startOffsetX;
    qreal startOffsetY;
    qreal endOffsetX;
    qreal endOffsetY;
};

#endif // ARROWITEM_H
