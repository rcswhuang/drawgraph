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
#include "hiconselectionitem.h"
#include "hiconitemgroup.h"
#include "hbaseobj.h"
#include "hiconobj.h"
#include "hrectangle.h"
#include "hcircle.h"
#include "hellipse.h"
#include "hline.h"
#include "hgraph.h"
#include "hgroupobj.h"
HGraphEditorOp::HGraphEditorOp(HGraphEditorMgr* mgr)
    :m_pGraphEditorMgr(mgr)
{
    m_scale = 1.0;
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
    QList<HIconGraphicsItem*> itemSelectList = m_pGraphEditorMgr->graphEditorScene()->getSelectedItems();
    stream<<itemSelectList.count();
    quint8 btType = 0;
    QString s = "";
    for(int i =0; i < itemSelectList.count();i++)
    {
        HBaseObj* pObj = qgraphicsitem_cast<HIconGraphicsItem*>(itemSelectList[i])->getItemObj();
        if(!pObj) continue;
        btType = (quint8)pObj->getShapeType();
        stream<<btType;
        if(enumComplex == btType)
        {
            s = ((HIconObj*)pObj)->getUuid();
            stream<<s;
        }
        HBaseObj* pNewObj = m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(pObj->getShapeType(),s);
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
    QString s;
    QList<HBaseObj*> objList;
    for(int i = 0; i < num;i++)
    {
        stream>>nType;
        if(enumComplex == nType)
        {
            stream>>s;
        }
        HBaseObj *pObj = m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(nType,s);
        if(!pObj) continue;
        pObj->readData(&stream);
        if(enumComplex == nType)
        {
            HIconObj* pObj1 = (HIconObj*)pObj;
            QSizeF pt = pObj1->iconTemplate()->getDefaultSize();
            double w1 = pObj1->getRectWidth()/(pt.width()*20);
            double h1 = pObj1->getRectHeight()/(pt.height()*20);
            pObj1->resetRectPoint(pObj1->getTopLeft(),QPointF(-pt.width()*10,-pt.height()*10));
            pObj1->resize(w1,h1);
        }
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
    m_pGraphEditorMgr->graphEditorScene()->clearSelectItem();

    foreach(HIconGraphicsItem* item,copyItemList)//有可能不是同一个画面拷贝，从A画面拷贝到B画面 ---huangw
    {
        item->setSelected(true);
        QPointF pt(10,10);
        item->moveItemBy(pt.x(),pt.y());
         m_pGraphEditorMgr->graphEditorScene()->addItemInScene(item);
    } 
    m_pGraphEditorMgr->graphEditorScene()->refreshSelectedItemRect();
    m_pGraphEditorMgr->addPasteCommand(objList);
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
        //选择框要删除
        if(((HIconGraphicsItem*)item)->type() == enumSelection)
        {
            continue;
        }
        HBaseObj* pObj = ((HIconGraphicsItem*)item)->getItemObj();
        pObj->setDeleted(true);
        pObj->setVisible(false);
        item->setVisible(false);
        objList.append(pObj);
    }
    m_pGraphEditorMgr->addDelCommand(objList);
    m_pGraphEditorMgr->graphEditorScene()->clearSelectItem();//必须要清除
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
    QList<HIconGraphicsItem*> itemList = m_pGraphEditorMgr->graphEditorScene()->getSelectedItems();
    if(itemList.count() > 1) return;
    HIconGraphicsItem* pItem = itemList.at(0);
    QList<QGraphicsItem*> collItemList = pItem->collidingItems();
    if(collItemList.count()<=0) return;
    maxZValue = collItemList.at(0)->zValue();
    for(int i = 1; i < collItemList.count();i++)
    {
        HIconGraphicsItem* item = (HIconGraphicsItem*)collItemList[i];
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
    QList<HIconGraphicsItem*> itemList = m_pGraphEditorMgr->graphEditorScene()->getSelectedItems();
    if(itemList.count() > 1) return;
    HIconGraphicsItem* pItem = itemList.at(0);
    QList<QGraphicsItem*> collItemList = pItem->collidingItems();
    if(collItemList.count()<=0) return;
    minZValue = (int)(collItemList.at(0)->zValue());
    for(int i = 1; i < collItemList.count();i++)
    {
        HIconGraphicsItem* item = (HIconGraphicsItem*)collItemList[i];
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
    QList<HIconGraphicsItem*> items = m_pGraphEditorMgr->graphEditorScene()->getSelectedItems();
    if(items.count() < 2) return;

    //1.增加一个group
    HBaseObj* pGroupObj = m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->newObj(enumGroup);//pIconMgr->getIconTemplate()->getSymbol()->newObj(enumGroup);
    QRectF groupRect;
    for(int i = 0; i < items.count();i++)
    {
        HIconGraphicsItem* item = (HIconGraphicsItem*)items.at(i);
        HBaseObj* pObj = item->getItemObj();
        groupRect = groupRect.united(item->rect());
        if(item->type() == enumGroup)
        {
            HGroupObj* pDelGroup = (HGroupObj*)pObj;
            items.removeOne(item);//注意删除后i--
            i--;
            m_pGraphEditorMgr->graphEditorScene()->removeItemInScene(item);//从scene中删除
            while(!pDelGroup->isEmpty())//加到group里面
            {
                HBaseObj* pObj = (HBaseObj*)pDelGroup->takeFirst();
                ((HGroupObj*)pGroupObj)->addObj(pObj);
            }
            m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->removeObj(pDelGroup);//graph删除
            delete pDelGroup;
            pDelGroup = NULL;
        }
        else
        {
            m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->removeObj(pObj);//移除
            ((HGroupObj*)pGroupObj)->addObj(pObj);
        }
    }
    //2.增加groupItem
    ((HGroupObj*)pGroupObj)->setObjRect(QRectF(0,0,groupRect.width(),groupRect.height()));
    HIconItemGroup *itemGroup = new HIconItemGroup(pGroupObj);
    itemGroup->setFlag(QGraphicsItem::ItemIsSelectable,true);
    m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->addObj(pGroupObj);
    itemGroup->setItemObj(pGroupObj);
    itemGroup->setRect(groupRect);
    foreach(HIconGraphicsItem* item,items)
    {
        m_pGraphEditorMgr->graphEditorScene()->removeItemInScene(item);
        HBaseObj* pObj = ((HIconGraphicsItem*)item)->getItemObj();
        if(pObj)
            pObj->setIconGraphicsItem(NULL);
        delete item;
        item = NULL;
    }
    m_pGraphEditorMgr->graphEditorScene()->addItem(itemGroup);
    itemGroup->setSelected(true);
    m_pGraphEditorMgr->graphEditorScene()->refreshSelectedItemRect();
    m_pGraphEditorMgr->setDrawShape(enumNo);
    m_pGraphEditorMgr->setSelectMode(enumSelect);
}

//取消组合
void HGraphEditorOp::ungroupObj()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;
    QList<HIconGraphicsItem*> items = m_pGraphEditorMgr->graphEditorScene()->getSelectedItems();
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
        m_pGraphEditorMgr->graphEditorScene()->removeItemInScene(item);
        if(pGroupObj)
            m_pGraphEditorMgr->graphEditorDoc()->getCurGraph()->delObj(pGroupObj);
        delete item;
        item = NULL;
    }

    m_pGraphEditorMgr->graphEditorScene()->refreshSelectedItemRect();
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
    for(int i = 0; i < graphScene->m_pIconMulSelectItemsList.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconMulSelectItemsList[i];
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
    for(int i = 0; i < graphScene->m_pIconMulSelectItemsList.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconMulSelectItemsList[i];
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


//等宽
void HGraphEditorOp::sizeEqualWidth()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Equalway = IconSize::EqualWidth;
    equalAlgorithm();
}

//等高
void HGraphEditorOp::sizeEqualHeight()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Equalway = IconSize::EqualHeight;
    equalAlgorithm();
}

//完全相等
void HGraphEditorOp::sizeEqualComplete()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Equalway = IconSize::EqualComplete;
    equalAlgorithm();
}

bool caseInsensitiveLessThan(const HIconGraphicsItem* s1, const HIconGraphicsItem* s2)
{
    return s1->rect().x() < s2->rect().x();
}

//横向等间距
void HGraphEditorOp::sizeHEqualSpace()
{
    //1.先求出等分的距离
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;
    HGraphEditorScene* graphScene = m_pGraphEditorMgr->graphEditorScene();
    QPointF benchMarkPt = QPoint(0,0);
    qreal fSelectlength = 0.0,fIconlength = 0.0;
    QRectF rectUnit;
    int nCount = 0;
    for(int i = 0; i < graphScene->m_pIconMulSelectItemsList.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconMulSelectItemsList[i];
        if(iconItem && iconItem->bBenchmark && iconItem->bMulSelect)
        {
            benchMarkPt = iconItem->rect().topLeft();
        }
        rectUnit = rectUnit.united(iconItem->rect());
        fIconlength += iconItem->rect().width();
        nCount++;
    }
    fSelectlength = rectUnit.width();
    if(fSelectlength < fIconlength)
         return;
    //2.计算出item新的位置
    qreal equalX = (fSelectlength - fIconlength)/(nCount-1);
    qSort(graphScene->m_pIconMulSelectItemsList.begin(),graphScene->m_pIconMulSelectItemsList.end(),caseInsensitiveLessThan);
    HIconGraphicsItem *iconFirstItem = graphScene->m_pIconMulSelectItemsList.first();
    fIconlength = 0;
    fIconlength += iconFirstItem->rect().width();
    for(int i = 1; i < graphScene->m_pIconMulSelectItemsList.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconMulSelectItemsList[i];
        if(!iconItem) continue;
        HBaseObj* pObj = iconItem->getItemObj();
        ushort shapeType = pObj->getShapeType();
        if(shapeType == enumRectangle || shapeType == enumEllipse || shapeType == enumCircle || shapeType == enumPie || shapeType == enumText
                || shapeType == enumArc || shapeType == enumPolygon || shapeType == enumPolyline || shapeType == enumComplex)
        {
            QRectF iconRect = iconItem->rect();
            QRectF newRect = iconRect;
            newRect.setLeft(iconFirstItem->rect().left() + fIconlength + i*equalX);
            newRect.setWidth(iconRect.width());
            fIconlength += iconRect.width();
            iconItem->setRect(newRect);
        }
        else if(shapeType == enumLine)
        {
            HIconLineItem* lineItem = (HIconLineItem*)iconItem;
            QPointF pt1(((HLine*)lineItem->getItemObj())->getHeadPoint());
            QPointF pt2(((HLine*)lineItem->getItemObj())->getTailPoint());
            QPointF pt11,pt22;
            if(pt1.x() < pt2.x())
            {
                pt11.setX(iconFirstItem->rect().left() + fIconlength + i*equalX);
                pt11.setY(pt1.y());
                pt22.setX(pt11.x() + (pt2.x() - pt1.x()));
                pt22.setY(pt2.y());
            }
            else
            {
                pt22.setX(iconFirstItem->rect().left() + fIconlength + i*equalX);
                pt22.setY(pt2.y());
                pt11.setX(pt22.x() + (pt1.x() - pt2.x()));
                pt11.setY(pt1.y());
            }
            fIconlength += lineItem->rect().width();
            QLineF newLineF = QLineF(pt11,pt22);
            lineItem->setLine(newLineF);
        }
    }

    m_pGraphEditorMgr->graphEditorScene()->refreshSelectedItemRect();
}

//纵向等间距
void HGraphEditorOp::sizeVEqualSpace()
{
    //1.先求出等分的距离
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;
    HGraphEditorScene* graphScene = m_pGraphEditorMgr->graphEditorScene();
    qreal fSelectlength = 0.0,fIconlength = 0.0;
    QRectF rectUnit;
    int nCount = 0;
    for(int i = 0; i < graphScene->m_pIconMulSelectItemsList.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconMulSelectItemsList[i];
        rectUnit = rectUnit.united(iconItem->rect());
        fIconlength += iconItem->rect().height();
        nCount++;
    }
    fSelectlength = rectUnit.height();
    if(fSelectlength < fIconlength)
         return;
    //2.计算出item新的位置
    qreal equalY = (fSelectlength - fIconlength)/(nCount-1);
    qSort(graphScene->m_pIconMulSelectItemsList.begin(),graphScene->m_pIconMulSelectItemsList.end(),caseInsensitiveLessThan);
    HIconGraphicsItem *iconFirstItem = graphScene->m_pIconMulSelectItemsList.first();
    fIconlength = 0;
    fIconlength += iconFirstItem->rect().height();
    for(int i = 1; i < graphScene->m_pIconMulSelectItemsList.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconMulSelectItemsList[i];
        if(!iconItem) continue;
        HBaseObj* pObj = iconItem->getItemObj();
        ushort shapeType = pObj->getShapeType();
        if(shapeType == enumRectangle || shapeType == enumEllipse || shapeType == enumCircle || shapeType == enumPie || shapeType == enumText
                || shapeType == enumArc || shapeType == enumPolygon || shapeType == enumPolyline || shapeType == enumComplex)
        {
            QRectF iconRect = iconItem->rect();
            QRectF newRect = iconRect;
            newRect.setTop(iconFirstItem->rect().top() + fIconlength + i*equalY);
            newRect.setHeight(iconRect.height());
            fIconlength += iconRect.height();
            iconItem->setRect(newRect);
        }
        else if(shapeType == enumLine)
        {
            HIconLineItem* lineItem = (HIconLineItem*)iconItem;
            QPointF pt1(((HLine*)lineItem->getItemObj())->getHeadPoint());
            QPointF pt2(((HLine*)lineItem->getItemObj())->getTailPoint());
            QPointF pt11,pt22;
            if(pt1.y() < pt2.y())
            {
                pt11.setY(iconFirstItem->rect().top() + fIconlength + i*equalY);
                pt11.setX(pt1.x());
                pt22.setY(pt11.y() + (pt2.y() - pt1.y()));
                pt22.setX(pt2.x());
            }
            else
            {
                pt22.setY(iconFirstItem->rect().top() + fIconlength + i*equalY);
                pt22.setX(pt2.x());
                pt11.setY(pt22.y() + (pt1.y() - pt2.y()));
                pt11.setX(pt1.x());
            }
            fIconlength += lineItem->rect().width();
            QLineF newLineF = QLineF(pt11,pt22);
            lineItem->setLine(newLineF);
        }
    }

}

bool HGraphEditorOp::getbenchMarkSize(QPointF& pt)
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return false;
    HGraphEditorScene* graphScene = m_pGraphEditorMgr->graphEditorScene();
    QPointF benchMarkPt = QPoint(0,0);
    bool bok = false;
    for(int i = 0; i < graphScene->m_pIconMulSelectItemsList.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconMulSelectItemsList[i];
        if(iconItem && iconItem->bBenchmark && iconItem->bMulSelect)
        {
            HBaseObj* pObj = iconItem->getItemObj();
            ushort shapeType = pObj->getShapeType();
            if(shapeType == enumRectangle || shapeType == enumEllipse || shapeType == enumCircle || shapeType == enumPie || shapeType == enumText
                    || shapeType == enumArc || shapeType == enumPolygon || shapeType == enumPolyline || shapeType == enumComplex)
            {
                QRectF iconRect = iconItem->rect();
               if(m_Equalway == IconSize::EqualWidth || m_Equalway == IconSize::EqualHeight || m_Equalway == IconSize::EqualComplete)
               {
                   benchMarkPt.setX(iconRect.width());
                   benchMarkPt.setY(iconRect.height());
                   bok = true;
               }

            }
            else if(shapeType == enumLine)// || shapeType == enumPolyline)
            {
                HIconLineItem* lineItem = (HIconLineItem*)iconItem;
                QPointF pt1(((HLine*)lineItem->getItemObj())->getHeadPoint());
                QPointF pt2(((HLine*)lineItem->getItemObj())->getTailPoint());
                if(m_Equalway == IconSize::EqualWidth || m_Equalway == IconSize::EqualHeight || m_Equalway == IconSize::EqualComplete)
                {
                    benchMarkPt.setX(qAbs(pt1.x() - pt2.x()));
                    benchMarkPt.setY(qAbs(pt1.y() - pt2.y()));
                    bok = true;
                }
            }
            else
                bok = false;
        }
    }
    pt = benchMarkPt;
    return bok;
}

void HGraphEditorOp::equalAlgorithm()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    QPointF benchMarkPt;
    if(!getbenchMarkSize(benchMarkPt))
        return;
    HGraphEditorScene* graphScene = m_pGraphEditorMgr->graphEditorScene();
    for(int i = 0; i < graphScene->m_pIconMulSelectItemsList.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconMulSelectItemsList[i];
        if(iconItem && !iconItem->bBenchmark && iconItem->bMulSelect)
        {
            HBaseObj* pObj = iconItem->getItemObj();
            ushort shapeType = pObj->getShapeType();
            if(shapeType == enumRectangle || shapeType == enumEllipse || shapeType == enumCircle || shapeType == enumPie || shapeType == enumText
                    || shapeType == enumArc || shapeType == enumPolygon || shapeType == enumPolyline || shapeType == enumComplex)
            {
                QRectF iconRect = iconItem->rect();
                QRectF newRect = iconRect;
                qreal dx = 0,dy = 0;
                if(m_Equalway == IconSize::EqualWidth)
                {
                    newRect.setWidth(benchMarkPt.x());
                    dx = (iconRect.width() - benchMarkPt.x())/2;
                }
                else if(m_Equalway == IconSize::EqualHeight)
                {
                    newRect.setHeight(benchMarkPt.y());
                    dy = (iconRect.height() - benchMarkPt.y())/2;
                }
                else if(m_Equalway == IconSize::EqualComplete)
                {
                    newRect.setWidth(benchMarkPt.x());
                    newRect.setHeight(benchMarkPt.y());
                    dx = (iconRect.width() - benchMarkPt.x())/2;
                    dy = (iconRect.height() - benchMarkPt.y())/2;
                }
                QRectF rectNew = newRect.translated(dx,dy);
                iconItem->setRect(rectNew);
            }
            else if(shapeType == enumLine)
            {
                HIconLineItem* lineItem = (HIconLineItem*)iconItem;
                QPointF pt1(((HLine*)lineItem->getItemObj())->getHeadPoint());
                QPointF pt2(((HLine*)lineItem->getItemObj())->getTailPoint());
                qreal dx = 0,dy = 0;
                if(m_Equalway == IconSize::EqualWidth || m_Equalway == IconSize::EqualComplete)
                {
                    if(pt1.x() < pt2.x())
                    {
                        dx = pt1.x() + benchMarkPt.x();//new width
                        pt2.setX(dx);
                    }
                    else if(pt1.x() > pt2.x())
                    {
                        dx = pt2.x() + benchMarkPt.x();
                        pt1.setX(dx);

                    }
                }

                if(m_Equalway == IconSize::EqualHeight || m_Equalway == IconSize::EqualComplete)
                {
                    if(pt1.y() < pt2.y())
                    {
                        dy = pt1.y() + benchMarkPt.y();//new width
                        pt2.setY(dy);
                    }
                    else if(pt1.y() > pt2.y())
                    {
                        dy = pt2.y() + benchMarkPt.y();
                        pt1.setY(dy);
                    }
                }
                QLineF newLineF = QLineF(pt1,pt2);
                lineItem->setLine(newLineF);
            }
        }
    }
    m_pGraphEditorMgr->graphEditorScene()->refreshSelectedItemRect();
}

void HGraphEditorOp::flipAlgorithm()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene() || !m_pGraphEditorMgr->graphEditorDoc())
        return;
    HGraphEditorScene* graphScene = m_pGraphEditorMgr->graphEditorScene();
    for(int i = 0; i < graphScene->m_pIconMulSelectItemsList.count();i++)
    {
        HIconGraphicsItem *iconItem = (HIconGraphicsItem*)graphScene->m_pIconMulSelectItemsList[i];
        if(!iconItem) continue;
        HBaseObj* pObj = iconItem->getItemObj();
        if(!pObj) continue;
        float fRotate = pObj->getRotateAngle();
        if(IconFlip::LeftFlip90 == m_Flipway)
        {
            fRotate -= 90;
            pObj->setRotateAngle(fRotate);
        }
        else if(IconFlip::RightFlip90 == m_Flipway)
        {
            fRotate += 90;
            pObj->setRotateAngle(fRotate);
        }
        else if(IconFlip::HorizonFlip == m_Flipway)
        {
            pObj->setTurn(true,false);
        }
        else if(IconFlip::VerticalFlip == m_Flipway)
        {
            pObj->setTurn(false,true);
        }

    }
    m_pGraphEditorMgr->graphEditorScene()->refreshSelectedItemRect();
}

void HGraphEditorOp::flipLeft90()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Flipway = IconFlip::LeftFlip90;
    flipAlgorithm();

}

void HGraphEditorOp::flipRight90()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Flipway = IconFlip::RightFlip90;
    flipAlgorithm();
}

void HGraphEditorOp::flipHorizon()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Flipway = IconFlip::HorizonFlip;
    flipAlgorithm();
}

void HGraphEditorOp::flipVertical()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorScene())
        return;
    m_Flipway = IconFlip::VerticalFlip;
    flipAlgorithm();
}

void HGraphEditorOp::setupMatrix()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorView())
        return;
    QMatrix oldMatrix = m_pGraphEditorMgr->graphEditorView()->matrix();
    m_pGraphEditorMgr->graphEditorView()->resetMatrix();
    m_pGraphEditorMgr->graphEditorView()->translate(oldMatrix.dx(), oldMatrix.dy());
    m_pGraphEditorMgr->graphEditorView()->scale(m_scale, m_scale);
}

//放大
void HGraphEditorOp::zoomIn()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorView())
        return;
    m_scale += (qreal)0.2;
    if(m_scale > (qreal)2.0) m_scale = (qreal)2.0;
    if(m_scale < (qreal)0.5) m_scale = (qreal)0.5;
    setupMatrix();
}

//缩小
void HGraphEditorOp::zoomOut()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorView())
        return;
    m_scale -= (qreal)0.1;
    if(m_scale > (qreal)2.0) m_scale = (qreal)2.0;
    if(m_scale < (qreal)0.5) m_scale = (qreal)0.5;
    setupMatrix();
}

//自由缩放
void HGraphEditorOp::zoom()
{

}

//原始大小
void HGraphEditorOp::zoomSame()
{
    if(!m_pGraphEditorMgr && !m_pGraphEditorMgr->graphEditorView())
        return;
    m_scale = (qreal)1.0;
    setupMatrix();
}
