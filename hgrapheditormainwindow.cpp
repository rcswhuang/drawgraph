#include "hgrapheditormainwindow.h"
#include "ui_grapheditormainwindow.h"

HGraphEditorMainWindow::HGraphEditorMainWindow(HGraphEditorMgr *mgr,QWidget *parent) :
    pGraphEditorMgr(mgr),QMainWindow(parent),
    ui(new Ui::GraphEditorMainWindow)
{
    ui->setupUi(this);
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

}
