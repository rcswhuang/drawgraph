#ifndef HGRAPHTREEWIDGET_H
#define HGRAPHTREEWIDGET_H
//左边的树结构
#include <QTreeWidget>
#include <QTreeWidgetItem>

class HGraphEditorMgr;
class QContextMenuEvent;
class HGraphTreeWidgetItem:public QTreeWidgetItem
{
public:
    HGraphTreeWidgetItem(QTreeWidget * parent, int type = UserType);
    HGraphTreeWidgetItem(QTreeWidgetItem * parent, int type = UserType);
    ~HGraphTreeWidgetItem();

public:
    void setGraphTreeID(int graphTreeID);
    int getGraphTreeID();
private:
    int wGraphTreeID;
};

class HGraphTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    HGraphTreeWidget(HGraphEditorMgr *pMgr);
    ~HGraphTreeWidget();
public:
    //初始化树结构
    void initGraphTreeWidget();

    //新建画面
    void addGraphTreeWidgetItem();

    //删除画面
    void delGraphTreeWidgetItem();

    //导入画面
    void importGraphTreeWigetItem();

    //根目录右键菜单
    void initGraphMenu(QContextMenuEvent* event);

    //画面右键菜单
    void intGraphFileMenu(QContextMenuEvent* event);

public slots:

    //新建画面
    void newGraph();

    //打开画面
    void openGraph();

    //删除管理
    void delGraph();

    //另存画面
    void saveAsGraph();

    //导入画面文件夹
    void importFolderGraph();

    //导入画面文件
    void importFileGraph();

    //点击树
    void clickGraphItem(QTreeWidgetItem*,int);

    //改变
    void changedGraphItem(QTreeWidgetItem*,QTreeWidgetItem*);
signals:
    void graphNew(const QString&);
    void graphOpen(const QString&,const int graphID);
    void graphDel(const QString&,const int graphID);
    void graphImport(const QString&);
    void graphImportPath(const QString&);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
protected:
    HGraphEditorMgr* pGraphEditorMgr;
    int nPreGraphID;
};

#endif // HGRAPHTREEWIDGET_H
