#include "arrowitem.h"

ArrowItem::ArrowItem(bool isLeftPort, QGraphicsItem *parent):
    QGraphicsLineItem(parent)
{
    startItem = static_cast<HEESGraphicsItem*>(parent);
    endItem = NULL;
    if( isLeftPort )
    {
        startOffsetX = -30;
        startOffsetY = 15;
    }
    else
    {
        startOffsetX = 0;
        startOffsetY = 15;
    }

    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);

    setEndItem(NULL);
}

ArrowItem::~ArrowItem()
{
    // Called when startItem is deleted, should notify endItem since this arrow will be deleted as well
    if( endItem != NULL )
        endItem->removeConverter(startItem);
}

void ArrowItem::setEndItem(HEESGraphicsItem *item)
{
    // Notify endItem since this arrow has changed its end
    if(endItem != NULL)
        endItem->removeConverter(startItem);
    endItem = item;
    if(endItem == NULL)
        return;

    switch( endItem->myType() )
    {
    case SOURCE:
        endOffsetX = -25;
        endOffsetY = 0;
        break;
    case BANK:
        endOffsetX = -20;
        endOffsetY = 20;
        break;
    case LOAD:
        endOffsetX = -20;
        endOffsetY = 20;
        break;
    case CTI:
        endOffsetX = 0;
        endOffsetY = 40;
        break;
    }
    endItem->addConverter(startItem);

    QLineF line( mapFromItem(startItem, startOffsetX, startOffsetY), mapFromItem(endItem, endOffsetX, endOffsetY) );
    setLine(line);
}

void ArrowItem::updatePosition()
{
    if( endItem != NULL )
    {
        QLineF line( mapFromItem(startItem, startOffsetX, startOffsetY), mapFromItem(endItem, endOffsetX, endOffsetY) );
        setLine(line);
    }
    else
    {
        QLineF line( mapFromItem(startItem, startOffsetX, startOffsetY), mapFromItem(startItem, startOffsetX, startOffsetY) );
        setLine(line);
    }
}
