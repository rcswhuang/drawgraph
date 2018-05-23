#include "hgrapheditorview.h"
#include <QScrollBar>
HGraphEditorView::HGraphEditorView(QWidget *parent)
    :QGraphicsView (parent)
{
    setAcceptDrops(true);
    //setDragMode(QGraphicsView::ScrollHandDrag);
    setCacheMode(QGraphicsView::CacheBackground);//背景缓存渲染时间
    setRubberBandSelectionMode(Qt::ContainsItemShape);//选择拖拽部分
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    viewport()->setMouseTracking(true);
}

void HGraphEditorView::refresh()
{
    QScrollBar* pBar = horizontalScrollBar();
    int hBar = 0;
    int vBar = 0;
    if(pBar && pBar->isHidden() == false )
    {
        hBar = pBar->height();//水平滚动条的高度
    }
    pBar =verticalScrollBar();
    if(pBar && pBar->isHidden() == false)
    {
        vBar = pBar->width();//垂直滚动条的宽度
    }
    int width = viewport()->width() + hBar;
    int height = viewport()->height() + vBar;
    resize(width-1,height-1);
    resize(width,height);
}

