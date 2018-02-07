#ifndef HDATAPROPERTYDLG_H
#define HDATAPROPERTYDLG_H

#include <QDialog>
class HBaseObj;
namespace Ui {
class DataPropertyDlg;
}

class HDataPropertyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HDataPropertyDlg(QWidget *parent = 0);
    HDataPropertyDlg(HBaseObj* pObj,QWidget *parent = 0);
    ~HDataPropertyDlg();
public:
    void initDataProperDlg();

private:
    Ui::DataPropertyDlg *ui;
    HBaseObj* pCurObj;
};

#endif // HDATAPROPERTYDLG_H
