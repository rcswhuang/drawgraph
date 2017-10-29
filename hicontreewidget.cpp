#include "hicontreewidget.h"
#include <QListWidget>
#include <QTreeWidgetItem>
#include "hiconlistwidget.h"
#include "hgrapheditormgr.h"
HIconTreeWidgetItem::HIconTreeWidgetItem(QTreeWidget* parent,int type ):QTreeWidgetItem(parent)
{

}

HIconTreeWidgetItem::HIconTreeWidgetItem(QTreeWidgetItem* parnet)
    :QTreeWidgetItem (parnet)
{

}

int HIconTreeWidgetItem::getIconItemType()
{
    return nType;
}

HIconTreeWidget::HIconTreeWidget(HIconEditorMgr* mgr)
    :pIconEditorMgr(mgr)
{
    initIconTypes();

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(treeItemClicked(QTreeWidgetItem*,int)));
    pIconListWidget = NULL;
}

void HIconTreeWidget::setIconListWidget(QListWidget* pList)
{
    pIconListWidget = static_cast<HIconListWidget*>(pList);
}

void HIconTreeWidget::initIconTypes()
{
    //最好把文件的读取都统一到datafile里面去
    //读取文件的内容需要修改成qt的
}

void HIconTreeWidget::treeItemClicked(QTreeWidgetItem* pItem,int column)
{
    HIconTreeWidgetItem* item = static_cast<HIconTreeWidgetItem*>(pItem);
    if(item && NULL != item->parent())
    {
        pIconListWidget->RefreshIconType(item->getIconItemType());
    }
}
