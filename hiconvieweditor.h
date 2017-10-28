#ifndef HICONVIEWEDITOR_H
#define HICONVIEWEDITOR_H

#include <QWidget>
class HIconTabWidget;
class HIconTreeWidget;
class HIconListWidget;
class HIconViewEditor : public QWidget
{
    Q_OBJECT
public:
    HIconViewEditor(HIconTabWidget *tabWidget = Q_NULLPTR, int nType = 0);

public:
    void initIconViewEditor();


protected:
    HIconTabWidget* pIconTabWidget;//父窗口
    HIconListWidget* pIconListWidget;//下部分显示窗口
    HIconTreeWidget* pIconTreeWidget;//上部分树型窗口

};

#endif // HICONVIEWEDITOR_H
