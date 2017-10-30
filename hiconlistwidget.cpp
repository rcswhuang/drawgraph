#include "hiconlistwidget.h"
#include <QListWidgetItem>
#include "hgrapheditormgr.h"
#include "hgrapheditordoc.h"
#include "H5IconGui/hicontemplate.h"
HIconListWidgetItem::HIconListWidgetItem(QListWidget * parent , int type )
    :QListWidgetItem(parent,type)
{
}


HIconListWidget::HIconListWidget(HGraphEditorMgr* mgr)
    :pGraphEditorMgr(mgr)
{

}

HIconListWidget::~HIconListWidget()
{

}


void HIconListWidget::initIconListWidget()
{
    clearIconListWidget();
    //初始化默认读取所有模板文件
    nType = TEMPLATE_TYPE_DIGITAL;
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
    if(nType == ntype)
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
