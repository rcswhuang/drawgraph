#include "hgrapheditorview.h"

HGraphEditorView::HGraphEditorView(QWidget *parent)
    :QGraphicsView (parent)
{
    setCacheMode(QGraphicsView::CacheBackground);//背景缓存渲染时间
    setRubberBandSelectionMode(Qt::ContainsItemShape);//选择拖拽部分
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

