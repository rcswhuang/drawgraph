#include "hgrapheditormgr.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QScrollBar>
#include "hgrapheditorview.h"
#include "hgrapheditordoc.h"
#include "hgrapheditorscene.h"
#include "hgraph.h"
//图形文件管理总类
HGraphEditorMgr::HGraphEditorMgr()
    :logicRectF(-500,-500,1000,1000)
{
    pGraphEditorDoc = new HGraphEditorDoc(this);
    Q_ASSERT(pGraphEditorDoc);

   // pCommand = new QUndoStack(this);

    //pTempGraph = new HGraph("");

    int width = qApp->desktop()->screen()->width();
    int height = qApp->desktop()->screen()->height();
    logicRectF.setX(0-(width-2)/2);
    logicRectF.setY(0-(height-100)/2);
    logicRectF.setWidth(width-2);
    logicRectF.setHeight(height-100/2);

    pGraphEditorScene = new HGraphEditorScene(this);
    pGraphEditorView = NULL;

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



HGraphEditorScene* HGraphEditorMgr::GraphEditorScene()
{
    return pGraphEditorScene;
}

void HGraphEditorMgr::setGraphEditorView(HGraphEditorView* view)
{
    pGraphEditorView = view;
    pGraphEditorView->setScene(pGraphEditorScene);
    //pGraphEditorScene->setView(pGraphEditorView);
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

//新建文件
void HGraphEditorMgr::New(const QString& graphName)
{
    if(!pGraphEditorDoc)
        return;
    pGraphEditorDoc->addGraph(graphName);
}

void HGraphEditorMgr::refreshView()
{
    if(!pGraphEditorView)
        return;
    pGraphEditorView->refresh();
}

void HGraphEditorMgr::delGraphSceneItem()
{
    if(!pGraphEditorScene)
        return;
    pGraphEditorScene->delGraphEditorSceneItems();
}


//增加对象
void HGraphEditorMgr::addIconObj(HBaseObj* pObj,bool bPaste)
{

}

//创建图符对象
//类型(遥信,遥控...),uuid,shape,fpoint
void HGraphEditorMgr::createIconObj(const QString& TypeName,const QString& uuid,int shape,QPointF fpoint,QList<HIconGraphicsItem*> &items)
{
    HIconTemplate* pIconTemplate = 0;
    HIconTemplate* pTemplate = pGraphEditorDoc->findIconTemplate(uuid);

}


