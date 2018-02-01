#ifndef HICONVIEWEDITOR_H
#define HICONVIEWEDITOR_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
class HIconTabWidget;
class HIconListWidget;
class HIconViewEditor;
class HGraphEditorMgr;

//树型控件
class HIconTreeWidgetItem : public QTreeWidgetItem
{

public:
    HIconTreeWidgetItem(QTreeWidgetItem* parent,int type = 0);
    HIconTreeWidgetItem(QTreeWidget* parnet);
public:
    int getIconItemType();
private:
    int nType;
};


class HIconTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    HIconTreeWidget(HGraphEditorMgr* mgr);
public:
    void initIconTypes();
    void setIconListWidget(QListWidget* pList);

public slots:
    void treeItemClicked(QTreeWidgetItem*,int);
private:
    HIconListWidget* pIconListWidget;
    HGraphEditorMgr* pIconEditorMgr;
};

///////////////////////////////////////////////列表控件///////////////////////////////////

class HIconListWidgetItem:public QListWidgetItem
{
public:
    HIconListWidgetItem(QListWidget * parent = 0, int type = Type);
};

class HIconListWidget :public QListWidget
{
    Q_OBJECT
public:
    HIconListWidget(HGraphEditorMgr* mgr);
    ~HIconListWidget();
public:
    void initIconListWidget();
    void clearIconListWidget();//切换需要先清除
    void RefreshIconType(int ntype);

    //要实现拖拽的方式
protected:
    virtual void startDrag(Qt::DropActions supportedActions);
   // virtual void dragLeaveEvent(QDragLeaveEvent *e);
    virtual void dragMoveEvent(QDragMoveEvent *e);
    virtual void dropEnterEvent(QDragEnterEvent *e);

protected:
    HGraphEditorMgr* pGraphEditorMgr;
    QString strUuid;
    int nType;
};

///////////////////////////////////////////////////////view////////////////////////////


class HIconViewEditor : public QWidget
{
    Q_OBJECT
public:
    HIconViewEditor(HIconTabWidget *tabWidget = Q_NULLPTR);

public:
    void initIconViewEditor();


protected:
    HIconTabWidget* pIconTabWidget;//父窗口
    HIconListWidget* pIconListWidget;//下部分显示窗口
    HIconTreeWidget* pIconTreeWidget;//上部分树型窗口

};

#endif // HICONVIEWEDITOR_H
