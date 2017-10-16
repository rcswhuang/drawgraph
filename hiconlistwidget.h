#ifndef HICONLISTWIDGET_H
#define HICONLISTWIDGET_H
//右边下面列表部分，显示每个类型下面的所有图元
#include <QListWidget>
#include <QListWidgetItem>
class HGraphEditorMgr;
class HIconListWidgetItem:public QListWidgetItem
{
    Q_OBJECT
public:
    HIconListWidgetItem(QListWidget * parent = 0, int type = UserType);
};

class HIconListWidget :public QListWidget
{
    Q_OBJECT
public:
    HIconListWidget(HGraphEditorMgr* pMgr);
    ~HIconListWidget();
public:
    void initIconListWidget();
    void clearIconListWidget();//切换需要先清除
protected:
    HGraphEditorMgr* pGraphEditorMgr;
    QString strUuid;
};

#endif // HICONLISTWIDGET_H
