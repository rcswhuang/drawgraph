#include "hgrapheditorop.h"
#include <QDir>
#include <QGraphicsItem>
#include <QProcessEnvironment>
#include "hgrapheditormgr.h"
#include "hgrapheditorscene.h"
#include "hgrapheditordoc.h"
#include "hgrapheditorview.h"
#include "hicongraphicsitem.h"
#include "hiconlineitem.h"
#include "hiconitemgroup.h"
#include "hbaseobj.h"
#include "hiconobj.h"
#include "hrectangle.h"
#include "hcircle.h"
#include "hellipse.h"
#include "hline.h"
#include "hgraph.h"
HGraphEditorOp::HGraphEditorOp(HGraphEditorMgr* mgr)
    :m_pGraphEditorMgr(mgr)
{

}

void HGraphEditorOp::cut()
{
    copy();
    del();
}

void HGraphEditorOp::copy()
{
    //寻找当前页面的所有选择的图元
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;
    QByteArray bytes;
    QDataStream stream(&bytes,QIODevice::WriteOnly);
    QList<QGraphicsItem*> itemSelectList = m_pGraphEditorMgr->graphEditorScene()->selectedItems();
    stream<<itemSelectList.count();
    for(int i =0; i < itemSelectList.count();i++)
    {
        HBaseObj* pObj = qgraphicsitem_cast<HIconGraphicsItem*>(itemSelectList[i])->getItemObj();
        if(!pObj) continue;
        stream<<(quint8)pObj->getShapeType();
        HBaseObj* pNewObj = m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(pObj->getShapeType());
        if(!pNewObj) continue;
        pObj->clone(pNewObj);//需要clone吗？
        pNewObj->writeData(&stream);
        if(pNewObj)
        {
            delete pNewObj;
            pNewObj = NULL;
        }
    }
    QString clipboardPath = getClipboardFile();
    QFile file(clipboardPath);
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream cbStream(&file);
        cbStream.writeRawData(bytes.data(),bytes.length());
        file.close();
    }
}

void HGraphEditorOp::paste()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;
    if(!m_pGraphEditorMgr->graphEditorDoc()->getCurGraph())
        return;

    QString clipboardPath = getClipboardFile();
    QFile file(clipboardPath);
    if(!file.exists() || !file.open(QIODevice::ReadOnly))
        return;
    QList<HIconGraphicsItem*> copyItemList;
    QDataStream stream(&file);
    int num;
    stream>>num;
    quint8 nType;
    QList<HBaseObj*> objList;
    for(int i = 0; i < num;i++)
    {
        stream>>nType;
        HBaseObj *pObj = m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(nType);
        if(!pObj) continue;
        pObj->readData(&stream);
        objList.append(pObj);
        HIconGraphicsItem* item = m_pGraphEditorMgr->graphEditorScene()->addIconGraphicsItem(pObj,true);
        if(!item)
        {
            delete pObj;
            pObj = NULL;
            continue;
        }
        copyItemList.append(item);
        m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
     }

    //改变选择状态，只选择拷贝后的图元元素
    foreach(QGraphicsItem* item,m_pGraphEditorMgr->graphEditorScene()->items())
    {
        item->setSelected(false);
    }

    //有可能不是同一个画面拷贝，从A画面拷贝到B画面
    foreach(HIconGraphicsItem* item,copyItemList)
    {
        item->setSelected(true);
        QPointF pt(10,10);
        item->moveBy(pt.x(),pt.y());
        item->getItemObj()->moveBy(pt.x(),pt.y());
    }


    if(m_pGraphEditorMgr->graphEditorView())
        m_pGraphEditorMgr->graphEditorView()->refresh();
    //iconScene()->update(getLogicRect());
    //HPasteIconCommand* pasteIconCommand = new HPasteIconCommand(pIconMgr,objList);
    //pIconMgr->getIconUndoStack()->push(pasteIconCommand);
}

void HGraphEditorOp::del()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;
    QList<QGraphicsItem*> itemSelectList = m_pGraphEditorMgr->graphEditorScene()->selectedItems();
    QList<HBaseObj*> objList;
    foreach(QGraphicsItem* item,itemSelectList)
    {
        if(!item) continue;
        HBaseObj* pObj = ((HIconGraphicsItem*)item)->getItemObj();
        pObj->setDeleted(true);
        pObj->setVisible(false);
        item->setVisible(false);
        objList.append(pObj);
    }
    //HDelIconCommand *delIconCommand = new HDelIconCommand(pIconMgr,objList);
    //pIconMgr->getIconUndoStack()->push(delIconCommand);
}

QString HGraphEditorOp::getClipboardFile()
{
    QString clipboardPath = QProcessEnvironment::systemEnvironment().value("wfsystem_dir");
    clipboardPath.append("/temp/grapheditor");
    QDir dirClipboard(clipboardPath);
    if(!dirClipboard.exists())
        dirClipboard.mkdir(clipboardPath);
    clipboardPath.append("/graphclipboard.data");
    return clipboardPath;
}

//移动到顶层
void HGraphEditorOp::bringToTop()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;

    int maxZValue = 0;
    QList<QGraphicsItem*> itemList = m_pGraphEditorMgr->graphEditorScene()->selectedItems();
    if(itemList.count() > 1) return;
    QGraphicsItem* pItem = itemList.at(0);
    QList<QGraphicsItem*> collItemList = pItem->collidingItems();
    if(collItemList.count()<=0) return;
    maxZValue = collItemList.at(0)->zValue();
    for(int i = 1; i < collItemList.count();i++)
    {
        QGraphicsItem* item = collItemList[i];
        if(item->zValue() > maxZValue)
            maxZValue = item->zValue();
    }
    if(pItem->zValue() > maxZValue)
        return;
    else
    {
        maxZValue++;
        pItem->setZValue(maxZValue);
        ((HIconGraphicsItem*)pItem)->getItemObj()->setStackOrder(maxZValue);
    }
}

//移动到底层
void HGraphEditorOp::bringToBottom()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;
    int minZValue = 0;
    QList<QGraphicsItem*> itemList = m_pGraphEditorMgr->graphEditorScene()->selectedItems();
    if(itemList.count() > 1) return;
    QGraphicsItem* pItem = itemList.at(0);
    QList<QGraphicsItem*> collItemList = pItem->collidingItems();
    if(collItemList.count()<=0) return;
    minZValue = (int)(collItemList.at(0)->zValue());
    for(int i = 1; i < collItemList.count();i++)
    {
        QGraphicsItem* item = collItemList[i];
        if(item->zValue() < minZValue)
            minZValue = item->zValue();
    }
    if(pItem->zValue() < minZValue)
        return;
    else
    {
        minZValue--;
        pItem->setZValue(minZValue);
        ((HIconGraphicsItem*)pItem)->getItemObj()->setStackOrder(minZValue);
    }
}

//组合
void HGraphEditorOp::groupObj()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;
    QList<QGraphicsItem*> items = m_pGraphEditorMgr->graphEditorScene()->selectedItems();
    if(items.count() < 2) return;

    HBaseObj* pGroupObj = m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumGroup);//pIconMgr->getIconTemplate()->getSymbol()->newObj(enumGroup);
    QRectF groupRect;
    for(int i = 0; i < items.count();i++)
    {
        HIconGraphicsItem* item = (HIconGraphicsItem*)items.at(i);
        HBaseObj* pObj = item->getItemObj();
        groupRect = groupRect.united(item->rect());
        m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->removeObj(pObj);//应该是take操作 不是删除
        ((HGroupObj*)pGroupObj)->addObj(pObj);
    }
    //矩形
    HIconItemGroup *itemGroup = new HIconItemGroup(QRectF(0,0,groupRect.width(),groupRect.height()));
    m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pGroupObj);
    itemGroup->setItemObj(pGroupObj);
    itemGroup->setRect(groupRect);
    foreach(QGraphicsItem* item,items)
    {
        m_pGraphEditorMgr->graphEditorScene()->removeItem(item);
        HBaseObj* pObj = ((HIconGraphicsItem*)item)->getItemObj();
        pObj->setIconGraphicsItem(NULL);
    }
    m_pGraphEditorMgr->graphEditorScene()->addItem(itemGroup);
    m_pGraphEditorMgr->setDrawShape(enumNo);
    m_pGraphEditorMgr->setSelectMode(enumSelect);
}

//取消组合
void HGraphEditorOp::ungroupObj()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;
    QList<QGraphicsItem*> items = m_pGraphEditorMgr->graphEditorScene()->selectedItems();
    for(int i = 0; i < items.count();i++)
    {
        HIconGraphicsItem* item = (HIconGraphicsItem*)items.at(i);
        if(item->type() != enumGroup) continue;
        HBaseObj* pObj = item->getItemObj();
        HGroupObj* pGroupObj = (HGroupObj*)pObj;
        while(!pGroupObj->isEmpty())
        {
            HBaseObj* pObj = (HBaseObj*)pGroupObj->takeFirst();
            m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pObj);
            m_pGraphEditorMgr->graphEditorScene()->addIconGraphicsItem(pObj,true);
        }
        m_pGraphEditorMgr->graphEditorScene()->removeItem(item);
        m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->delObj(pObj);
    }
    m_pGraphEditorMgr->setDrawShape(enumNo);
    m_pGraphEditorMgr->setSelectMode(enumSelect);
}

//对齐
bool HGraphEditorOp::getbenchMarkPoint(QPointF& pt)
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return false;
    HGraphEditorScene* graphScene = m_pGraphEditorMgr->graphEditorScene();
    //找到标杆
    QPointF benchMarkPt = QPoint(0,0);
    bool bok = false;
    for(int i = 0; i < graphScene->m_pIconSelectItems.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconSelectItems[i];
        if(iconItem && iconItem->bBenchmark && iconItem->bMulSelect)
        {
            HBaseObj* pObj = iconItem->getItemObj();
            ushort shapeType = pObj->getShapeType();
            if(shapeType == enumRectangle || shapeType == enumEllipse || shapeType == enumCircle || shapeType == enumPie || shapeType == enumText
                    || shapeType == enumArc || shapeType == enumPolygon || shapeType == enumPolyline || shapeType == enumComplex)
            {
                if(m_Alignment == Qt::AlignLeft || m_Alignment == Qt::AlignTop)
                {
                    benchMarkPt = iconItem->rect().topLeft();
                    bok = true;
                }
                else if(m_Alignment == Qt::AlignRight)
                {
                    benchMarkPt = iconItem->rect().topRight();
                    bok = true;
                }
                else if(m_Alignment == Qt::AlignVCenter || m_Alignment == Qt::AlignHCenter)
                {
                    benchMarkPt = iconItem->rect().center();
                    bok = true;
                }
                else if(m_Alignment == Qt::AlignBottom)
                {
                    benchMarkPt = iconItem->rect().bottomLeft();//bottomRight也行
                    bok = true;
                }
                else
                    bok = false;
            }
            else if(shapeType == enumLine)// || shapeType == enumPolyline)
            {
                HIconLineItem* lineItem = (HIconLineItem*)iconItem;
                QPointF pt1(((HLine*)lineItem->getItemObj())->getHeadPoint());
                QPointF pt2(((HLine*)lineItem->getItemObj())->getTailPoint());
                if(m_Alignment == Qt::AlignLeft)
                {
                    if(pt1.x() < pt2.x())
                        benchMarkPt = pt1;
                    else
                        benchMarkPt = pt2;
                    bok = true;
                }
                else if(m_Alignment == Qt::AlignRight)
                {
                    if(pt1.x() < pt2.x())
                        benchMarkPt = pt2;
                    else
                        benchMarkPt = pt1;
                    bok = true;
                }
                else if(m_Alignment == Qt::AlignVCenter || m_Alignment == Qt::AlignHCenter)
                {
                    benchMarkPt = (pt1 + pt2)/2;
                    bok = true;
                }
                else if(m_Alignment == Qt::AlignTop)
                {
                    if(pt1.y() < pt2.y())
                        benchMarkPt = pt1;
                    else
                        benchMarkPt = pt2;
                    bok = true;
                }
                else if(m_Alignment == Qt::AlignBottom)
                {
                    if(pt1.y() < pt2.y())
                        benchMarkPt = pt2;
                    else
                        benchMarkPt = pt1;
                    bok = true;
                }
                else
                    bok = false;
            }
            else
                bok = false;
        }
    }
    pt = benchMarkPt;
    return bok;
}

//对齐算法
void HGraphEditorOp::alignAlgorithm()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    QPointF benchMarkPt;
    if(!getbenchMarkPoint(benchMarkPt))
        return;
    HGraphEditorScene* graphScene = m_pGraphEditorMgr->graphEditorScene();
    for(int i = 0; i < graphScene->m_pIconSelectItems.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconSelectItems[i];
        if(iconItem && !iconItem->bBenchmark && iconItem->bMulSelect)
        {
            HBaseObj* pObj = iconItem->getItemObj();
            ushort shapeType = pObj->getShapeType();
            if(shapeType == enumRectangle || shapeType == enumEllipse || shapeType == enumCircle || shapeType == enumPie || shapeType == enumText
                    || shapeType == enumArc || shapeType == enumPolygon || shapeType == enumPolyline || shapeType == enumComplex)
            {
                QRectF iconRect = iconItem->rect();
                qreal dx = 0,dy = 0;
                if(m_Alignment == Qt::AlignLeft)
                {
                    dx = benchMarkPt.x() - iconRect.topLeft().x();
                    dy = 0;
                }
                else if(m_Alignment == Qt::AlignRight)
                {
                    dx = benchMarkPt.x() - iconRect.topRight().x();
                    dy = 0;
                }
                else if(m_Alignment == Qt::AlignVCenter)
                {
                    dx = benchMarkPt.x() - iconRect.center().x();
                    dy = 0;
                }
                else if(m_Alignment == Qt::AlignTop)
                {
                    dy = benchMarkPt.y() - iconRect.topLeft().y();
                    dx = 0;
                }
                else if(m_Alignment == Qt::AlignBottom)
                {
                    dy = benchMarkPt.y() - iconRect.bottomLeft().y();
                    dx = 0;
                }
                else if(m_Alignment == Qt::AlignHCenter)
                {
                    dy = benchMarkPt.y() - iconRect.center().y();
                    dx = 0;
                }
                QRectF newRect = iconRect.translated(dx,dy);
                iconItem->setRect(newRect);
            }
            else if(shapeType == enumLine)
            {
                HIconLineItem* lineItem = (HIconLineItem*)iconItem;
                QPointF pt1(((HLine*)lineItem->getItemObj())->getHeadPoint());
                QPointF pt2(((HLine*)lineItem->getItemObj())->getTailPoint());
                qreal dx = 0,dy = 0;
                if(m_Alignment == Qt::AlignLeft)
                {
                    dx = benchMarkPt.x() - pt1.x();
                    dy = 0;
                    if(pt1.x() > pt2.x())
                    {
                        dx = benchMarkPt.x() - pt2.x();
                        dy = 0;
                    }
                }
                else if(m_Alignment == Qt::AlignRight)
                {
                    dx = benchMarkPt.x() - pt2.x();
                    dy = 0;
                    if(pt1.x() > pt2.x())
                    {
                        dx = benchMarkPt.x() - pt1.x();
                        dy = 0;
                    }
                }
                else if(m_Alignment == Qt::AlignVCenter)
                {
                    QPointF ptC =  (pt1 + pt2)/2;
                    dx = benchMarkPt.x() - ptC.x();
                    dy = 0;
                }
                else if( m_Alignment == Qt::AlignHCenter)
                {
                    QPointF ptC =  (pt1 + pt2)/2;
                    dy = benchMarkPt.y() - ptC.y();
                    dx = 0;
                }
                else if(m_Alignment == Qt::AlignTop)
                {
                    dx = 0;
                    dy = benchMarkPt.y() - pt1.y();
                    if(pt1.y() > pt2.y())
                    {
                        dx = 0;
                        dy = benchMarkPt.y() - pt2.y();
                    }
                }
                else if(m_Alignment == Qt::AlignBottom)
                {
                    dx = 0;
                    dy = benchMarkPt.y() - pt2.y();
                    if(pt1.y() > pt2.y())
                    {
                        dx = 0;
                        dy = benchMarkPt.y() - pt1.y();
                    }
                }
                QLineF newLineF = lineItem->line().translated(dx,dy);
                lineItem->setLine(newLineF);
            }
        }
    }

    //还要刷新一下selection 才可以
    m_pGraphEditorMgr->graphEditorScene()->refreshSelectedItemRect();
}

//左对齐
void HGraphEditorOp::alignLeft()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Alignment = Qt::AlignLeft;
    alignAlgorithm();

}

//右对齐
void HGraphEditorOp::alignRight()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Alignment = Qt::AlignRight;
    alignAlgorithm();
}

//垂直居中
void HGraphEditorOp::alignVCenter()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Alignment = Qt::AlignVCenter;
    alignAlgorithm();
}

//上对齐
void HGraphEditorOp::alignTop()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Alignment = Qt::AlignTop;
    alignAlgorithm();
}

//下对齐
void HGraphEditorOp::alignBottom()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Alignment = Qt::AlignBottom;
    alignAlgorithm();
}

//水平居中
void HGraphEditorOp::alignHCenter()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Alignment = Qt::AlignHCenter;
    alignAlgorithm();
}
