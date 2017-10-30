#ifndef HICONTREEWIDGET_H
#define HICONTREEWIDGET_H
//右边的树结构 读取的是图元模板信息结构
#include <QTreeWidget>
class QTreeWidgetItem;
class QListWidget;
class HIconListWidget;
class HIconViewEditor;
class HIconEditorMgr;
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
    HIconTreeWidget(HIconEditorMgr* mgr);
public:
    void initIconTypes();
    void setIconListWidget(QListWidget* pList);

public slots:
    void treeItemClicked(QTreeWidgetItem*,int);
private:
    HIconListWidget* pIconListWidget;
    HIconEditorMgr* pIconEditorMgr;
};

#endif // HICONTREEWIDGET_H
