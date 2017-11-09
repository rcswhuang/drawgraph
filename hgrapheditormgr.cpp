﻿#include "hgrapheditormgr.h"
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



