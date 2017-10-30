#ifndef HICONLISTWIDGET_H
#define HICONLISTWIDGET_H
//右边下面列表部分，显示每个类型下面的所有图元
#include <QListWidget>
#include <QListWidgetItem>
class HGraphEditorMgr;
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

protected:
    HGraphEditorMgr* pGraphEditorMgr;
    QString strUuid;
    int nType;
};

#endif // HICONLISTWIDGET_H
