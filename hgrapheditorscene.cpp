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
#include "hiconsymbol.h"
#include "hgraph.h"
#include "hdigitalpage.h"
#include "hanaloguepage.h"
#include "hrelaypage.h"
#include "hiconproperty.h"
#include "hgraphpage.h"
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

void HGraphEditorScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if(!pGraphEditorMgr->graphEditorDoc() || !pGraphEditorMgr->graphEditorDoc()->getCurGraph())
        return;
    QList<QGraphicsItem*> itemList = selectedItems();
    if(itemList.size()== 1)
    {
        QGraphicsItem* pItem = itemList.first();
        setItemProperty(pItem);
    }
    else
    {
        {
            HGraph* graph = pGraphEditorMgr->graphEditorDoc()->getCurGraph();
            HGraphPage page(graph);
            page.exec();
        }
    }
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void HGraphEditorScene::setItemProperty(QGraphicsItem* item)
{
    if(!item) return;
    HIconGraphicsItem* pItem = qgraphicsitem_cast<HIconGraphicsItem*>(item);
    HBaseObj* pObj = pItem->getItemObj();
    DRAWSHAPE drawShape = pObj->getShapeType();
    if(drawShape == enumComplex)
    {
        HIconObj* pComplexObj = (HIconObj*)pObj;
        int nPointType = pComplexObj->getObjType();
        if(nPointType == TEMPLATE_TYPE_DIGITAL)
        {
            HDigitalPage dlg(pComplexObj);
            dlg.exec();
        }
        else if(nPointType == TEMPLATE_TYPE_ANALOGUE)
        {
            HAnaloguePage dlg(pComplexObj);
            dlg.exec();
        }
        else if(nPointType == TEMPLATE_TYPE_RELAY || nPointType == TEMPLATE_TYPE_CONTROL)
        {
            HRelayPage dlg(pComplexObj);
            dlg.exec();
        }
    }
    else
    {
        HPropertyDlg dlg(pObj);
        dlg.exec();
    }
}

void HGraphEditorScene::drawForeground(QPainter *painter, const QRectF &rect)
{

}

void HGraphEditorScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    DRAWSHAPE drawShape = pGraphEditorMgr->getDrawShape();
    if(drawShape == enumPolygon && polygon != 0)
    {
        if(polygon->polygon().size()<=2)
        {
            HBaseObj* pObj = polygon->getItemObj();
            pObj->setDeleted(true);
            polygon->setVisible(false);
        }
        else
        {
            QPolygonF tempF = polygon->polygon();
            tempF.replace(tempF.size()-1,event->scenePos());
            tempF.append(tempF.at(0));
            polygon->setPolygon(tempF);
        }
        polygon->getItemObj()->setModify(true);
        emit itemInserted(polygon->type());
        polygon = 0;
        return;
    }
    else if(drawShape == enumPolyline && polyline != 0)
    {
        if(polyline->polygon().size()<=2)
        {
            HBaseObj* pObj = polyline->getItemObj();
            pObj->setDeleted(true);
            polyline->setVisible(false);
        }
        else
        {
            QPolygonF tempF = polyline->polygon();
            tempF.replace(tempF.size()-1,event->scenePos());
            polyline->setPolygon(tempF);
        }
        polyline->getItemObj()->setModify(true);
        emit itemInserted(polyline->type());
        polyline = 0;
        return;
    }

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
        HLine* pObj1 = (HLine*)pObj;
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
        HRectangle* pObj1 = (HRectangle*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);

    }
        break;
    case enumEllipse:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumEllipse);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HEllipse* pObj1 = (HEllipse*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);

    }
        break;
    case enumCircle:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumCircle);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HCircle* pObj1 = (HCircle*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);

    }
        break;
    case enumArc:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumArc);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HArc* pObj1 = (HArc*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);

    }
        break;
    case enumPie:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumPie);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HPie* pObj1 = (HPie*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);

    }
        break;
    case enumPolygon:
    {
        if(polygon == 0)
        {
            HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumPolygon);
            pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
            HPolygon* pObj1 = (HPolygon*)pObj;
            pObj1->pylist<<prePoint<<prePoint;
            addIconGraphicsItem(pObj1);
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
            HPolyline* pObj1 = (HPolyline*)pObj;
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
    case enumText:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumText);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HText* pObj1 = (HText*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        addIconGraphicsItem(pObj1);
    }
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

void HGraphEditorScene::addIconGraphicsItem(HBaseObj* pObj,bool bdel)
{
    DRAWSHAPE drawShape = pObj->getShapeType();
    int nZValue = pObj->getStackOrder();
    if(drawShape == enumLine)
    {
        line = new HIconLineItem(QLineF(((HLine*) pObj)->getHeadPoint(),((HLine*)pObj)->getTailPoint()));
        line->setItemObj(pObj);
        line->setZValue(nZValue);
        addItem(line);

    }
    else if(drawShape == enumRectangle)
    {
        HRectangle* pObj1 = (HRectangle*)pObj;
        rectangle = new HIconRectItem(pObj1->getObjRect());
        rectangle->setItemObj(pObj);
        rectangle->setZValue(nZValue);
        addItem(rectangle);
    }
    else if(drawShape == enumEllipse)
    {
        HEllipse* pObj1 = (HEllipse*)pObj;
        ellipse = new HIconEllipseItem(pObj1->getObjRect());
        ellipse->setItemObj(pObj);
        ellipse->setZValue(nZValue);
        addItem(ellipse);
    }
    else if(drawShape == enumCircle)
    {
        HCircle* pObj1 = (HCircle*)pObj;
        circle = new HIconCircleItem(pObj1->getObjRect());
        circle->setItemObj(pObj);
        circle->setZValue(nZValue);
        addItem(circle);
    }
    else if(drawShape == enumPolygon)
    {
        HPolygon* pObj1 = (HPolygon*)pObj;
        polygon = new HIconPolygonItem(pObj1->pylist);
        polygon->setItemObj(pObj);
        polygon->setZValue(nZValue);
        addItem(polygon);
    }
    else if(drawShape == enumPolyline)
    {
        HPolyline* pObj1 = (HPolyline*)pObj;
        polyline = new HIconPolylineItem(pObj1->pylist);
        polyline->setItemObj(pObj);
        polyline->setZValue(nZValue);
        addItem(polyline);
    }
    else if(drawShape == enumArc)
    {
        HArc* pObj1 = (HArc*)pObj;
        arc = new HIconArcItem(pObj1->getObjRect());
        arc->setItemObj(pObj);
        arc->setZValue(nZValue);
        addItem(arc);
    }
    else if(drawShape == enumPie)
    {
        HPie* pObj1 = (HPie*)pObj;
        pie = new HIconPieItem(pObj1->getObjRect());
        pie->setItemObj(pObj);
        pie->setZValue(nZValue);
        addItem(pie);
    }
    else if(drawShape == enumText)
    {
        HText* pObj1 = (HText*)pObj;
        text = new HIconTextItem(pObj1->getObjRect());
        text->setItemObj(pObj);
        text->setZValue(nZValue);
        addItem(text);
    }
    else if(drawShape == enumComplex)
    {
        HIconObj* pObj1 = (HIconObj*)pObj;
        complex = new HIconComplexItem(pObj1->getObjRect().normalized());
        complex->setItemObj(pObj);
        complex->setZValue(nZValue);
        addItem(complex);
    }
    //if(complex != 0)
     //   complex->setAcceptDrops(true);
    if(bdel)
    {
        if(drawShape == enumLine && line != 0)
        {
            line = 0;
        }
        else if(drawShape == enumRectangle && rectangle != 0)
        {
            rectangle = 0;
        }
        else if(drawShape == enumEllipse && ellipse != 0)
        {
            ellipse = 0;
        }
        else if(drawShape == enumCircle && circle != 0)
        {
            circle = 0;
        }
        else if(drawShape == enumPolyline && polyline != 0)
        {
            polyline = 0;
        }
        else if(drawShape == enumPolygon && polygon !=0)
        {
            polygon = 0;
        }
        else if(drawShape == enumArc && arc !=0)
        {
            arc = 0;
        }
        else if(drawShape == enumPie && pie !=0)
        {
            pie = 0;
        }
        else if(drawShape == enumText && text != 0)
        {
            text = 0;
        }
        else if(drawShape == enumComplex && complex != 0)
        {
            complex = 0;
        }
    }
}

void HGraphEditorScene::openGraphEditorSceneItems()
{
    if(!pGraphEditorMgr)
        return;
    if(!pGraphEditorMgr->graphEditorDoc())
        return;
    HGraph* pGraph = pGraphEditorMgr->graphEditorDoc()->getCurGraph();
    if(!pGraph)
        return;
    QList<HBaseObj*> pObjList = pGraph->getObjList();
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        if(!pObj)
            continue;
        addIconGraphicsItem(pObj,true);
    }
}

void HGraphEditorScene::delGraphEditorSceneItems()
{
    foreach (QGraphicsItem *item, items())
    {
        HIconGraphicsItem* pItem = qgraphicsitem_cast<HIconGraphicsItem*>(item);
        if(!pItem) continue;
        removeItem(pItem);
        delete pItem;
     }
}

void HGraphEditorScene::setItemCursor(QGraphicsSceneMouseEvent *mouseEvent)
{
    QList<QGraphicsItem*> itemList = selectedItems();
    //获取所在item，只要item选择到即可
    //这个地方要改，只要获取当前选择那个，如果是多个选择则返回 ---huangw
    QPointF pointF = mouseEvent->scenePos();
    if(itemList.count()>=2 || itemList.count() == 0) return;
    QGraphicsItem* item = itemList.at(0);
    if(!item) return;
    HIconGraphicsItem* pItem = qgraphicsitem_cast<HIconGraphicsItem*>(item);
    int location = pItem->pointInRect(pointF);
    pItem->setItemCursor(location);
}
