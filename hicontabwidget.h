#ifndef HICONTABWIDGET_H
#define HICONTABWIDGET_H

//右边Tab结构
#include <QTabWidget>
class HGraphEditorMgr;
class HIconViewEditor;
class HIconTreeWidget;
class HIconListWidget;
class HIconTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    HIconTabWidget(HGraphEditorMgr* pMgr);

public:
    void initIconTabWiget();
    HGraphEditorMgr* getGraphEditorMgr(){return pGraphEditorMgr;}

    HIconListWidget* getIconListWidget();
    HIconTreeWidget* getIconTreeWidget();
signals:
    void iconSelected(const QString&,const QString&,const QString&,int);
public slots:
    void iconTemplateSelected(const QString&,const QString&,const QString&,int);
protected:
    HGraphEditorMgr* pGraphEditorMgr;
    HIconViewEditor* pIconViewEditor;
    HIconListWidget* pIconListWidget;//下部分显示窗口
    HIconTreeWidget* pIconTreeWidget;//上部分树型窗口
    //应该把tree和list放到此处，供每个类型的view来调用，而不是放到每个view里面

};

#endif // HICONTABWIDGET_H
