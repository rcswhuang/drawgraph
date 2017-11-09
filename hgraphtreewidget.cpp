#include "hgraphtreewidget.h"
#include <QContextMenuEvent>
#include "hgrapheditormgr.h"
#include "hgrapheditordoc.h"
#include "hgraph.h"
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
    :pGraphEditorMgr(pMgr)
{
    setRootIsDecorated(true);
    setHeaderLabel(QStringLiteral("画面列表"));
    initGraphTreeWidget();
}

HGraphTreeWidget::~HGraphTreeWidget()
{

}

void HGraphTreeWidget::initGraphTreeWidget()
{
    HGraphTreeWidgetItem* rootItem = new HGraphTreeWidgetItem(this);
    rootItem->setText(0,QStringLiteral("wfSystem"));
    rootItem->setIcon(0,QIcon(":/images/Folder.png"));
    addTopLevelItem(rootItem);
    expandItem(rootItem);

    HGraphEditorDoc* doc = pGraphEditorMgr->graphEditorDoc();
    if(!doc) return;
    QList<HGraph*> graphList = doc->getGraphList();
    for(int i = 0; i < graphList.count();i++)
    {
        HGraph* graph = (HGraph*)graphList[i];
        if(!graph)
            continue;
        HGraphTreeWidgetItem* item = new HGraphTreeWidgetItem(rootItem);
        item->setGraphTreeID((int)graph->getGraphID());
        item->setText(0,graph->getGraphName());
    }
}

void HGraphTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{

}
