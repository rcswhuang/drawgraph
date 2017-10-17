#include "hgraphtreewidget.h"
#include <QContextMenuEvent>
HGraphTreeWidgetItem::HGraphTreeWidgetItem(QTreeWidget * parent, int type ):QTreeWidgetItem(parent,type)
{

}

HGraphTreeWidgetItem::HGraphTreeWidgetItem(QTreeWidgetItem * parent, int type)
    :QTreeWidgetItem (parent,type)
{

}

void HGraphTreeWidgetItem::setGraphTreeID(unsigned short graphTreeID)
{
    wGraphTreeID = graphTreeID;
}

unsigned short HGraphTreeWidgetItem::getGraphTreeID()
{
    return wGraphTreeID;
}



HGraphTreeWidget::HGraphTreeWidget(HGraphEditorMgr *pMgr)
{

}

HGraphTreeWidget::~HGraphTreeWidget()
{

}

void HGraphTreeWidget::initGraphTreeWidget()
{

}

void HGraphTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{

}
