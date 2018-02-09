#ifndef HDATAPROPERTYDLG_H
#define HDATAPROPERTYDLG_H

#include <QDialog>
class HBaseObj;
class HIconComplexObj;
class HDigital;
namespace Ui {
class digitalPage;
}

class HDigitalPage : public QDialog
{
    Q_OBJECT

public:
    explicit HDigitalPage(QWidget *parent = 0);
    HDigitalPage(HBaseObj* pObj,QWidget *parent = 0);
    ~HDigitalPage();
public:
    void initDataProper();
    void initBaseProper();
    void setDigitalAttr(HDigital* pDigital);

public slots:
    void onStationCurrentIndexChanged(int index);
    void onIntervalCurrentIndexChanged(int index);
    void onOk();
    void onCancel();
private:
    Ui::digitalPage *ui;
    HIconComplexObj* pCurObj;
    ushort wStation;//厂站
    ushort wPoint;//测点
    ushort wAttrib;//属性
};

#endif // HDATAPROPERTYDLG_H
