#ifndef HICONVIEWEDITOR_H
#define HICONVIEWEDITOR_H

#include <QWidget>
class HIconTabWidget;
class HIconTreeWidget;
class HIconListWidget;
class HIconViewEditor : public QWidget
{
public:
    HIconViewEditor(QWidget *parent = Q_NULLPTR, int nType = 0);

protected:
    HIconTabWidget* pIconTabWidget;
    HIconListWidget* pIconListWidget;
    HIconTreeWidget* pIconTreeWidget;

};

#endif // HICONVIEWEDITOR_H
