#include "hicontreewidget.h"
#include <QListWidget>
#include <QTreeWidgetItem>
#include "hiconlistwidget.h"

HIconTreeWidgetItem::HIconTreeWidgetItem(QTreeWidget* parent,int type )
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

HIconTreeWidget::HIconTreeWidget(QWidget* parent)
    :QTreeWidget (parent)
{
    initIconTypes();

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(treeItemClicked(QTreeWidgetItem*,int)));
    pIconListWidget = NULL;
}

void HIconTreeWidget::setIconListWidget(QListWidget* pList)
{
    pIconListWidget = static_cast<HIconListWidget*>(pList);
}


void HIconTreeWidget::treeItemClicked(QTreeWidgetItem* pItem,int column)
{
    HIconTreeWidgetItem* item = static_cast<HIconTreeWidgetItem*>(pItem);
    if(item && NULL != item->parent())
    {
        pIconListWidget->RefreshIconType(item->getIconItemType());
    }
}
