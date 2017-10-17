#include "hiconlistwidget.h"
#include <QListWidgetItem>

HIconListWidgetItem::HIconListWidgetItem(QListWidget * parent , int type )
    :QListWidgetItem(parent,type)
{
}


HIconListWidget::HIconListWidget(QWidget* parent)
    :QListWidget(parent)
{

}

HIconListWidget::~HIconListWidget()
{

}


void HIconListWidget::initIconListWidget()
{

}

void HIconListWidget::clearIconListWidget()//切换需要先清除
{

}

void HIconListWidget::RefreshIconType(int ntype)
{

}
