#ifndef HICONTREEWIDGET_H
#define HICONTREEWIDGET_H
//右边的树结构 读取的是图元模板信息结构
#include <QTreeWidget>
class QTreeWidgetItem;
class QListWidget;
class HIconListWidget;
class HIconViewEditor;

class HIconTreeWidgetItem : public QTreeWidgetItem
{

public:
    HIconTreeWidgetItem(QTreeWidget* parent,int type = 0);
    HIconTreeWidgetItem(QTreeWidgetItem* parnet);
public:
    int getIconItemType();
private:
    int nType;
};


class HIconTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    HIconTreeWidget(QWidget* parent = 0);
public:
    void initIconTypes();
    void setIconListWidget(QListWidget* pList);

public slots:
    void treeItemClicked(QTreeWidgetItem*,int);
private:
    HIconListWidget* pIconListWidget;
};

#endif // HICONTREEWIDGET_H
