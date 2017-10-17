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
    //删除画面
    //导入画面
protected:
    void contextMenuEvent(QContextMenuEvent *event);
protected:
    HGraphEditorMgr* pGraphEditorMgr;
};

#endif // HGRAPHTREEWIDGET_H
