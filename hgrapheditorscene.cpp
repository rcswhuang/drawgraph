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
#include "hiconapi.h"
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
    line = 0;
    ellipse = 0;
    circle = 0;
    polygon = 0;
    polyline = 0;
    arc = 0;
    pie = 0;
    text = 0;
    select = 0;
    group = 0;
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
    if(mouseEvent->button() != Qt::LeftButton)
        return;

    prePoint = mouseEvent->scenePos();
    newIconGraphicsObj();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void HGraphEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!pGraphEditorMgr)
        return;
    DRAWSHAPE drawShape = pGraphEditorMgr->getDrawShape();
    curPoint = mouseEvent->scenePos();
    if(drawShape == enumLine && line != 0)
    {
        QLineF newline = QLineF(prePoint,curPoint);
        line->setLine(newline);
    }
    else if(drawShape == enumRectangle && rectangle != 0)
    {
        QRectF newRect = QRectF(prePoint,curPoint).normalized();
        rectangle->setRect(newRect);
    }
    else if(drawShape == enumEllipse && ellipse != 0)
    {
        QRectF newRect = QRectF(prePoint,curPoint).normalized();
        ellipse->setRect(newRect);
    }
    else if(drawShape == enumCircle && circle != 0)
    {
        qreal dx = qAbs(curPoint.x() - prePoint.x());
        qreal dy = qAbs(curPoint.y() - prePoint.x());
        QRectF newRect = QRectF(prePoint,QSize(qMin(dx,dy),qMin(dx,dy)));
        circle->setRect(newRect);
    }
    else if(drawShape == enumPolygon && polygon != 0)
    {
        QPolygonF tempF = polygon->polygon();
        tempF.replace(tempF.length()-1,curPoint);
        polygon->setPolygon(tempF);
    }
    else if(drawShape == enumPolyline && polyline != 0)
    {
        QPolygonF tempF = polyline->polygon();
        tempF.replace(tempF.length()-1,curPoint);
        polyline->setPolygon(tempF);
    }
    else if(drawShape == enumArc && arc != 0)
    {
        QRectF newRect = QRectF(prePoint,curPoint).normalized();
        arc->setRect(newRect);
    }
    else if(drawShape == enumPie && pie != 0)
    {
        QRectF newRect = QRectF(prePoint,curPoint).normalized();
        pie->setRect(newRect);
    }
    else if(drawShape == enumText && text != 0)
    {
        QRectF newRect = QRectF(prePoint,curPoint).normalized();
        text->setRect(newRect);
    }
    else if(drawShape == enumMulSelection)
    {
        QRectF newRect = QRectF(prePoint,curPoint).normalized();
        select->setRect(newRect.normalized());
    }
    //判断当前是否处于选择状态
    if(drawShape == enumSelection)
    {
        setItemCursor(mouseEvent);
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void HGraphEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!pGraphEditorMgr)
        return;
    DRAWSHAPE drawShape = pGraphEditorMgr->getDrawShape();
    if(drawShape == enumLine && line != 0)
    {
        line->getItemObj()->setModify(true);
        emit itemInserted(line->type());
        line = 0;
    }
    else if(drawShape == enumRectangle && rectangle != 0)
    {
        rectangle->getItemObj()->setModify(true);
        emit itemInserted(rectangle->type());
        rectangle = 0;
    }
    else if(drawShape == enumEllipse && ellipse != 0)
    {
        ellipse->getItemObj()->setModify(true);
        emit itemInserted(ellipse->type());
        ellipse = 0;
    }
    else if(drawShape == enumCircle && circle != 0)
    {
        circle->getItemObj()->setModify(true);
        emit itemInserted(circle->type());
        circle = 0;
    }
    else if(drawShape == enumArc && arc !=0)
    {
        arc->getItemObj()->setModify(true);
        emit itemInserted(arc->type());
        arc = 0;
    }
    else if(drawShape == enumPie && pie !=0)
    {
        pie->getItemObj()->setModify(true);
        emit itemInserted(pie->type());
        pie = 0;
    }
    else if(drawShape == enumText && text != 0)
    {
        text->getItemObj()->setModify(true);
        emit itemInserted(text->type());
        text = 0;
    }
    else if(drawShape == enumPolygon && polygon != 0)
    {
        return QGraphicsScene::mouseReleaseEvent(mouseEvent);;
    }
    else if(drawShape == enumPolyline && polyline !=0)
    {
        return QGraphicsScene::mouseReleaseEvent(mouseEvent);;
    }
    else if(drawShape == enumMulSelection && select != 0)
    {
        //计算选择点
        QRectF rectF = select->rect();
        //calcSelectedItem(rectF);//判断item是否选到 选到就是enumSelect否则enumNo
        removeItem(select);
        delete select;
        select = 0;
    }
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
    if(event->mimeData()->hasFormat("DragIcon"))
    {
        HBaseObj * pObj = complex->getItemObj();
        DRAWSHAPE drawShape = pObj->getShapeType();

        if(drawShape == enumComplex && complex != 0)
        {
            complex->getItemObj()->setModify(true);
            emit itemInserted(complex->type());
            complex = 0;
        }
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->ignore();
}

void HGraphEditorScene::newIconGraphicsObj()
{
    if(!pGraphEditorMgr)
        return;
    DRAWSHAPE drawShape = pGraphEditorMgr->getDrawShape();
    switch (drawShape) {
    case enumLine:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumLine);
        HLineObj* pObj1 = (HLineObj*)pObj;
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        pObj1->setHeadPoint(prePoint);
        pObj1->setTailPoint(prePoint);
        addIconGraphicsItem(pObj1);
    }
        break;
    case enumRectangle:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumRectangle);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HRectObj* pObj1 = (HRectObj*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);

    }
        break;
    case enumEllipse:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumEllipse);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HRectObj* pObj1 = (HRectObj*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);

    }
        break;
    case enumCircle:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumCircle);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HRectObj* pObj1 = (HRectObj*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);

    }
        break;
    case enumArc:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumArc);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HRectObj* pObj1 = (HRectObj*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);

    }
        break;
    case enumPie:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumPie);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HRectObj* pObj1 = (HRectObj*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);

    }
        break;
    case enumPolygon:
    {
        if(polygon == 0)
        {
            HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumLine);
            pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
            HPolygonObj* pObj1 = (HPolygonObj*)pObj;
            pObj1->pylist<<prePoint<<prePoint;
            addIconGraphicsItem(pObj);
            //addNewIconCommand(polygon->getItemObj());
        }
        else
        {
            QPolygonF tempF = polygon->polygon();
            tempF.replace(tempF.length()-1,prePoint);
            tempF.append(prePoint);
            polygon->setPolygon(tempF);
        }
    }
        break;
    case enumPolyline:
    {
        if(polyline == 0)
        {
            HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumPolyline);
            pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
            HPolygonObj* pObj1 = (HPolygonObj*)pObj;
            pObj1->pylist<<prePoint<<prePoint;
            addIconGraphicsItem(pObj);
            //addNewIconCommand(polyline->getItemObj());
        }
        else
        {
            QPolygonF tempF = polyline->polygon();
            tempF.replace(tempF.length()-1,prePoint);
            tempF.append(prePoint);
            polyline->setPolygon(tempF);
            //addNewIconCommand(polygon->getItemObj());
        }
    }
        break;
    case enumMulSelection:
    {
        QRectF tempF = QRectF(prePoint,prePoint).normalized();
        select = new HIconSelectionItem(tempF);
        addItem(select);
        break;
    }
    default:
        break;
    }
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
        complex = new HIconComplexItem(pObj1->getObjRect().normalized());
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

void HGraphEditorScene::setItemCursor(QGraphicsSceneMouseEvent *mouseEvent)
{
    QList<QGraphicsItem*> itemList = selectedItems();
    //获取所在item，只要item选择到即可
    //这个地方要改，只要获取当前选择那个，如果是多个选择则返回
    QPointF pointF = mouseEvent->scenePos();
    if(itemList.count()>=2 || itemList.count() == 0) return;
    QGraphicsItem* item = itemList.at(0);
    if(!item) return;
    HIconGraphicsItem* pItem = qgraphicsitem_cast<HIconGraphicsItem*>(item);
    int location = pItem->pointInRect(pointF);
    pItem->setItemCursor(location);
}
