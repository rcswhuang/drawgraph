#include "hdrawgraphmainwindow.h"
#include "ui_hdrawgraphmainwindow.h"

HDrawGraphMainWindow::HDrawGraphMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HDrawGraphMainWindow)
{
    ui->setupUi(this);
}

HDrawGraphMainWindow::~HDrawGraphMainWindow()
{
    delete ui;
}
