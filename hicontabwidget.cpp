#include "hicontabwidget.h"
#include "hgrapheditormgr.h"
#include "hicontreewidget.h"
#include "hiconvieweditor.h"
HIconTabWidget::HIconTabWidget(HGraphEditorMgr* pMgr)
    :pGraphEditorMgr(pMgr)
{
    initIconTabWiget();
}

//初始化tab信息
void HIconTabWidget::initIconTabWiget()
{
    setTabPosition(QTabWidget::West);
    //connect(pIconTabWidget,SIGNAL(iconSelected(const QString&,const QString&,const QString&,int)),SLOT(iconTemplateSelected(const QString&,const QString&,const QString&,int)));
    pIconViewEditor = new HIconViewEditor(this);
    removeTab(0);
    addTab(pIconViewEditor,QStringLiteral("图元"));
}

void HIconTabWidget::iconTemplateSelected(QString const&, QString const&, QString const&, int)
{

}
