#ifndef HGRAPHEDITORMGR_H
#define HGRAPHEDITORMGR_H

#include <QObject>
#include <QRectF>
#include <QUndoStack>
#include <QUndoCommand>
#include "hiconapi.h"
class HGraphEditorScene;
class HGraphEditorView;
class HGraphEditorDoc;
class HIconGraphicsItem;
class HBaseObj;
class HGraph;
//class QRectF;
/*
 * 图形绘图工具 管理总类，对各种管理类之间的转换调用进行控制。
 * 包含了各种管理类，绘制类，图形类的对象。
 * 彼此之间的调用都在这里进行转换。
 *
*/
class HGraphEditorMgr : public QObject
{
    Q_OBJECT
public:
    HGraphEditorMgr();


    //
    HGraphEditorScene* GraphEditorScene();
    virtual void setGraphEditorView(HGraphEditorView* view);
    HGraphEditorView* graphEditorView();
    HGraphEditorDoc* graphEditorDoc();

public:

    //启动时加载数据库
    void loadStation();

    //启动时加载模板信息
    void loadTemplates();

    //启动时加载画面信息
    void loadGraphs();



    //判断graph文件名是否存在
    bool findGraphByName(const QString&);

    //判断graph文件是否修改
    bool isGraphModify();

    //新建文件
    void New(const QString&);

    bool Open(const QString&,int);

    bool Del(const QString&,int);

    int ImportFile(const QString&);

   /* virtual void refresh();

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
    void reName(const QString& newName);*/

    void setDrawShape(DRAWSHAPE ds);
    DRAWSHAPE getDrawShape();

    virtual void ObjCreated(HBaseObj* pObj);

    //增加对象
    virtual void addIconObj(HBaseObj* pObj,bool bPaste = false);

    //创建图符对象
    void createIconObj(const QString&,const QString&,int shape,QPointF,QList<HIconGraphicsItem*> &items);



    //历史
    //void addCommand(QUndoCommand* pCommand);
    //设置逻辑界面大小
    void setLogicRect(const QRectF& rect);

    QRectF getLogicRect();

    //刷新view
    void refreshView();

    //清空scene上面所有的Item
    void openGraphScene();
    void delGraphSceneItem();


protected:
    HGraphEditorDoc* pGraphEditorDoc;

    HGraphEditorScene* pGraphEditorScene;

    HGraphEditorView* pGraphEditorView;

    QUndoStack* pCommand;

private:
    DRAWSHAPE drawShape;
    QRectF logicRectF;  //scene的大小
    HGraph* pTempGraph; //临时画面文件
    QString strGraphFile;




};

#endif // HGRAPHEDITORMGR_H
