#include "hiconvieweditor.h"
#include <QVBoxLayout>
#include <QDrag>
#include <QMimeData>
#include <QDragMoveEvent>
#include "hiconapi.h" //图元api 后续要统一到一个api头文件中去
#include "hicontabwidget.h"
#include "hgrapheditormgr.h"
#include "hgrapheditordoc.h"
#include "hicontemplate.h"
////////////////////////////////////树型控件////////////////////////////////
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

HIconTreeWidget::HIconTreeWidget(HGraphEditorMgr* mgr)
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


///////////////////////////////列表控件/////////////////////////////////////////////////////

HIconListWidgetItem::HIconListWidgetItem(QListWidget * parent , int type )
    :QListWidgetItem(parent,type)
{
}


HIconListWidget::HIconListWidget(HGraphEditorMgr* mgr)
    :pGraphEditorMgr(mgr)
{
    initIconListWidget();
}

HIconListWidget::~HIconListWidget()
{
    clearIconListWidget();
}


void HIconListWidget::initIconListWidget()
{
    setResizeMode(QListView::Adjust);
    setAcceptDrops(true);//放
    setDragEnabled(true);//拖
    setDropIndicatorShown(true);
    setIconSize(QSize(50,50));

    clearIconListWidget();
    //初始化默认读取所有模板文件
    nType = TEMPLATE_TYPE_NULL;
    RefreshIconType(nType);
}

void HIconListWidget::clearIconListWidget()//切换需要先清除
{
    for(int i = 0; i < count();i++)
    {
        HIconTemplate* item = (HIconTemplate*)takeItem(i);
        if(item)
        {
            delete item;
            item = NULL;
        }
    }
    clear();
}

void HIconListWidget::RefreshIconType(int ntype)
{
    if(nType == ntype || TEMPLATE_TYPE_NULL == ntype)
        return;
    nType = ntype;
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorDoc())
        return;
    QList<HIconTemplate*> pList = pGraphEditorMgr->graphEditorDoc()->getIconTemplateList();

    for(int i=0;i < pList.count();i++)
    {
        HIconTemplate* pTemp = (HIconTemplate*)pList[i];
        if(pTemp->getCatalogType() == ntype)
        {
            HIconListWidgetItem* listItem = new HIconListWidgetItem(this,ntype);
            listItem->setData(Qt::UserRole,pTemp->getUuid());//uuid
            listItem->setText(pTemp->getCatalogName());//名字
            listItem->setIcon(pTemp->getIcon());
            addItem(listItem);
        }
    }
}

void HIconListWidget::startDrag(Qt::DropActions supportedActions)
{
    HIconListWidgetItem* item = dynamic_cast<HIconListWidgetItem*>(currentItem());
    QByteArray itemData;//数组
    QDataStream dataStream(&itemData,QIODevice::WriteOnly);//读数据到数组的流
    //uuid,name,type uuid,红绿灯,遥信
    dataStream<<item->data(Qt::UserRole).toString()<<item->text()<<nType; //读入数据到数组
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("DragIcon",itemData);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}

void HIconListWidget::dragLeaveEvent(QDragLeaveEvent *e)
{

}

void HIconListWidget::dragMoveEvent(QDragMoveEvent *e)
{
    if(e->mimeData()->hasFormat("DragIcon"))
    {
        e->setDropAction(Qt::MoveAction);
        e->accept();
    }
    else
        e->ignore();
}

void HIconListWidget::dropEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasFormat("DragIcon"))
        e->accept();
    else
        e->ignore();
}

///////////////////////////////////////////视图控件///////////////////////////////////////////////////////////////////
//Tab上的
/*
 * 树型窗口显示所有模板图符的名称信息，树型里面的类型都是按照模板里面区分的(包含遥信类，遥测类......)
 * 模板的名称都是从doc类里面来的,只要显示模板的名称就可以了。
 *
 * 列表窗口显示对应树型类型里面的所有图符信息
*/
HIconViewEditor::HIconViewEditor(HIconTabWidget *tabWidget )
    :pIconTabWidget(tabWidget)
{
    QVBoxLayout* pVBoxLayout = new QVBoxLayout(this);
    Q_ASSERT(pVBoxLayout);
    pVBoxLayout->setMargin(0);


    pIconTreeWidget = new HIconTreeWidget((HGraphEditorMgr*)pIconTabWidget->getGraphEditorMgr());   // (pIconTabWidget->getGraphEditorMgr());
    pVBoxLayout->addWidget(pIconTreeWidget);
    pIconListWidget = new HIconListWidget((HGraphEditorMgr*)pIconTabWidget->getGraphEditorMgr());
    pVBoxLayout->addWidget(pIconListWidget);

    pIconTreeWidget->setIconListWidget(pIconListWidget);
}


void HIconViewEditor::initIconViewEditor()
{

}
