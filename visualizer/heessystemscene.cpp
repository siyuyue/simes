#include "heessystemscene.h"

HEESSystemScene::HEESSystemScene(QObject *parent)
    : QGraphicsScene(parent)
{
    myMode = DEFAULT;
}

HEESSystemScene::~HEESSystemScene()
{
    clearAllItems();
}

void HEESSystemScene::clearAllItems()
{
    QList<QGraphicsItem *> itemList = this->items();
    for( int i=0; i<itemList.size(); i++ )
        delete itemList[i];
    this->clear();
}

void HEESSystemScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if( myMode == ADD_ARROW )
    {
        if( mouseEvent->button() != Qt::LeftButton )
        {
            emit portSelected( NULL );
            myMode = DEFAULT;
            return;
        }

        QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());

        if( item != NULL )
        {
            // dynamic_cast returns a NULL pointer if item is not the right type
            emit portSelected( dynamic_cast<HEESGraphicsItem*>(item) );
            myMode = DEFAULT;
        }
    }
    else
    {
        // Event is propogated so that selection, drag and drop still work in the original way
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
}

void HEESSystemScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // Ignore double click event
    return;
}

void HEESSystemScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // Event is propogated so that selection, drag and drop still work in the original
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void HEESSystemScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // Event is propogated so that selection, drag and drop still work in the original
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void HEESSystemScene::setAddArrowMode()
{
    myMode = ADD_ARROW;
}
