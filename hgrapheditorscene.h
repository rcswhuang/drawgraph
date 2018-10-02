#ifndef HGRAPHEDITORSCENE_H
#define HGRAPHEDITORSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include "hiconapi.h"
class HIconLineItem;
class HIconRectangleItem;
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
    void openGraphEditorSceneItems();
    void delGraphEditorSceneItems();
    HIconGraphicsItem* addIconGraphicsItem(HBaseObj* pObj,bool del = false);//obj->item
    void removeItemInScene(HIconGraphicsItem* item);

    void setItemCursor(QGraphicsSceneMouseEvent *mouseEvent);
    void setItemProperty(QGraphicsItem* item);
    bool getItemAt(const QPointF &pos,bool bCtrl);



    QList<HIconGraphicsItem*> getSelectedItems();//不带选择框
    QRectF getMulSelectedItemsRect();
    void refreshSelectedItemRect(const QRectF &rectF = QRectF(0,0,0,0),bool bAreaSelect = false);
    void addItemInScene(HIconGraphicsItem* item);
    quint8 calcSelectedItem(const QRectF &rectF,bool bAreaSelect = true);
    void clearSelectItem();//清除select操作
    void clearMulSelectItems();


    int pointInRect(QPointF& pointF);
    void getGraphicsItemPointList(HIconGraphicsItem* item,QList<QPolygonF>& pfList);
    void prepareMoveItem(QGraphicsSceneMouseEvent *mouseEvent);
    void prepareRezieItem(QGraphicsSceneMouseEvent *mouseEvent);
    void updateScene();//当画面背景等改变后进行更新

public:
    //鼠标操作类
    void newIconGraphicsObj();//新建obj
    void onMousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void onMouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void onMouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
    //virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
signals:
    void itemInserted(int);
    void mousePosChanged(QPointF logPos);
    void selectItemChanged(int);

public:
    SELECTMODE nSelectMode;
    QPointF ptStart;
    QPointF prePoint;
    QPointF curPoint;
    bool bLeftShift;
    QList<QPolygonF> oldPolygonF;
    QList<QPolygonF> newPolygonF;

    HGraphEditorMgr* pGraphEditorMgr;
    HIconLineItem* line;
    HIconRectangleItem* rectangle;
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
    QList<HIconGraphicsItem*> m_pIconMulSelectItemsList; //多选item列表
};

#endif // HGRAPHEDITORSCENE_H
