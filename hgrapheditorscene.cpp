#include "hgrapheditorscene.h"
#include "hgrapheditormgr.h"
#include "hgrapheditordoc.h"
#include "hicongraphicsitem.h"
#include "hiconlineitem.h"
#include "hiconrectitem.h"
#include "hiconellipseitem.h"
#include "hiconcircleitem.h"
#include "hiconpolygonitem.h"
#include "hiconpolylineitem.h"
#include "hiconarcitem.h"
#include "hiconpieitem.h"
#include "hicontextitem.h"
#include "hiconitemgroup.h"
#include "hiconshowpattern.h"
#include "hiconselectionitem.h"
#include "hiconcomplexitem.h"
#include "hbaseobj.h"
#include "hiconobj.h"
#include "hiconrectobj.h"
#include "hgraph.h"
#include <QMimeData>
HGraphEditorScene::HGraphEditorScene(HGraphEditorMgr *mgr)
    :pGraphEditorMgr(mgr)
{
    //setAcceptDraps(true);
    complex = 0;
    rectangle = 0;
}

void HGraphEditorScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if(NULL == pGraphEditorMgr)
        return;
    painter->save();
    painter->setBrush(backgroundBrush());
    QGraphicsScene::drawBackground(painter,rect);

    HGraph* pGraph = pGraphEditorMgr->graphEditorDoc()->getCurGraph();
    QColor clr;
    if(pGraph)
    {
        clr = QColor(pGraph->getFillColor());
    }
    else
    {
        clr = QColor(Qt::darkGray);
    }
    QRectF rectF = pGraphEditorMgr->getLogicRect();
    painter->fillRect(rectF,clr);
    painter->restore();
}

void HGraphEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pt = mouseEvent->scenePos();
    QList<QGraphicsItem*> itemlist = items();
    for(int i = 0; i < itemlist.count();i++)
    {
        HIconGraphicsItem* item = qgraphicsitem_cast<HIconGraphicsItem*>(itemlist[i]);
        HBaseObj* pObj = item->getItemObj();
        int a = 0;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void HGraphEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //判断当前是否处于选择状态
    //if(drawShape == enumSelection)
    //{
      //  setItemCursor(mouseEvent);
    QList<QGraphicsItem*> itemlist = selectedItems();
    for(int i = 0; i < itemlist.count();i++)
    {
        HIconGraphicsItem* item = qgraphicsitem_cast<HIconGraphicsItem*>(itemlist[i]);
    }
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    //}
}

void HGraphEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void HGraphEditorScene::drawForeground(QPainter *painter, const QRectF &rect)
{

}

void HGraphEditorScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{

}

void HGraphEditorScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    dragMoveItems.clear();
    QPointF point = event->scenePos();

    if(event->mimeData()->hasFormat("DragIcon"))
    {
        ptStart = point;
        QByteArray itemData = event->mimeData()->data("DragIcon");//数组
        QDataStream dataStream(&itemData,QIODevice::ReadOnly);//读数据到数组的流
        //uuid,name,type uuid,红绿灯,遥信
        QString strUuid,strTypeName;
        int nShapeType = 0;
        dataStream>>strUuid>>strTypeName>>nShapeType; //读入数据到数组

        pGraphEditorMgr->createIconObj(strTypeName,strUuid,nShapeType,point,dragMoveItems);
        event->setDropAction(Qt::MoveAction);
        event->accept();

    }
    else
        event->ignore();

}

void HGraphEditorScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{

}

void HGraphEditorScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QPointF pointF = event->scenePos();
    if(event->mimeData()->hasFormat("DragIcon"))
    {
        if(complex != 0)
        {
            double dx,dy;
            dx = pointF.x()-ptStart.x();
            dy = pointF.y()-ptStart.y();
            complex->moveItemBy(dx,dy);
            //HBaseObj* pObj = complex->getItemObj();
            //HIconComplexObj* pObj1 = (HIconComplexObj*)pObj;
            //pObj->moveBy(dx,dy);
            ptStart = pointF;
        }
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->ignore();
}

void HGraphEditorScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QPointF pointF = event->scenePos();
    if(event->mimeData()->hasFormat("DragIcon"))
    {
        HBaseObj * pObj = complex->getItemObj();
        DRAWSHAPE drawShape = pObj->getShapeType();

        if(drawShape == enumComplex && complex != 0)
        {
            //HIconComplexObj* pObj1 = (HIconComplexObj*)pObj;
            //pObj1->resetRectPoint(pointF,ptStart);
            //pObj1->resize(1,1);
            //pObj1->moveBy(pointF.x()-ptStart)
            complex = 0;
        }
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->ignore();
}

void HGraphEditorScene::addIconGraphicsItem(HBaseObj* pObj)
{
    DRAWSHAPE drawShape = pObj->getShapeType();
    int nZValue = pObj->getStackOrder();
    if(drawShape == enumLine)
    {
        line = new HIconLineItem(QLineF(((HLineObj*) pObj)->getHeadPoint(),((HLineObj*)pObj)->getTailPoint()));
        line->setItemObj(pObj);
        line->setZValue(nZValue);
        addItem(line);

    }
    else if(drawShape == enumRectangle)
    {
        HRectObj* pObj1 = (HRectObj*)pObj;
        rectangle = new HIconRectItem(pObj1->getObjRect());
        rectangle->setItemObj(pObj);
        rectangle->setZValue(nZValue);
        addItem(rectangle);
    }
    else if(drawShape == enumEllipse)
    {
        HEllipseObj* pObj1 = (HEllipseObj*)pObj;
        ellipse = new HIconEllipseItem(pObj1->getObjRect());
        ellipse->setItemObj(pObj);
        ellipse->setZValue(nZValue);
        addItem(ellipse);
    }
    else if(drawShape == enumCircle)
    {
        HCircleObj* pObj1 = (HCircleObj*)pObj;
        circle = new HIconCircleItem(pObj1->getObjRect());
        circle->setItemObj(pObj);
        circle->setZValue(nZValue);
        addItem(circle);
    }
    else if(drawShape == enumPolygon)
    {
        HPolygonObj* pObj1 = (HPolygonObj*)pObj;
        polygon = new HIconPolygonItem(pObj1->pylist);
        polygon->setItemObj(pObj);
        polygon->setZValue(nZValue);
        addItem(polygon);
    }
    else if(drawShape == enumPolyline)
    {
        HPolylineObj* pObj1 = (HPolylineObj*)pObj;
        polyline = new HIconPolylineItem(pObj1->pylist);
        polyline->setItemObj(pObj);
        polyline->setZValue(nZValue);
        addItem(polyline);
    }
    else if(drawShape == enumArc)
    {
        HArcObj* pObj1 = (HArcObj*)pObj;
        arc = new HIconArcItem(pObj1->getObjRect());
        arc->setItemObj(pObj);
        arc->setZValue(nZValue);
        addItem(arc);
    }
    else if(drawShape == enumPie)
    {
        HPieObj* pObj1 = (HPieObj*)pObj;
        pie = new HIconPieItem(pObj1->getObjRect());
        pie->setItemObj(pObj);
        pie->setZValue(nZValue);
        addItem(pie);
    }
    else if(drawShape == enumText)
    {
        HTextObj* pObj1 = (HTextObj*)pObj;
        text = new HIconTextItem(pObj1->getObjRect());
        text->setItemObj(pObj);
        text->setZValue(nZValue);
        addItem(text);
    }
    else if(drawShape == enumComplex)
    {
        HIconComplexObj* pObj1 = (HIconComplexObj*)pObj;
        complex = new HIconComplexItem(pObj1->getObjRect());
        complex->setItemObj(pObj);
        complex->setZValue(nZValue);
        addItem(complex);
    }
    //if(complex != 0)
     //   complex->setAcceptDrops(true);
}

void HGraphEditorScene::delGraphEditorSceneItems()
{
    foreach (QGraphicsItem *item, items())
    {
        HIconGraphicsItem* pItem = qgraphicsitem_cast<HIconGraphicsItem*>(item);
        if(!pItem) continue;
        removeItem(item);
        delete item;
     }
}
