#include "hgrapheditormgr.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QScrollBar>
#include <QDir>
#include "hgrapheditorview.h"
#include "hgrapheditordoc.h"
#include "hgrapheditorscene.h"
#include "hgrapheditorop.h"
#include "hgraphundocommand.h"
#include "hgraph.h"
//图形文件管理总类
HGraphEditorMgr::HGraphEditorMgr()
    :logicRectF(-500,-500,1000,1000)
{
    pGraphEditorDoc = new HGraphEditorDoc(this);
    Q_ASSERT(pGraphEditorDoc);

    m_pGraphEditorUndoStack = new QUndoStack;

    //pTempGraph = new HGraph("");

    int width = qApp->desktop()->screen()->width();
    int height = qApp->desktop()->screen()->height();
    logicRectF.setX(0-(width-2)/2);
    logicRectF.setY(0-(height-100)/2);
    logicRectF.setWidth(width-2);
    logicRectF.setHeight(height-100/2);

    pGraphEditorScene = new HGraphEditorScene(this);
    pGraphEditorView = NULL;
    m_pGraphEditorOp = new HGraphEditorOp(this);
    drawShape = enumNo;
    selectMode = enumSelect;
}

//启动时加载数据库
void HGraphEditorMgr::loadStation()
{
    if(!pGraphEditorDoc)
        return;
    pGraphEditorDoc->loadStation();
}

//启动时加载模板信息
void HGraphEditorMgr::loadTemplates()
{
    if(!pGraphEditorDoc)
        return;
    pGraphEditorDoc->loadIconTemplate();
}

//启动时加载画面信息
void HGraphEditorMgr::loadGraphs()
{
    if(!pGraphEditorDoc)
        return;
    pGraphEditorDoc->loadAllGraph();
}

HGraphEditorOp* HGraphEditorMgr::graphEditorOp()
{
    return m_pGraphEditorOp;
}

HGraphEditorScene* HGraphEditorMgr::graphEditorScene()
{
    return pGraphEditorScene;
}

void HGraphEditorMgr::setGraphEditorView(HGraphEditorView* view)
{
    pGraphEditorView = view;
    pGraphEditorView->setScene(pGraphEditorScene);
    QRectF rectF(logicRectF);
    pGraphEditorScene->setSceneRect(rectF);

    if(pTempGraph)
    {
       // p
    }

    QScrollBar* pBar = pGraphEditorView->horizontalScrollBar();
    if(pBar && pBar->isHidden() == false)
    {
        pBar->setSliderPosition(pBar->minimum());
    }
    pBar = pGraphEditorView->verticalScrollBar();
    if(pBar && pBar->isHidden() == false)
    {
        pBar->setSliderPosition(pBar->minimum());
    }
}

HGraphEditorView* HGraphEditorMgr::graphEditorView()
{
    return pGraphEditorView;
}

HGraphEditorDoc* HGraphEditorMgr::graphEditorDoc()
{
    return pGraphEditorDoc;
}

QUndoStack* HGraphEditorMgr::graphEditorStack()
{
    return m_pGraphEditorUndoStack;
}

//设置逻辑界面大小
void HGraphEditorMgr::setLogicRect(const QRectF& rect)
{
    logicRectF = rect;
}

QRectF HGraphEditorMgr::getLogicRect()
{
    return logicRectF;
}

//判断graph文件名是否存在
bool HGraphEditorMgr::findGraphByName(const QString& graphName)
{
    if(!pGraphEditorDoc)
        return false;
    HGraph* graph = pGraphEditorDoc->findGraph(graphName);
    if(!graph)
        return false;
    return true;
}

//判断graph文件是否修改
bool HGraphEditorMgr::isGraphModify()
{
    if(!pGraphEditorDoc)
        return false;
    return pGraphEditorDoc->isGraphModify();
}

void HGraphEditorMgr::setDrawShape(DRAWSHAPE ds)
{
    drawShape = ds;
}

DRAWSHAPE HGraphEditorMgr::getDrawShape()
{
    return drawShape;
}

void HGraphEditorMgr::setSelectMode(SELECTMODE mode)
{
    selectMode = mode;
}

SELECTMODE HGraphEditorMgr::getSelectMode()
{
    return selectMode;
}

//新建文件
void HGraphEditorMgr::New(const QString& graphName)
{
    if(!pGraphEditorDoc)
        return;
    pGraphEditorDoc->addGraph(graphName);
}

bool HGraphEditorMgr::Open(const QString& graphName,int id)
{
    if(!pGraphEditorDoc)
        return false;
    return pGraphEditorDoc->openGraph(graphName,id);
}

void HGraphEditorMgr::Save()
{
    if(!pGraphEditorDoc)
        return;
    return pGraphEditorDoc->saveCurGraph();
}

bool HGraphEditorMgr::Del(const QString& graphName,int id)
{
    if(!pGraphEditorDoc)
        return false;
    return pGraphEditorDoc->delGraph(graphName,id);
}

int HGraphEditorMgr::ImportFile(const QString& graphName)
{
    if(!pGraphEditorDoc)
        return false;
    return pGraphEditorDoc->importGraph(graphName);
}

void HGraphEditorMgr::refreshView()
{
    if(!pGraphEditorView)
        return;
    pGraphEditorView->refresh();
}

void HGraphEditorMgr::openGraphScene()
{
    if(!pGraphEditorScene)
        return;
    pGraphEditorScene->openGraphEditorSceneItems();
}

void HGraphEditorMgr::delGraphSceneItem()
{
    if(!pGraphEditorScene)
        return;
    pGraphEditorScene->delGraphEditorSceneItems();
}

//创建图符对象
//类型(遥信,遥控...),uuid,shape,fpoint
void HGraphEditorMgr::createIconObj(const QString& TypeName,const QString& uuid,int shape,QPointF fpoint,QList<HIconGraphicsItem*> &items)
{
    HIconTemplate* pIconTemplate;// = new HIconTemplate("");
    //先到画面模板中去寻找
    if(!pGraphEditorDoc || !pGraphEditorDoc->getCurGraph())
        return;
    pIconTemplate = pGraphEditorDoc->getCurGraph()->findIconTemplate(uuid);
    if(!pIconTemplate)//没找到
    {
        pIconTemplate = new HIconTemplate("");
        HIconTemplate* pTemplate = pGraphEditorDoc->findIconTemplate(uuid);
        pTemplate->copyTo(pIconTemplate);
        pGraphEditorDoc->getCurGraph()->addIconTemplate(pIconTemplate);
    }

    HBaseObj* pObj = pGraphEditorDoc->getCurGraph()->newObj(enumComplex,uuid);
    HIconObj* pIconObj = (HIconObj*)pObj;
    pIconObj->initIconTemplate();
    //设置图元坐标位置
    double width = 100;
    double height = 100;
    QSizeF sizeF = pIconTemplate->getDefaultSize();
    double rectwidth = sizeF.width();
    double rectheight = sizeF.height();
    if(rectwidth > 0 || rectheight > 0)
    {
        width = rectwidth;
        height = rectheight;
    }
    else
    {
        width = 30;
        height = 30;
    }

    QPointF ptCenter = QPointF(fpoint.x() - width/2,fpoint.y() - height/2);
    pIconObj->setTopLeft(ptCenter);
    pIconObj->setRectWidth(width);
    pIconObj->setRectHeight(height);
    QSizeF pt = pIconTemplate->getDefaultSize();
    double w1 = width/(pt.width()*20);
    double h1 = height/(pt.height()*20);
    pIconObj->resetRectPoint(ptCenter,QPointF(-pt.width()*10,-pt.height()*10));
    pIconObj->resize(w1,h1);
    addIconObj(pIconObj);
    items.append(pIconObj->getIconGraphicsItem());
}

//增加对象
void HGraphEditorMgr::addIconObj(HBaseObj* pObj,bool bPaste)
{
    if(!pObj)
        return;
    //图元对象添加到画面文件
    pGraphEditorDoc->getCurGraph()->addObj(pObj);
    ObjCreated(pObj);
}

void HGraphEditorMgr::ObjCreated(HBaseObj* pObj)
{
    //添加Item之后不能删除，后面还有拖拽移动等操作，等到拖拽释放之后才能删除
    pGraphEditorScene->addIconGraphicsItem(pObj);
}

void HGraphEditorMgr::addNewCommand(HBaseObj *pObj)
{
    if(!m_pGraphEditorUndoStack)
        return;
    HGraphNewCommand* newGraphCommand = new HGraphNewCommand(this,pObj);
    m_pGraphEditorUndoStack->push(newGraphCommand);
}

void HGraphEditorMgr::addDelCommand(QList<HBaseObj*> pObjs)
{
    if(!m_pGraphEditorUndoStack)
        return;
    HGraphDelCommand* delGraphCommand = new HGraphDelCommand(this,pObjs);
    m_pGraphEditorUndoStack->push(delGraphCommand);
}

void HGraphEditorMgr::addPasteCommand(QList<HBaseObj*> pObjs)
{
    if(!m_pGraphEditorUndoStack)
        return;
    HGraphPasteCommand* pasteGraphCommand = new HGraphPasteCommand(this,pObjs);
    m_pGraphEditorUndoStack->push(pasteGraphCommand);
}

void HGraphEditorMgr::addMoveCommand(QList<HBaseObj*> pObjs,qreal dx,qreal dy)
{
    if(!m_pGraphEditorUndoStack)
        return;
    HGraphMoveCommand* moveGraphCommand = new HGraphMoveCommand(this,pObjs,dx,dy);
    m_pGraphEditorUndoStack->push(moveGraphCommand);
}

void HGraphEditorMgr::addRotateCommand(QList<HBaseObj*> pObjs,float angle)
{
    if(!m_pGraphEditorUndoStack)
        return;
    HGraphRotateCommand* rotateGraphCommand = new HGraphRotateCommand(this,pObjs,angle);
    m_pGraphEditorUndoStack->push(rotateGraphCommand);
}

void HGraphEditorMgr::addTurnCommand(QList<HBaseObj*> pObjs,bool bdirection)
{
    if(!m_pGraphEditorUndoStack)
        return;
    HGraphTurnCommand* turnGraphCommand = new HGraphTurnCommand(this,pObjs,bdirection);
    m_pGraphEditorUndoStack->push(turnGraphCommand);
}

void HGraphEditorMgr::addResizeCommand(QList<HBaseObj*> pObjs,QList<QPolygonF> oldPts,QList<QPolygonF> newPts)
{
    if(!m_pGraphEditorUndoStack)
        return;
    HGraphResizeCommand* resizeGraphCommand = new HGraphResizeCommand(this,pObjs,oldPts,newPts);
    m_pGraphEditorUndoStack->push(resizeGraphCommand);
}








