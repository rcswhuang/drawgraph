#include "hrelaypage.h"
#include "ui_relaypage.h"

HRelayPage::HRelayPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HRelayPage)
{
    ui->setupUi(this);
}

HRelayPage::~HRelayPage()
{
    delete ui;
}
