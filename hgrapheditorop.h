#ifndef HGRAPHEDITOROP_H
#define HGRAPHEDITOROP_H

#include <QObject>
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

    void alignAlgorithm();

private:
    HGraphEditorMgr* m_pGraphEditorMgr;
    Qt::Alignment m_Alignment;
};

#endif // HGRAPHEDITOROP_H
