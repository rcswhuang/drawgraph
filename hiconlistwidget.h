#ifndef HICONLISTWIDGET_H
#define HICONLISTWIDGET_H
//右边下面列表部分，显示每个类型下面的所有图元
#include <QListWidget>
class HIconListWidget :public QListWidget
{
    Q_OBJECT
public:
    HIconListWidget();
};

#endif // HICONLISTWIDGET_H
