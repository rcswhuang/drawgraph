#ifndef HRELAYPAGE_H
#define HRELAYPAGE_H

#include <QDialog>
class QListWidgetItem;
class HBaseObj;
class HIconComplexObj;
class HRelay;
namespace Ui {
class relayPage;
}

class HRelayPage : public QDialog
{
    Q_OBJECT

public:
    explicit HRelayPage(QWidget *parent = 0);
    HRelayPage(HBaseObj* pObj,QWidget *parent = 0);
    ~HRelayPage();

    void initBaseProperty();
    void initRelayPorperty();
    void createIcons();
    QIcon createBrushStyleIcon(Qt::BrushStyle brushStyle);

public slots:
    void onTextClrBtn_clicked();
    void onFontBtn_clicked();
    void onMoreBtn_clicked();
    void onStyleCombo_clicked(int index);
    void onNoFillRadio_clicked();
    void onClrFillRadio_clicked();
    void onPicFillRadio_clicked();
    //void onTransHSlider_clicked();
    //void onTransSpinBox_clicked();
    //void onFillStyleCombo_clicked(int index);
    void onFillClrBtn_clicked();
    void onPicSelect_clicked();
    //void onAlignPicCombo_clicked(int index);
    void changePage(QListWidgetItem* curItem,QListWidgetItem* preItem);

private:
    Ui::relayPage *ui;
    HIconComplexObj* pCurObj;
    QFont font;
    QString strTextColor;
    QString strBgColor;
    QString strFillColor;
    ushort wStation;//厂站
    ushort wPoint;//测点
    ushort wAttrib;//属性
};

#endif // HRELAYPAGE_H
