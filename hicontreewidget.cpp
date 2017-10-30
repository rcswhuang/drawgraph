#include "hicontreewidget.h"
#include <QListWidget>
#include <QTreeWidgetItem>
#include "hiconapi.h"
#include "hiconlistwidget.h"
#include "hgrapheditormgr.h"

HIconTreeWidgetItem::HIconTreeWidgetItem(QTreeWidgetItem* parent,int type ):QTreeWidgetItem(parent,type)
{

}

HIconTreeWidgetItem::HIconTreeWidgetItem(QTreeWidget* parnet)
    :QTreeWidgetItem (parnet)
{

}

int HIconTreeWidgetItem::getIconItemType()
{
    return type();
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
    HIconTreeWidgetItem* rootItem = new HIconTreeWidgetItem(this);
    rootItem->setText(0,QStringLiteral("测点"));
    addTopLevelItem(rootItem);
    expandItem(rootItem);
    HIconTreeWidgetItem *digitalItem = new HIconTreeWidgetItem(rootItem,TEMPLATE_TYPE_DIGITAL);
    //digitalItem->setIcon(0,QIcon(":/images/tree.png"));
    digitalItem->setText(0,QStringLiteral("遥信"));

    HIconTreeWidgetItem *analogueItem = new HIconTreeWidgetItem(rootItem,TEMPLATE_TYPE_ANALGOUE);
    //analogueItem->setIcon(0,QIcon(":/images/tree.png"));
    analogueItem->setText(0,QStringLiteral("遥测"));

    HIconTreeWidgetItem *controlItem = new HIconTreeWidgetItem(rootItem,TEMPLATE_TYPE_YK);
    //controlItem->setIcon(0,QIcon(":/images/tree.png"));
    controlItem->setText(0,QStringLiteral("遥控"));

    HIconTreeWidgetItem *pulseItem = new HIconTreeWidgetItem(rootItem,TEMPLATE_TYPE_PLUSE);
    //pulseItem->setIcon(0,QIcon(":/images/tree.png"));
    pulseItem->setText(0,QStringLiteral("遥脉"));

    HIconTreeWidgetItem *tapItem = new HIconTreeWidgetItem(rootItem,TEMPLATE_TYPE_TAP);
    //tapItem->setIcon(0,QIcon(":/images/tree.png"));
    tapItem->setText(0,QStringLiteral("档位"));

    HIconTreeWidgetItem *lightItem = new HIconTreeWidgetItem(rootItem,TEMPLATE_TYPE_LIGHT);
    //lightItem->setIcon(0,QIcon(":/images/tree.png"));
    lightItem->setText(0,QStringLiteral("光字牌"));

    HIconTreeWidgetItem *paiItem = new HIconTreeWidgetItem(rootItem,TEMPLATE_TYPE_JDPAI);
    //paiItem->setIcon(0,QIcon(":/images/tree.png"));
    paiItem->setText(0,QStringLiteral("接地牌"));

    //connect(this,SIGNAL(itemSelectionChanged()),SLOT(openIcon()));
    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(treeItemClicked(QTreeWidgetItem*,int)));

}

void HIconTreeWidget::treeItemClicked(QTreeWidgetItem* pItem,int column)
{
    HIconTreeWidgetItem* item = static_cast<HIconTreeWidgetItem*>(pItem);
    if(item && NULL != item->parent())
    {
        pIconListWidget->RefreshIconType(item->getIconItemType());
    }
}
