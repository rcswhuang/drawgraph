#include "hgrapheditormainwindow.h"
#include "ui_grapheditormainwindow.h"

HGraphEditorMainWindow::HGraphEditorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphEditorMainWindow)
{
    ui->setupUi(this);
}

HGraphEditorMainWindow::~HGraphEditorMainWindow()
{
    delete ui;
}
