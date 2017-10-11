#ifndef HICONTABWIDGET_H
#define HICONTABWIDGET_H

//右边Tab结构
#include <QTabWidget>
class HGraphEditorMgr;
class HIconTreeWidget;
class HIconListWidget;
class QVBoxLayout;
class HIconTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    HIconTabWidget(HGraphEditorMgr* pMgr);

public:
    void initIconTabWiget();

protected:
    HGraphEditorMgr* pGraphEditorMgr;
    HIconListWidget* pIconListWidget;
    HIconTreeWidget* pIconTreeWidget;
    QVBoxLayout* pVBoxLayout;
};

#endif // HICONTABWIDGET_H
