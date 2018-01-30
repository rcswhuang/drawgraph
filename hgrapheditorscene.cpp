#include "hgrapheditorscene.h"
#include "hgrapheditormgr.h"
#include "hgrapheditordoc.h"
#include "hgraph.h"
#include <QMimeData>
HGraphEditorScene::HGraphEditorScene(HGraphEditorMgr *mgr)
    :pGraphEditorMgr(mgr)
{

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

void HGraphEditorScene::drawForeground(QPainter *painter, const QRectF &rect)
{

}

void HGraphEditorScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{

}



void HGraphEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void HGraphEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    int a = 0;
}


void HGraphEditorScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("DragIcon"))
    {
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
    if(event->mimeData()->hasFormat("DragIcon"))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->ignore();
}

void HGraphEditorScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{

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
