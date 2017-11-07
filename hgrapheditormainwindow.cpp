#include "hgrapheditormainwindow.h"
#include "ui_grapheditormainwindow.h"
#include "hgrapheditormgr.h"
#include "hicontabwidget.h"
#include "hiconvieweditor.h"
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>
#include "hfonthelper.h"
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

    pFontBox = new QComboBox(ui->fontBar);
    QStringList fontList = HFontHelper::Instance()->fontFamilies();

    for(int i = 0; i < fontList.count();i++)
        qDebug()<<fontList[i];

    pFontBox->insertItems(0,fontList);
    pFontBox->setCurrentIndex(0);
    ui->fontBar->insertWidget(ui->actionBold,pFontBox);

    pFontSizeBox = new QComboBox(ui->fontBar);
    pFontSizeBox->clear();
    QString strFont = pFontBox->currentText();
    //QString strStyle = HFontHelper::Instance()->styleString();
    QList<int> fontSize = HFontHelper::Instance()->pointSizes(strFont);
    QStringList styleList = HFontHelper::Instance()->styles(strFont);
    for(int i = 0;i < fontSize.count();i++)
    {
        QString str = QString("%1").arg(fontSize[i]);
        pFontSizeBox->insertItem(i,str,QVariant(fontSize[i]));
    }
    ui->fontBar->insertWidget(ui->actionBold,pFontSizeBox);

    //bool bBold = HFontHelper::Instance()->bold(strFont);
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
