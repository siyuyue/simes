#ifndef HEESSYSTEMSCENE_H
#define HEESSYSTEMSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include "heesgraphicsitem.h"

class HEESSystemScene : public QGraphicsScene
{
    Q_OBJECT
    // ADD_ARROW mode is to select an item for a converter to establish connection
    enum Mode{ADD_ARROW, DEFAULT};

public:
    explicit HEESSystemScene(QObject *parent = 0);
    virtual ~HEESSystemScene();
    void setAddArrowMode();
    void clearAllItems();

signals:
    void portSelected(HEESGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
     Mode myMode;
};

#endif // HEESSYSTEMSCENE_H
