#include "hanaloguepage.h"
#include "ui_analoguepage.h"

HAnaloguePage::HAnaloguePage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HAnaloguePage)
{
    ui->setupUi(this);
}

HAnaloguePage::~HAnaloguePage()
{
    delete ui;
}
