#include "hgrapheditorview.h"

HGraphEditorView::HGraphEditorView(QWidget *parent)
    :QGraphicsView (parent)
{
    setAcceptDrops(true);
    //setDragMode(QGraphicsView::ScrollHandDrag);
    setCacheMode(QGraphicsView::CacheBackground);//背景缓存渲染时间
    setRubberBandSelectionMode(Qt::ContainsItemShape);//选择拖拽部分
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void HGraphEditorView::refresh()
{
    int width = viewport()->width();
    int height = viewport()->height();
    resize(width-1,height-1);
    resize(width,height);
}
