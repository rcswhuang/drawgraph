#include "hgraphtreewidget.h"
#include <QContextMenuEvent>
#include "hgrapheditormgr.h"
#include "hgrapheditordoc.h"
#include "hgraph.h"
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>

#define GRAPHTREE_TYPE_ROOT  0
#define GRAPHTREE_TYPE_FILE  1
#define GRAPHTREE_TYPE_CFILE 2

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
    setHeaderLabel(QStringLiteral("画面"));
    initGraphTreeWidget();
}

HGraphTreeWidget::~HGraphTreeWidget()
{

}

void HGraphTreeWidget::initGraphTreeWidget()
{
    HGraphTreeWidgetItem* rootItem = new HGraphTreeWidgetItem(this,GRAPHTREE_TYPE_ROOT);
    rootItem->setText(0,QStringLiteral("wfSystem"));
    rootItem->setIcon(0,QIcon(":/images/Folder.png"));
    addTopLevelItem(rootItem);
    //expandItem(rootItem);

    HGraphEditorDoc* doc = pGraphEditorMgr->graphEditorDoc();
    if(!doc) return;
    QList<HGraph*> graphList = doc->getGraphList();
    for(int i = 0; i < graphList.count();i++)
    {
        HGraph* graph = (HGraph*)graphList[i];
        if(!graph)
            continue;
        //触发此item会引发文件打开的动作
        HGraphTreeWidgetItem* item = new HGraphTreeWidgetItem(rootItem,GRAPHTREE_TYPE_FILE);
        item->setGraphTreeID((int)graph->getGraphID());
        item->setText(0,graph->getGraphName());      
        //HGraphTreeWidgetItem* citem = new HGraphTreeWidgetItem(item,GRAPHTREE_TYPE_CFILE)
    }
}

void HGraphTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint point = event->pos();
    HGraphTreeWidgetItem* curItem = (HGraphTreeWidgetItem*)itemAt(point);
    if(!curItem)
        return;
    int n = curItem->type();
    if(curItem->type() == GRAPHTREE_TYPE_ROOT)
    {
        initGraphMenu(event);
    }
    else if(curItem->type() == GRAPHTREE_TYPE_FILE)
    {
        intGraphFileMenu(event);
    }
}

void HGraphTreeWidget::initGraphMenu(QContextMenuEvent* event)
{
    QMenu* menu = new QMenu;
    QAction *newAct = new QAction(QStringLiteral("新建画面"),this);
    newAct->setStatusTip(QStringLiteral("新建一个画面文件"));
    menu->addAction(newAct);
    connect(newAct,SIGNAL(triggered(bool)),SLOT(newGraph()));
    menu->addSeparator();
    QAction *importAct = new QAction(QStringLiteral("导入文件夹"),this);
    importAct->setStatusTip(QStringLiteral("导入一个画面文件"));
    menu->addAction(importAct);

    menu->popup(event->globalPos());
}

//画面右键菜单
void HGraphTreeWidget::intGraphFileMenu(QContextMenuEvent* event)
{
    QMenu* menu = new QMenu;
    QAction *delAct = new QAction(QStringLiteral("删除画面"),this);
    delAct->setStatusTip(QStringLiteral("删除一个画面文件"));
    menu->addAction(delAct);
    connect(delAct,SIGNAL(triggered(bool)),SLOT(delGraph()));
    menu->addSeparator();
    QAction *saveAsAct = new QAction(QStringLiteral("另存文件为"),this);
    saveAsAct->setStatusTip(QStringLiteral("另存一个画面文件"));
    menu->addAction(saveAsAct);

    menu->popup(event->globalPos());
}


void HGraphTreeWidget::newGraph()
{
    bool ok;
    QString strGraphName = QInputDialog::getText(this,QStringLiteral("输入画面的名称"),QStringLiteral("画面名称:"),QLineEdit::Normal,"",&ok);
    if(!ok) return;
    emit graphNew(strGraphName);
}

void HGraphTreeWidget::openGraph()
{

}

void HGraphTreeWidget::delGraph()
{
    if(!pGraphEditorMgr)
        return;
    if(QMessageBox::Ok == QMessageBox::warning(NULL,QStringLiteral("删除画面"),QStringLiteral("确认删除此画面所有信息?"),QMessageBox::Ok|QMessageBox::Cancel))
    {
        HGraphTreeWidgetItem* item = (HGraphTreeWidgetItem*)currentItem();
        if(!item)
        {
            QMessageBox::warning(NULL,QStringLiteral("删除画面"),QStringLiteral("删除失败!"),QMessageBox::Ok);
            return;
        }
        //emit IconDel();
    }
}

void HGraphTreeWidget::saveAsGraph()
{

}

void HGraphTreeWidget::importGraph()
{

}


void HGraphTreeWidget::addGraphTreeWidgetItem()
{

}

void HGraphTreeWidget::delGraphTreeWidgetItem()
{

}

void HGraphTreeWidget::initGraphTreeWigetItem()
{

}











