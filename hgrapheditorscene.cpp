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
#include <QDebug>
#define TYPE_SELECT_NONE     0x00
#define TYPE_SELECT_SINGLE   0x01
#define TYPE_SELECT_MULTIPLE 0x02
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

//获取选择状态
QList<HIconGraphicsItem*> HGraphEditorScene::getSelectedItems()
{
    QList<HIconGraphicsItem*> pList;
    foreach (QGraphicsItem *item, selectedItems())
    {
        HIconGraphicsItem* pItem = qgraphicsitem_cast<HIconGraphicsItem*>(item);
        if(!pItem || pItem->type() == enumSelection)
        {
            continue;
        }
        pList.append(pItem);
    }
    return pList;
}

void HGraphEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() != Qt::LeftButton)
        return;
    if(!pGraphEditorMgr)
        return;
    SELECTMODE selectMode = pGraphEditorMgr->getSelectMode();
    prePoint = mouseEvent->scenePos();
    if(selectMode == enumSelect)
    {
        if(!getItemAt(prePoint))
        {
            pGraphEditorMgr->setDrawShape(enumSelection);
            selectMode = enumDraw;
        }
        else
        {
            ////pGraphEditorMgr->setDrawShape(enumNo);
            //nSelectMode = enumSelect;
        }

        /*
        if(nSelectMode == enumSelect)
        {
            if(pointInRect(pt) != 0)
                nSelectMode = enumSize;
            else
                nSelectMode = enumMove;
        }*/
    }
    if(selectMode == enumDraw)
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
    else if(drawShape == enumSelection && select != 0)
    {
        QRectF newRect = QRectF(prePoint,curPoint).normalized();
        select->setRect(newRect.normalized());
    }
    //判断当前是否处于选择状态
    if(pGraphEditorMgr->getSelectMode() == enumSelect)
    {
        //setItemCursor(mouseEvent);
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void HGraphEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() != Qt::LeftButton)
        return;
    if(!pGraphEditorMgr)
        return;
    bool multiSelect = (mouseEvent->modifiers() & Qt::ControlModifier) != 0;
    if(multiSelect)
    {
        if(!select)
        {
            pGraphEditorMgr->setSelectMode(enumDraw);
            pGraphEditorMgr->setDrawShape(enumSelection);
            newIconGraphicsObj();
        }
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
        refreshSelectedItemRect();
        pGraphEditorMgr->setSelectMode(enumSelect);
        pGraphEditorMgr->setDrawShape(enumNo);
        return;
    }
    DRAWSHAPE drawShape = pGraphEditorMgr->getDrawShape();
    if(drawShape == enumLine && line != 0)
    {
        line->setFlag(QGraphicsItem::ItemIsSelectable,true);
        emit itemInserted(line->type());
        line = 0;
    }
    else if(drawShape == enumRectangle && rectangle != 0)
    {
        rectangle->setFlag(QGraphicsItem::ItemIsSelectable,true);
        emit itemInserted(rectangle->type());
        rectangle = 0;
    }
    else if(drawShape == enumEllipse && ellipse != 0)
    {
        ellipse->setFlag(QGraphicsItem::ItemIsSelectable,true);
        emit itemInserted(ellipse->type());
        ellipse = 0;
    }
    else if(drawShape == enumCircle && circle != 0)
    {
        circle->setFlag(QGraphicsItem::ItemIsSelectable,true);
        emit itemInserted(circle->type());
        circle = 0;
    }
    else if(drawShape == enumArc && arc !=0)
    {
        arc->setFlag(QGraphicsItem::ItemIsSelectable,true);
        emit itemInserted(arc->type());
        arc = 0;
    }
    else if(drawShape == enumPie && pie !=0)
    {
        pie->setFlag(QGraphicsItem::ItemIsSelectable,true);
        emit itemInserted(pie->type());
        pie = 0;
    }
    else if(drawShape == enumText && text != 0)
    {
        text->setFlag(QGraphicsItem::ItemIsSelectable,true);
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
    else if(drawShape == enumSelection && select != 0)
    {
        QRectF rectF = select->rect();
        refreshSelectedItemRect(rectF,true);
    }
    pGraphEditorMgr->setDrawShape(enumNo);
    pGraphEditorMgr->setSelectMode(enumSelect);
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

/*
 * m_pIconMulSelectItemsList 说明:scene自带的selectedItems没有顺序
 * 为了对齐，大小等功能必须有一个带顺序的(有一个标杆，其他参考这个标杆)
 * 为了统一，其他比如组合解除组合，拷贝复制粘贴等其他功能都是用m_pIconMulSelectItemsList来实现
 * 缺点就是m_pIconMulSelectItemsList必须要自己来维护
*/
bool HGraphEditorScene::getItemAt(const QPointF &pos)
{
    //判断当前点位置是否有item被选择
    QTransform transform;
    QGraphicsItem* item = itemAt(pos,transform);
    if(item)//点选择到item,既是一般item也包含select项
    {
        //如果点落入select所在区域就刷新，否则删除select
        if(select){
            QRectF rectF = select->rect();
            if(rectF.contains(pos))
            {
                return true;
            }
            else
            {
                clearSeleteItem();
            }

        }
        addItemInScene((HIconGraphicsItem*)item);
        return true;
    }
    else
    {
        clearSeleteItem();
    }
    return false;
}

/*
 * 计算选择的Item，返回选择结果，包含无选择，单选，多选
*/
quint8 HGraphEditorScene::calcSelectedItem(const QRectF &rectF,bool bAreaSelect)
{
    quint8 nResult = SELECT_MODE_MULTIPLE;
    if(bAreaSelect)//区域选择为真，ctrl选择为假
    {
        QPainterPath path;
        path.addRect(rectF);
        QTransform transform;
        setSelectionArea(path,Qt::ContainsItemShape,transform);
    }

    //m_pIconMulSelectItemsList.clear();//主要是手动选择标杆的问题,和组合的问题
    foreach(HIconGraphicsItem *item, getSelectedItems())
    {
        HIconGraphicsItem* pItem = qgraphicsitem_cast<HIconGraphicsItem*>(item);
        if(m_pIconMulSelectItemsList.indexOf(pItem)>=0) continue;
        m_pIconMulSelectItemsList.append(pItem);
    }
    bool bMulSelected = true;
    if(m_pIconMulSelectItemsList.count() <= 1)
        bMulSelected = false;
    int nIndex = (int)-1;
    if(select)
        select->clear();
    foreach (HIconGraphicsItem *item,m_pIconMulSelectItemsList)
    {
        if(!bMulSelected)
        {
            item->bMulSelect = false;
            item->bBenchmark = false;
            nIndex++;
        }
        else
        {
            item->bMulSelect = true;
            nIndex++;
            if(nIndex == 0)
                item->bBenchmark = true;
            select->pObjList.append(item->getItemObj());
        }
    }

    //so low......
    if(nIndex == (int)-1)
    {
        nResult = SELECT_MODE_NO;
        emit selectItemChanged(SELECT_MODE_NO);
    }
    else if(nIndex == 0)
    {
        nResult = SELECT_MODE_SINGLE;
        emit selectItemChanged(SELECT_MODE_SINGLE);
    }
    else if(nIndex == 1)
        emit selectItemChanged(SELECT_MODE_2MULTIPLE);

    else
        emit selectItemChanged(SELECT_MODE_MULTIPLE);
    return nResult;
}

QRectF HGraphEditorScene::getMulSelectedItemsRect()
{
    QRectF rectF(0,0,0,0);
    foreach (HIconGraphicsItem *item, m_pIconMulSelectItemsList)
    {
        if(!item )
            continue;
        rectF = rectF.united(item->rect());
    }
    return rectF.adjusted(-5,-5,5,5);
}

//对scene之外统一接口,针对select及其选择对象进行刷新
void HGraphEditorScene::refreshSelectedItemRect(const QRectF &rectF,bool bAreaSelect)
{
    if(!select)
    {
        pGraphEditorMgr->setSelectMode(enumDraw);
        pGraphEditorMgr->setDrawShape(enumSelection);
        newIconGraphicsObj();
        pGraphEditorMgr->setSelectMode(enumSelect);
        pGraphEditorMgr->setDrawShape(enumNo);
    }
    quint8 bResult = calcSelectedItem(rectF,bAreaSelect);
    if(SELECT_MODE_NO == bResult)
    {
        clearSeleteItem();
        return;
    }
    else if(SELECT_MODE_SINGLE == bResult)
    {
        select->setRect(QRectF(0,0,0,0));
        select->setSelected(false);
    }
    else
    {
        QRectF rectF = getMulSelectedItemsRect();
        select->setRect(rectF);
        select->setFlag(QGraphicsItem::ItemIsSelectable,true);
        select->setSelected(true);
    }
}

/*
 * 参数的作用:如果重新操作新的选择区域，则必须要删除select，重新构建
 * 如果在复制粘贴等操作中，将select移到新的区域，此时不需删除select，只要将多选清除掉即可
 * 但在删除过程中，由于对象被删除，此时select已经没有存在的意义，所以释放
*/
void HGraphEditorScene::clearSeleteItem()
{
    if(select)
    {
        foreach (HIconGraphicsItem *item, m_pIconMulSelectItemsList)
        {
            if(!item) continue;
            item->bBenchmark = false;
            item->bMulSelect = false;
            item->setSelected(false);
        }  
        removeItem(select);
        delete select;
        select = 0;
    }
    m_pIconMulSelectItemsList.clear();
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
        polygon->setFlag(QGraphicsItem::ItemIsSelectable,true);
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
        polyline->setFlag(QGraphicsItem::ItemIsSelectable,true);
        emit itemInserted(polyline->type());
        polyline = 0;
        return;
    }
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

            //HBaseObj* pObj = complex->getItemObj();
            //HIconObj* pObj1 = (HIconObj*)complex->getItemObj();
            //pObj1->moveBy(dx,dy);
            complex->moveItemBy(dx,dy);
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
        line = new HIconLineItem(QLineF(prePoint,prePoint));
        line->setItemObj(pObj);
        line->getItemObj()->setModify(true);
        addItem(line);

    }
        break;
    case enumRectangle:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumRectangle);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HRectangle* pObj1 = (HRectangle*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        rectangle = new HIconRectItem(pObj1->getObjRect());
        rectangle->setItemObj(pObj);
        rectangle->getItemObj()->setModify(true);
        addItem(rectangle);
    }
        break;
    case enumEllipse:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumEllipse);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HEllipse* pObj1 = (HEllipse*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        ellipse = new HIconEllipseItem(pObj1->getObjRect());
        ellipse->setItemObj(pObj);
        ellipse->getItemObj()->setModify(true);
        addItem(ellipse);

    }
        break;
    case enumCircle:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumCircle);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HCircle* pObj1 = (HCircle*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        circle = new HIconCircleItem(pObj1->getObjRect());
        circle->setItemObj(pObj);
        circle->getItemObj()->setModify(true);
        addItem(circle);
    }
        break;
    case enumArc:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumArc);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HArc* pObj1 = (HArc*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        arc = new HIconArcItem(pObj1->getObjRect());
        arc->setItemObj(pObj);
        arc->getItemObj()->setModify(true);
        addItem(arc);
    }
        break;
    case enumPie:
    {
        HBaseObj *pObj = pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumPie);
        pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
        HPie* pObj1 = (HPie*)pObj;
        pObj1->setObjRect(QRectF(prePoint,prePoint).normalized());
        pie = new HIconPieItem(pObj1->getObjRect());
        pie->setItemObj(pObj);
        pie->getItemObj()->setModify(true);
        addItem(pie);

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
            polygon = new HIconPolygonItem(pObj1->pylist);
            polygon->setItemObj(pObj);
            polygon->getItemObj()->setModify(true);
            addItem(polygon);
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
            polyline = new HIconPolylineItem(pObj1->pylist);
            polyline->setItemObj(pObj);
            polyline->getItemObj()->setModify(true);
            addItem(polyline);
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
        text = new HIconTextItem(pObj1->getObjRect());
        text->setItemObj(pObj);
        text->getItemObj()->setModify(true);
        addItem(text);
        break;
    }
    case enumSelection:
    {
        QRectF tempF = QRectF(prePoint,prePoint).normalized();
        select = new HIconSelectionItem(tempF);
        select->setZValue(-0.5);
        addItem(select);
        break;
    }
    default:
        break;
    }
}

HIconGraphicsItem* HGraphEditorScene::addIconGraphicsItem(HBaseObj* pObj,bool bdel)
{
    DRAWSHAPE drawShape = pObj->getShapeType();
    HIconGraphicsItem* item = NULL;
    int nZValue = pObj->getStackOrder();
    if(drawShape == enumLine)
    {
        line = new HIconLineItem(QLineF(((HLine*) pObj)->getHeadPoint(),((HLine*)pObj)->getTailPoint()));
        line->setItemObj(pObj);
        line->setZValue(nZValue);
        line->setFlag(QGraphicsItem::ItemIsSelectable,true);
        addItem(line);

    }
    else if(drawShape == enumRectangle)
    {
        HRectangle* pObj1 = (HRectangle*)pObj;
        rectangle = new HIconRectItem(pObj1->getObjRect());
        rectangle->setItemObj(pObj);
        rectangle->setZValue(nZValue);
        rectangle->setFlag(QGraphicsItem::ItemIsSelectable,true);
        addItem(rectangle);
    }
    else if(drawShape == enumEllipse)
    {
        HEllipse* pObj1 = (HEllipse*)pObj;
        ellipse = new HIconEllipseItem(pObj1->getObjRect());
        ellipse->setItemObj(pObj);
        ellipse->setZValue(nZValue);
        ellipse->setFlag(QGraphicsItem::ItemIsSelectable,true);
        addItem(ellipse);
    }
    else if(drawShape == enumCircle)
    {
        HCircle* pObj1 = (HCircle*)pObj;
        circle = new HIconCircleItem(pObj1->getObjRect());
        circle->setItemObj(pObj);
        circle->setZValue(nZValue);
        circle->setFlag(QGraphicsItem::ItemIsSelectable,true);
        addItem(circle);
    }
    else if(drawShape == enumPolygon)
    {
        HPolygon* pObj1 = (HPolygon*)pObj;
        polygon = new HIconPolygonItem(pObj1->pylist);
        polygon->setItemObj(pObj);
        polygon->setZValue(nZValue);
        polygon->setFlag(QGraphicsItem::ItemIsSelectable,true);
        addItem(polygon);
    }
    else if(drawShape == enumPolyline)
    {
        HPolyline* pObj1 = (HPolyline*)pObj;
        polyline = new HIconPolylineItem(pObj1->pylist);
        polyline->setItemObj(pObj);
        polyline->setZValue(nZValue);
        polyline->setFlag(QGraphicsItem::ItemIsSelectable,true);
        addItem(polyline);
    }
    else if(drawShape == enumArc)
    {
        HArc* pObj1 = (HArc*)pObj;
        arc = new HIconArcItem(pObj1->getObjRect());
        arc->setItemObj(pObj);
        arc->setZValue(nZValue);
        arc->getItemObj()->setModify(true);
        arc->setFlag(QGraphicsItem::ItemIsSelectable,true);
        addItem(arc);
    }
    else if(drawShape == enumPie)
    {
        HPie* pObj1 = (HPie*)pObj;
        pie = new HIconPieItem(pObj1->getObjRect());
        pie->setItemObj(pObj);
        pie->setZValue(nZValue);
        pie->getItemObj()->setModify(true);
        pie->setFlag(QGraphicsItem::ItemIsSelectable,true);
        addItem(pie);
    }
    else if(drawShape == enumText)
    {
        HText* pObj1 = (HText*)pObj;
        text = new HIconTextItem(pObj1->getObjRect());
        text->setItemObj(pObj);
        text->setZValue(nZValue);
        text->setFlag(QGraphicsItem::ItemIsSelectable,true);
        addItem(text);
    }
    else if(drawShape == enumComplex)
    {
        HIconObj* pObj1 = (HIconObj*)pObj;
        complex = new HIconComplexItem(pObj1->getObjRect().normalized());
        complex->setItemObj(pObj);
        complex->setZValue(nZValue);
        complex->setFlag(QGraphicsItem::ItemIsSelectable,true);
        addItem(complex);
    }

    if(bdel)//等于false时，是让mousereleaseevent来释放
    {
        if(drawShape == enumLine && line != 0)
        {
            item = line;
            line = 0;
        }
        else if(drawShape == enumRectangle && rectangle != 0)
        {
            item = rectangle;
            rectangle = 0;
        }
        else if(drawShape == enumEllipse && ellipse != 0)
        {
            item = ellipse;
            ellipse = 0;
        }
        else if(drawShape == enumCircle && circle != 0)
        {
            item = circle;
            circle = 0;
        }
        else if(drawShape == enumPolyline && polyline != 0)
        {
            item = polyline;
            polyline = 0;
        }
        else if(drawShape == enumPolygon && polygon !=0)
        {
            item = polygon;
            polygon = 0;
        }
        else if(drawShape == enumArc && arc !=0)
        {
            item = arc;
            arc = 0;
        }
        else if(drawShape == enumPie && pie !=0)
        {
            item = pie;
            pie = 0;
        }
        else if(drawShape == enumText && text != 0)
        {
            item = text;
            text = 0;
        }
        else if(drawShape == enumComplex && complex != 0)
        {
            item = complex;
            complex = 0;
        }
    }
    return item;
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
    if(!pGraphEditorMgr)
        return;
    if(!pGraphEditorMgr->graphEditorDoc())
        return;
    foreach (QGraphicsItem *item, items())
    {
        HIconGraphicsItem* pItem = qgraphicsitem_cast<HIconGraphicsItem*>(item);
        if(!pItem) continue;
        //删除必须是以下顺序
        removeItemInScene(pItem);
        HBaseObj* pObj = pItem->getItemObj();
        if(pObj)
            pGraphEditorMgr->graphEditorDoc()->getCurGraph()->delObj(pObj);
        delete pItem;
        pItem = NULL;
    }
    m_pIconMulSelectItemsList.clear();
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

void HGraphEditorScene::removeItemInScene(HIconGraphicsItem* item)
{
    if(!item) return;
    removeItem(item);
    m_pIconMulSelectItemsList.removeOne(item);
}

void HGraphEditorScene::addItemInScene(HIconGraphicsItem *item)
{
    if(!item) return;
    m_pIconMulSelectItemsList.append(item);
}
