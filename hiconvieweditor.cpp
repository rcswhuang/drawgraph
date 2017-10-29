#include "hiconvieweditor.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QTreeWidget>
#include "hgrapheditormgr.h"
#include "hiconlistwidget.h"
#include "hicontreewidget.h"
#include "hicontabwidget.h"
//Tab上的
/*
 * 树型窗口显示所有模板图符的名称信息，树型里面的类型都是按照模板里面区分的(包含遥信类，遥测类......)
 * 模板的名称都是从doc类里面来的,只要显示模板的名称就可以了。
 *
 * 列表窗口显示对应树型类型里面的所有图符信息
*/
HIconViewEditor::HIconViewEditor(HIconTabWidget *tabWidget , int nType )
    :pIconTabWidget(tabWidget)
{
    QVBoxLayout* pVBoxLayout = new QVBoxLayout(this);
    Q_ASSERT(pVBoxLayout);
    pVBoxLayout->setMargin(0);

    pIconTreeWidget = new HIconTreeWidget(pIconTabWidget->getGraphEditorMgr());
    pVBoxLayout->addWidget(pIconTreeWidget);
    pIconListWidget = new HIconListWidget(this);
    pVBoxLayout->addWidget(pIconListWidget);

    pIconTreeWidget->setIconListWidget(pIconListWidget);
}


void HIconViewEditor::initIconViewEditor()
{
    //获取模板信息
}
