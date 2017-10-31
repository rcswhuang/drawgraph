#include "hicontabwidget.h"
#include "hgrapheditormgr.h"
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
    pIconViewEditor = new HIconViewEditor(this);
    removeTab(0);
    addTab(pIconViewEditor,QStringLiteral("图元"));

    //树形控件
    pIconTreeWidget = new HIconTreeWidget(getGraphEditorMgr());   // (pIconTabWidget->getGraphEditorMgr());

    //列表控件
    pIconListWidget = new HIconListWidget(getGraphEditorMgr());

    //两者关联
    pIconTreeWidget->setIconListWidget(pIconListWidget);
}

//多个Tab页面进行切换的，不同页面，树型和列表都要进行刷新
//如果多个页面需要增加多个tab，每个tab对应一个view就可以了。
void HIconTabWidget::iconTemplateSelected(QString const&, QString const&, QString const&, int)
{
    return;
}
