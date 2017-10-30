#include "hiconvieweditor.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QTreeWidget>
#include "hiconapi.h" //图元api 后续要统一到一个api头文件中去
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


    pIconTreeWidget = new HIconTreeWidget((HIconEditorMgr*)pIconTabWidget->getGraphEditorMgr());   // (pIconTabWidget->getGraphEditorMgr());
    pVBoxLayout->addWidget(pIconTreeWidget);
    pIconListWidget = new HIconListWidget(this);
    pVBoxLayout->addWidget(pIconListWidget);

    pIconTreeWidget->setIconListWidget(pIconListWidget);
}


void HIconViewEditor::initIconViewEditor()
{
    //树型结构只要读取icon存放的各种大类型，比如遥信，遥测，遥控遥调，接地线，牌等等
    //这些结构和名称都是事先定义好的。




    //list结构是需要读取icontemplate里面的每个模板的信息，模板都是分类放置，设置uuid,icon,name等等
}
