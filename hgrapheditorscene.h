#ifndef HGRAPHEDITORSCENE_H
#define HGRAPHEDITORSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
class HIconLineItem;
class HIconRectItem;
class HIconEllipseItem;
class HIconCircleItem;
class HIconPolygonItem;
class HIconPolylineItem;
class HIconArcItem;
class HIconPieItem;
class HIconTextItem;
class HIconComplexItem;
class HIconSelectionItem;
class HIconGraphicsItem;
class HIconItemGroup;
class HBaseObj;

class HGraphEditorMgr;
class HGraphEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    HGraphEditorScene(HGraphEditorMgr *mgr);

public:
    void delGraphEditorSceneItems();
    void addIconGraphicsItem(HBaseObj* pObj);
protected:

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

public:
    QPointF ptStart;
    HGraphEditorMgr* pGraphEditorMgr;
    HIconLineItem* line;
    HIconRectItem* rectangle;
    HIconEllipseItem* ellipse;
    HIconCircleItem* circle;
    HIconPolygonItem* polygon;
    HIconPolylineItem* polyline;
    HIconArcItem* arc;
    HIconPieItem* pie;
    HIconTextItem* text;
    HIconSelectionItem* select;
    HIconItemGroup* group;
    HIconComplexItem* complex;

public:
    QList<HIconGraphicsItem*> dragMoveItems;
};

#endif // HGRAPHEDITORSCENE_H
