#include "hgrapheditormainwindow.h"
#include "ui_grapheditormainwindow.h"
#include "hgrapheditormgr.h"
#include "hicontabwidget.h"
#include "hiconvieweditor.h"
#include <QLineEdit>
#include <QComboBox>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QDebug>
HGraphEditorMainWindow::HGraphEditorMainWindow(HGraphEditorMgr *pMgr,QWidget *parent)
:QMainWindow (parent),
ui(new Ui::GraphEditorMainWindow)
{
    ui->setupUi(this);
    pGraphEditorMgr = pMgr,
    initMainWindow();
    createToolBars();
}

HGraphEditorMainWindow::~HGraphEditorMainWindow()
{
    delete ui;
}

void HGraphEditorMainWindow::createActions()
{

}

void HGraphEditorMainWindow::createToolBars()
{


   createFontBar();
}

void HGraphEditorMainWindow::createFontBar()
{

    //pFontBox = new QComboBox(ui->fontBar);
    QFontComboBox* pFontBox1 = new QFontComboBox(ui->fontBar);
    QFontDatabase database;
    QStringList fontList = database.families();

    for(int i = 0; i < fontList.count();i++)
        qDebug()<<fontList[i];

   // pFontBox->insertItems(0,fontList);
    ui->fontBar->insertWidget(ui->actionBold,pFontBox1);

    pFontSizeBox = new QComboBox(ui->fontBar);
    pFontSizeBox->clear();
    QString strFont = pFontBox1->currentFont().family();
    QList<int> fontSize = database.pointSizes(strFont);
    QStringList styleList = database.styles(strFont);
    for(int i = 0;i < fontSize.count();i++)
    {
        QString str = QString("%1").arg(fontSize[i]);
        pFontSizeBox->insertItem(i,str,QVariant(fontSize[i]));
    }
    ui->fontBar->insertWidget(ui->actionBold,pFontSizeBox);

    bool bBold = styleList.contains("Bold");
    ui->actionBold->setEnabled(bBold);
    //最后插入文本编辑框
    pTextEdit = new QLineEdit(ui->fontBar);
    ui->fontBar->addWidget(pTextEdit);
}

void HGraphEditorMainWindow::createStatusBar()
{

}

void HGraphEditorMainWindow::createDockWidget()
{

}

void HGraphEditorMainWindow::initGraphEditorMgr()
{

}

void HGraphEditorMainWindow::initMainWindow()
{
    pIconTabWidget = new HIconTabWidget(pGraphEditorMgr);
    //pIconTabWidget->setTabPosition(QTabWidget::West);
    //connect(pIconTabWidget,SIGNAL(iconSelected(const QString&,const QString&,const QString&,int)),SLOT(iconTemplateSelected(const QString&,const QString&,const QString&,int)));
    //pIconViewEditor = new HIconViewEditor(pIconTabWidget);
    //pIconTabWidget->removeTab(0);
    //pIconTabWidget->addTab(pIconViewEditor,QStringLiteral("图元"));

    ui->resDockWidget->setWidget(pIconTabWidget);
}
