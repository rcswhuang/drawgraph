#ifndef HGRAPHEDITOROP_H
#define HGRAPHEDITOROP_H

#include <QObject>
#include "hiconapi.h"
class HGraphEditorMgr;
class HGraphEditorOp
{
public:
    HGraphEditorOp(HGraphEditorMgr* mgr);

public:
    void cut();

    void copy();

    void paste();

    void del();

    QString getClipboardFile();

    //移动到顶层
    void bringToTop();

    //移动到底层
    void bringToBottom();

    //组合
    void groupObj();

    //取消组合
    void ungroupObj();

    //左对齐
    void alignLeft();

    //右对齐
    void alignRight();

    //水平居中
    void alignHCenter();

    //上对齐
    void alignTop();

    //下对齐
    void alignBottom();

    //垂直居中
    void alignVCenter();

    //不同对齐方式不同点
    bool getbenchMarkPoint(QPointF& pt);

    //对齐
    void alignAlgorithm();

    //等宽
    void sizeEqualWidth();

    //等高
    void sizeEqualHeight();

    //完全相等
    void sizeEqualComplete();

    //横向等间距
    void sizeHEqualSpace();

    //纵向等间距
    void sizeVEqualSpace();

    //相等数据获取
    bool getbenchMarkSize(QPointF& pt);

    //相等关系算法
    void equalAlgorithm();

    //逆时针翻转90
    void flipLeft90();

    //顺时针翻转90
    void flipRight90();

    //水平翻转
    void flipHorizon();

    //垂直翻转
    void flipVertical();

    //旋转算法
    void flipAlgorithm();

    //放大
    void zoomIn();

    //缩小
    void zoomOut();

    //自由缩放
    void zoom();

    //原始大小
    void zoomSame();

    void setupMatrix();

private:
    HGraphEditorMgr* m_pGraphEditorMgr;
    Qt::Alignment m_Alignment;
    IconSize m_Equalway;
    IconFlip m_Flipway;
    qreal m_scale;

};

#endif // HGRAPHEDITOROP_H
