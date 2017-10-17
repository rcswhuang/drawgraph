#include "hgraphtreewidget.h"
#include <QTreeWidgetItem>
/*HGraphTreeWidgetItem::HGraphTreeWidgetItem(QTreeWidget * parent, int type )
{

}*/

HGraphTreeWidgetItem::HGraphTreeWidgetItem(HGraphTreeWidgetItem * parent, int type)
    :HGraphTreeWidgetItem (parent)
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
