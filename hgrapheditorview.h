#ifndef HGRAPHEDITORVIEW_H
#define HGRAPHEDITORVIEW_H

#include <QGraphicsView>
class HGraphEditorView : public QGraphicsView
{
    Q_OBJECT
public:
    HGraphEditorView(QWidget *parent = Q_NULLPTR);

public:
    //目前只刷新界面
    void refresh();

public:

};

#endif // HGRAPHEDITORVIEW_H
