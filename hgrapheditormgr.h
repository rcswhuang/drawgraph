#ifndef HGRAPHEDITORMGR_H
#define HGRAPHEDITORMGR_H

#include <QObject>
#include <QRectF>
class HGraphEditorScene;
class HGraphEditorView;
class HGraphEditorDoc;
class HIconGraphicsItem;
class HBaseObj;
class QUndoStatck;
class QUndoCommand;
//class QRectF;
/*
 * 图形绘图工具 管理总类，对各种管理类之间的转换调用进行控制。
 * 包含了各种管理类，绘制类，图形类的对象。
 * 彼此之间的调用都在这里进行转换。
*/
class HGraphEditorMgr : public QObject
{
public:
    HGraphEditorMgr();


    //
    HGraphEditorScene* GraphEditorScene();
    virtual void setGraphEditorView(HGraphEditorView* view);
    HGraphEditorView* graphEditorView();
    HGraphEditorDoc* graphEditorDoc();

public:
    virtual void refresh();

    virtual void reset();

    virtual void print();

    virtual void printPreview();

    virtual void redo();

    virtual void undo();

    virtual void del(QList<HIconGraphicsItem*> &lItem);

    virtual void cut();

    virtual void copy();

    virtual void paste();

    virtual void save();

    virtual void saveAs();

    virtual void initZValue();

    //
    void reName(const QString& newName);

    //增加对象
    virtual void addIconObj(HBaseObj* pObj,bool bPaste = false);

    //创建图符对象
    void createIconObj(const QString&,const QString&,int shape,QPointF,QList<HIconGraphicsItem*> &items);

    //设置逻辑界面大小
    void setLogicRect(const QRectF& rect);

    QRectF getLogicRect();

    //历史
    void addCommand(QUndoCommand* pCommand);

protected:
    HGraphEditorDoc* pGraphEditorDoc;

    HGraphEditorScene* pGraphEditorScene;

    HGraphEditorView* pGraphEditorView;

    QUndoStatck* pCommand;

    QRectF logicRectF;




};

#endif // HGRAPHEDITORMGR_H
