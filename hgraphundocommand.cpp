#include "hgraphundocommand.h"
#include "hiconapi.h"
#include "hbaseobj.h"
#include "hiconobj.h"
#include "hgrapheditormgr.h"
#include "hicongraphicsitem.h"
#include "hiconlineitem.h"
#include "hgrapheditorview.h"
#include "hgrapheditorscene.h"
HGraphUndoCommand::HGraphUndoCommand(HGraphEditorMgr* graphEditorMgr):m_pGraphEditorMgr(graphEditorMgr)
{
    bFirstTime = true;
}

HGraphUndoCommand::~HGraphUndoCommand()
{

}


int HGraphUndoCommand::id() const
{
    return -1;
}
void HGraphUndoCommand::redo()
{

}

void HGraphUndoCommand::undo()
{

}


////////////////////////////////////////新建///////////////////////////////////////////
/***************************
 * 备注:新建对象为单个对象，如果是多个对象，则换成list
 *
 * **************************/
HGraphNewCommand::HGraphNewCommand(HGraphEditorMgr* graphEditorMgr,HBaseObj* obj):HGraphUndoCommand(graphEditorMgr),pObj(obj)
{
    setText("create object");
}

HGraphNewCommand::~HGraphNewCommand()
{

}

int HGraphNewCommand::id() const
{
    return New;
}

void HGraphNewCommand::redo()
{
    if(bFirstTime)
    {
        bFirstTime = false;
        return;
    }
    if(!m_pGraphEditorMgr || !pObj)
        return;
    pObj->setDeleted(false);
    HIconGraphicsItem* item = pObj->getIconGraphicsItem();
    if(item && m_pGraphEditorMgr->graphEditorView())
    {
        m_pGraphEditorMgr->graphEditorView()->ensureVisible(item);
        item->setVisible(true);
    }
}

void HGraphNewCommand::undo()
{
    if(!m_pGraphEditorMgr || !pObj )
        return;
    //删除新建的
    pObj->setDeleted(true);
    HIconGraphicsItem* item = pObj->getIconGraphicsItem();
    if(item)
        item->setVisible(false);
}

////////////////////////////////////////删除///////////////////////////////////////////
HGraphDelCommand::HGraphDelCommand(HGraphEditorMgr* graphEditorMgr,QList<HBaseObj*> objs):HGraphUndoCommand(graphEditorMgr),pObjList(objs)
{
    setText("create object");
}

HGraphDelCommand::~HGraphDelCommand()
{

}

int HGraphDelCommand::id() const
{
    return Delete;
}

void HGraphDelCommand::redo()
{
    if(bFirstTime)
    {
        bFirstTime = false;
        return;
    }
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF bounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        obj->setDeleted(true);
        bounding = bounding.united(item->boundingRect());
        item->setVisible(false);
    }
    //pIconMgr->getIconFrame()->refreshSelected(bounding);
}

void HGraphDelCommand::undo()
{
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF bounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        obj->setDeleted(false);
        bounding = bounding.united(item->boundingRect());
        item->setVisible(true);
    }
    //pIconMgr->getIconFrame()->refreshSelected(bounding);
}


////////////////////////////////////////粘贴///////////////////////////////////////////
HGraphPasteCommand::HGraphPasteCommand(HGraphEditorMgr* graphEditor,QList<HBaseObj*> objs):HGraphUndoCommand(graphEditor),pObjList(objs)
{
    setText("create object");
}

HGraphPasteCommand::~HGraphPasteCommand()
{

}

int HGraphPasteCommand::id() const
{
    return Paste;
}

void HGraphPasteCommand::redo()
{
    if(bFirstTime)
    {
        bFirstTime = false;
        return;
    }
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF bounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        obj->setDeleted(false);
        bounding = bounding.united(item->boundingRect());
        item->setVisible(true);
    }
    //pIconMgr->getIconFrame()->refreshSelected(bounding);
}

void HGraphPasteCommand::undo()
{
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF bounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        obj->setDeleted(true);
        bounding = bounding.united(item->boundingRect());
        item->setVisible(false);
    }
    //pIconMgr->getIconFrame()->refreshSelected(bounding);
}


////////////////////////////////////////移动///////////////////////////////////////////
HGraphMoveCommand::HGraphMoveCommand(HGraphEditorMgr* graphEditor,QList<HBaseObj*> pObjs,qreal dx,qreal dy)
    :HGraphUndoCommand(graphEditor),pObjList(pObjs)
{
    for(int i = 0; i < pObjList.count();i++)
    {
        dxList.append(dx);
        dyList.append(dy);
    }
    setText("Move Object(s)");
}

HGraphMoveCommand::HGraphMoveCommand(HGraphEditorMgr* graphEditor,QList<HBaseObj*> pObjs,QList<qreal> dxs,QList<qreal> dys)
    :HGraphUndoCommand(graphEditor),pObjList(pObjs),dxList(dxs),dyList(dys)
{
    setText("Move Object(s)");
}

HGraphMoveCommand::HGraphMoveCommand(HGraphEditorMgr* graphEditor,QList<HBaseObj*> pObjs,QList<QPointF> oldPt,QList<QPointF> newPt)
    :HGraphUndoCommand(graphEditor),pObjList(pObjs)
{
    for(int i = 0; i< pObjList.count();i++)
    {
        dxList.append(newPt.at(i).x() - oldPt.at(i).x());
        dyList.append(newPt.at(i).y() - oldPt.at(i).y());
    }
    setText("Move Object(s)");
}

HGraphMoveCommand::~HGraphMoveCommand()
{

}

int HGraphMoveCommand::id() const
{
    return Move;
}

void HGraphMoveCommand::redo()
{
    //redo只能是undo完成之后的动作，第一次不能有redo动作
    if(bFirstTime)
    {
        bFirstTime = false;
        return;
    }
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF oldBounding;
    QRectF newBounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        oldBounding = oldBounding.united(item->boundingRect());
        newBounding = newBounding.united(item->boundingRect().translated(dxList[i],dyList[i]));
        item->moveItemBy(dxList[i],dyList[i]);
    }
    //需要把select给删除掉
    m_pGraphEditorMgr->graphEditorScene()->clearSelectItem();
    //pIconMgr->getIconFrame()->refreshSelected(oldBounding);
   // pIconMgr->getIconFrame()->refreshSelected(newBounding);
   // m_pGraphEditorMgr->graphEditorView()->ensureVisible(newBounding);
}

void HGraphMoveCommand::undo()
{
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF oldBounding;
    QRectF newBounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        oldBounding = oldBounding.united(item->boundingRect());
        newBounding = newBounding.united(item->boundingRect().translated(-dxList[i],-dyList[i]));
        item->moveItemBy(-dxList[i],-dyList[i]);
    }
    m_pGraphEditorMgr->graphEditorScene()->clearSelectItem();
    //m_pGraphEditorMgr->graphEditorView()->ensureVisible(newBounding);
}


////////////////////////////////////////旋转///////////////////////////////////////////
HGraphRotateCommand::HGraphRotateCommand(HGraphEditorMgr* graphEditor,QList<HBaseObj*> pObjs,float angle)
    :HGraphUndoCommand(graphEditor),pObjList(pObjs)
{
    for(int i = 0; i < pObjList.count();i++)
    {
        angleList.append(angle);
    }
    setText("rotate object");
}

HGraphRotateCommand::HGraphRotateCommand(HGraphEditorMgr* graphEditor,QList<HBaseObj*> pObjs,QList<float> angles)
    :HGraphUndoCommand(graphEditor),pObjList(pObjs),angleList(angles)
{
    setText("rotate object");
}

HGraphRotateCommand::~HGraphRotateCommand()
{

}

int HGraphRotateCommand::id() const
{
    return Rotate;
}

void HGraphRotateCommand::redo()
{
    if(bFirstTime)
    {
        bFirstTime = false;
        return;
    }
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF oldBounding;
    QRectF newBounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        //oldBounding = oldBounding.united(item->boundingRect());
        obj->setRotateAdd(angleList[i]);
        newBounding = newBounding.united(item->boundingRect());
    }
    //pIconMgr->getIconFrame()->refreshSelected(oldBounding);
    //pIconMgr->getIconFrame()->refreshSelected(newBounding);
    m_pGraphEditorMgr->graphEditorView()->ensureVisible(newBounding);
}

void HGraphRotateCommand::undo()
{
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF oldBounding;
    QRectF newBounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        oldBounding = oldBounding.united(item->boundingRect());
        obj->setRotateAdd(-angleList[i]);
        newBounding = newBounding.united(item->boundingRect());
    }
    //pIconMgr->getIconFrame()->refreshSelected(oldBounding);
   // pIconMgr->getIconFrame()->refreshSelected(newBounding);
    m_pGraphEditorMgr->graphEditorView()->ensureVisible(newBounding);
}


////////////////////////////////////////翻转///////////////////////////////////////////
/*
 * direction = true; 水平翻转，direction = false;垂直翻转
*/
HGraphTurnCommand::HGraphTurnCommand(HGraphEditorMgr* graphEditorMgr,QList<HBaseObj*> objs,bool direction )
    :HGraphUndoCommand(graphEditorMgr),pObjList(objs),bDirection(direction)
{
    setText("turn object");
}

HGraphTurnCommand::~HGraphTurnCommand()
{

}

int HGraphTurnCommand::id() const
{
    return Turn;
}

void HGraphTurnCommand::redo()
{
    if(bFirstTime)
    {
        bFirstTime = false;
        return;
    }
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF oldBounding;
    QRectF newBounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        oldBounding = oldBounding.united(item->boundingRect());
        if(bDirection)
            obj->setTurn(obj->bHorizonTurn,!obj->bVerticalTurn);
        else
            obj->setTurn(!obj->bHorizonTurn,obj->bVerticalTurn);
        newBounding = newBounding.united(item->boundingRect());
    }
    //pIconMgr->getIconFrame()->refreshSelected(oldBounding);
   // pIconMgr->getIconFrame()->refreshSelected(newBounding);
    m_pGraphEditorMgr->graphEditorView()->ensureVisible(newBounding);
}

void HGraphTurnCommand::undo()
{
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF oldBounding;
    QRectF newBounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        oldBounding = oldBounding.united(item->boundingRect());
        if(bDirection)
            obj->setTurn(obj->bHorizonTurn,!obj->bVerticalTurn);
        else
            obj->setTurn(!obj->bHorizonTurn,obj->bVerticalTurn);
        newBounding = newBounding.united(item->boundingRect());
    }
    //pIconMgr->getIconFrame()->refreshSelected(oldBounding);
   // pIconMgr->getIconFrame()->refreshSelected(newBounding);
    m_pGraphEditorMgr->graphEditorView()->ensureVisible(newBounding);
}


////////////////////////////////////////改变大小///////////////////////////////////////////
HGraphResizeCommand::HGraphResizeCommand(HGraphEditorMgr* graphEditorMgr,QList<HBaseObj*> objs,QList<QPolygonF> oldPts,QList<QPolygonF> newPts)
    :HGraphUndoCommand(graphEditorMgr),pObjList(objs),oldPtList(oldPts),newPtList(newPts)
{
    setText("resize object");
}

HGraphResizeCommand::~HGraphResizeCommand()
{

}

int HGraphResizeCommand::id() const
{
    return Resize;
}

void HGraphResizeCommand::redo()
{
    if(bFirstTime)
    {
        bFirstTime = false;
        return;
    }
    if(!m_pGraphEditorMgr || pObjList.isEmpty() )
        return;
    QRectF oldBounding;
    QRectF newBounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        oldBounding = oldBounding.united(item->boundingRect());
        item->resizeItem(newPtList[i]);
        newBounding = newBounding.united(item->boundingRect());
    }
    //pIconMgr->getIconFrame()->refreshSelected(oldBounding);
    //pIconMgr->getIconFrame()->refreshSelected(newBounding);
    m_pGraphEditorMgr->graphEditorView()->ensureVisible(newBounding);
}

void HGraphResizeCommand::undo()
{
    if(!m_pGraphEditorMgr || pObjList.isEmpty())
        return;
    QRectF oldBounding;
    QRectF newBounding;
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* obj = (HBaseObj*)pObjList[i];
        if(!obj) continue;
        HIconGraphicsItem* item = obj->getIconGraphicsItem();
        if(!item) continue;
        oldBounding = oldBounding.united(item->boundingRect());
        item->resizeItem(oldPtList[i]);
        newBounding = newBounding.united(item->boundingRect());
    }
    //pIconMgr->getIconFrame()->refreshSelected(oldBounding);
    //pIconMgr->getIconFrame()->refreshSelected(newBounding);
    m_pGraphEditorMgr->graphEditorView()->ensureVisible(newBounding);
}
