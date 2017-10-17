#include "hiconvieweditor.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QTreeWidget>
#include "hiconlistwidget.h"
#include "hicontreewidget.h"
//Tab上的
HIconViewEditor::HIconViewEditor(QWidget *parent , int nType )
    :QWidget(parent)
{
    QVBoxLayout* pVBoxLayout = new QVBoxLayout(this);
    Q_ASSERT(pVBoxLayout);
    pVBoxLayout->setMargin(0);

    pIconTreeWidget = new HIconTreeWidget;
    pVBoxLayout->addWidget(pIconTreeWidget);
    pIconListWidget = new HIconListWidget(this);
    pVBoxLayout->addWidget(pIconListWidget);

    pIconTreeWidget->setIconListWidget(pIconListWidget);

}
