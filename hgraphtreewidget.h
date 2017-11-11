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

public:
    void setGraphTreeID(unsigned short graphTreeID);
    unsigned short getGraphTreeID();
private:
    unsigned short wGraphTreeID;
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
    void initGraphTreeWigetItem();

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

    //导入画面
    void importGraph();

signals:
    void graphNew(const QString&);
    void graphOepn(const QString&,const int graphID);
    void graphDel(const QString&,const int graphID);
    void graphOpen();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
protected:
    HGraphEditorMgr* pGraphEditorMgr;
};

#endif // HGRAPHTREEWIDGET_H
