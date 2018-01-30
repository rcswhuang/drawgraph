#ifndef HGRAPHEDITORSCENE_H
#define HGRAPHEDITORSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
class HGraphEditorMgr;
class HGraphEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    HGraphEditorScene(HGraphEditorMgr *mgr);

public:
    void delGraphEditorSceneItems();

protected:

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

public:
    HGraphEditorMgr* pGraphEditorMgr;
};

#endif // HGRAPHEDITORSCENE_H
