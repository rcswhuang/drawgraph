#include "hgraphtreewidget.h"
#include <QContextMenuEvent>
#include "hgrapheditormgr.h"
#include "hgrapheditordoc.h"
#include "hgraph.h"
#include <QMenu>
#include <QInputDialog>
#include <QFileDialog>
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

HGraphTreeWidgetItem::~HGraphTreeWidgetItem()
{
    for(int i = 0; i < childCount();i++)
    {
        HGraphTreeWidgetItem* item = (HGraphTreeWidgetItem*)child(i);
        if(item)
        {
            delete item;
            item = NULL;
        }
    }
}

void HGraphTreeWidgetItem::setGraphTreeID(int graphTreeID)
{
    wGraphTreeID = graphTreeID;
}

int HGraphTreeWidgetItem::getGraphTreeID()
{
    return wGraphTreeID;
}



HGraphTreeWidget::HGraphTreeWidget(HGraphEditorMgr *pMgr)
    :pGraphEditorMgr(pMgr)
{
    nPreGraphID = (int)-1;
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
    rootItem->setText(0,QStringLiteral("厂站五防画面总览"));
    rootItem->setIcon(0,QIcon(":/images/Folder.png"));
    rootItem->setGraphTreeID(9999);
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
        HGraphTreeWidgetItem* newItem = new HGraphTreeWidgetItem(rootItem,GRAPHTREE_TYPE_FILE);
        newItem->setGraphTreeID(graph->getGraphID());
        newItem->setText(0,graph->getGraphName());
        newItem->setIcon(0,QIcon(":/images/Folder_Documents.png"));
        rootItem->addChild(newItem);
        HGraphTreeWidgetItem* fileItem = new HGraphTreeWidgetItem(newItem,GRAPHTREE_TYPE_CFILE);
        fileItem->setGraphTreeID(graph->getGraphID());
        fileItem->setIcon(0,QIcon(":/images/document-text.png"));
        fileItem->setText(0,QStringLiteral("最新版本"));
    }
    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(clickGraphItem(QTreeWidgetItem*,int)));
    //connect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),SLOT(changedGraphItem(QTreeWidgetItem*,QTreeWidgetItem*)));
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
    QAction *importFolderAct = new QAction(QStringLiteral("导入文件夹"),this);
    importFolderAct->setStatusTip(QStringLiteral("导入一个画面文件夹"));
    menu->addAction(importFolderAct);
    connect(importFolderAct,SIGNAL(triggered(bool)),this,SLOT(importFolderGraph()));
    QAction *importFileAct = new QAction(QStringLiteral("导入文件"),this);
    importFileAct->setStatusTip(QStringLiteral("导入一个画面文件"));
    menu->addAction(importFileAct);
    connect(importFileAct,SIGNAL(triggered(bool)),this,SLOT(importFileGraph()));
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

void HGraphTreeWidget::clickGraphItem(QTreeWidgetItem* item,int column)
{
    HGraphTreeWidgetItem* clickItem = dynamic_cast<HGraphTreeWidgetItem*>(item);
    int nCurGraphID = clickItem->getGraphTreeID();
    if(nCurGraphID != nPreGraphID)
    {
        if(GRAPHTREE_TYPE_FILE == clickItem->type());
        int count = topLevelItemCount();
        for(int k = 0; k < count;k++)
        {
            HGraphTreeWidgetItem* rootItem = static_cast<HGraphTreeWidgetItem*>(topLevelItem(k));
           if(!rootItem) continue;
           for(int i = 0;i < rootItem->childCount();i++)
           {
               HGraphTreeWidgetItem* childItem = static_cast<HGraphTreeWidgetItem*>(rootItem->child(i));
               collapseItem(childItem);
           }
       }
           if(GRAPHTREE_TYPE_ROOT == clickItem->type())
               expandItem(clickItem);
           else if(GRAPHTREE_TYPE_CFILE == clickItem->type())
           {
               if(clickItem->parent())
                expandItem(clickItem->parent());
           }
    }

    int type = clickItem->type();
    if(GRAPHTREE_TYPE_FILE == type)
    {
        //要切换到子文件里面
        HGraphTreeWidgetItem* childItem = dynamic_cast<HGraphTreeWidgetItem*>(clickItem->child(column));
        if(childItem && GRAPHTREE_TYPE_CFILE == childItem->type())
        {
            setCurrentItem(childItem);
        }
    }
    if(nCurGraphID == nPreGraphID) return;
    nPreGraphID = nCurGraphID;

    if(GRAPHTREE_TYPE_ROOT == clickItem->type())
        return;

    openGraph();
}

void HGraphTreeWidget::changedGraphItem(QTreeWidgetItem* cur,QTreeWidgetItem* pre)
{
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
    HGraphTreeWidgetItem* pCurItem = dynamic_cast<HGraphTreeWidgetItem*> (currentItem());
    if(!pCurItem) return;
    QString strGraphName = pCurItem->text(0);
    int nGraphID = pCurItem->getGraphTreeID();
    emit graphOpen(strGraphName,nGraphID);
}

void HGraphTreeWidget::delGraph()
{
    if(!pGraphEditorMgr)
        return;
    /*if(QMessageBox::Ok == QMessageBox::warning(NULL,QStringLiteral("删除画面"),QStringLiteral("确认删除此画面所有信息?"),QMessageBox::Ok|QMessageBox::Cancel))
    {
        HGraphTreeWidgetItem* item = (HGraphTreeWidgetItem*)currentItem();
        if(!item)
        {
            QMessageBox::warning(NULL,QStringLiteral("删除画面"),QStringLiteral("删除失败!"),QMessageBox::Ok);
            return;
        }
        //emit IconDel();
    }*/
    HGraphTreeWidgetItem* pCurItem = dynamic_cast<HGraphTreeWidgetItem*> (currentItem());
    QString strGraphName = pCurItem->text(0);
    int nGraphID = pCurItem->getGraphTreeID();
    emit graphDel(strGraphName,nGraphID);
}

void HGraphTreeWidget::saveAsGraph()
{

}

void HGraphTreeWidget::importFolderGraph()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("打开文件夹"),
                  "../",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if(dirPath.isEmpty() || dirPath.isNull())
        return;
    emit graphImportPath(dirPath);
}



void HGraphTreeWidget::importFileGraph()
{
    QString graphName = QFileDialog::getOpenFileName(this, QStringLiteral("打开文件"),"../","Files(*.grf)");
    if(graphName.isEmpty() || graphName.isNull())
        return;
    emit graphImport(graphName);
}
void HGraphTreeWidget::addGraphTreeWidgetItem()
{
    if(!pGraphEditorMgr)
        return;
    HGraphTreeWidgetItem *parentItem = (HGraphTreeWidgetItem*)currentItem();
    int type = parentItem->type();
    if(!parentItem || type > GRAPHTREE_TYPE_ROOT)
        return;

    HGraph* pGraph = pGraphEditorMgr->graphEditorDoc()->getCurGraph();

    HGraphTreeWidgetItem* newItem = new HGraphTreeWidgetItem(parentItem,GRAPHTREE_TYPE_FILE);
    newItem->setGraphTreeID(pGraph->getGraphID());
    newItem->setText(0,pGraph->getGraphName());
    newItem->setIcon(0,QIcon(":/images/Folder_Documents.png"));
    parentItem->addChild(newItem);
    expandItem(newItem);
    HGraphTreeWidgetItem* fileItem = new HGraphTreeWidgetItem(newItem,GRAPHTREE_TYPE_CFILE);
    fileItem->setGraphTreeID(pGraph->getGraphID());
    fileItem->setIcon(0,QIcon(":/images/document-text.png"));
    fileItem->setText(0,QStringLiteral("最新版本"));
    //setCurrentItem(fileItem);
    emit itemClicked(newItem,0);
}

void HGraphTreeWidget::delGraphTreeWidgetItem()
{
    if(!pGraphEditorMgr)
        return;
    HGraphTreeWidgetItem *curItem = (HGraphTreeWidgetItem*)currentItem();
    if(!curItem || curItem->type() != GRAPHTREE_TYPE_FILE) return;
    HGraphTreeWidgetItem *parentItem = (HGraphTreeWidgetItem*)curItem->parent();
    HGraphTreeWidgetItem* childItem = dynamic_cast<HGraphTreeWidgetItem*>(curItem->child(0));
    if(!childItem)
        return;
    parentItem->removeChild(curItem);
    delete curItem;
    curItem = NULL;
    setCurrentItem(parentItem);
}

void HGraphTreeWidget::importGraphTreeWigetItem()
{

}











