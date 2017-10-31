#include "hgrapheditormainwindow.h"
#include "ui_grapheditormainwindow.h"
#include "hgrapheditormgr.h"
#include "hicontabwidget.h"
#include "hiconvieweditor.h"
HGraphEditorMainWindow::HGraphEditorMainWindow(HGraphEditorMgr *pMgr,QWidget *parent)
:pGraphEditorMgr(pMgr),QMainWindow (parent),
ui(new Ui::GraphEditorMainWindow)
{
    ui->setupUi(this);
    initMainWindow();
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
    connect(pIconTabWidget,SIGNAL(iconSelected(const QString&,const QString&,const QString&,int)),SLOT(iconTemplateSelected(const QString&,const QString&,const QString&,int)));
    //pIconViewEditor = new HIconViewEditor(pIconTabWidget);
    //pIconTabWidget->removeTab(0);
    //pIconTabWidget->addTab(pIconViewEditor,QStringLiteral("图元"));

    ui->resDockWidget->setWidget(pIconTabWidget);
}
