#ifndef HICONTABWIDGET_H
#define HICONTABWIDGET_H

//右边Tab结构
#include <QTabWidget>
class HGraphEditorMgr;

class HIconTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    HIconTabWidget(HGraphEditorMgr* pMgr);

public:
    void initIconTabWiget();
    HGraphEditorMgr* getGraphEditorMgr(){return pGraphEditorMgr;}

signals:
    void iconSelected(const QString&,const QString&,const QString&,int);
public slots:
    void iconTemplateSelected(const QString&,const QString&,const QString&,int);
protected:
    HGraphEditorMgr* pGraphEditorMgr;

};

#endif // HICONTABWIDGET_H
