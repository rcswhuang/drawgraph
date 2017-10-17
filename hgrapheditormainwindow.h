#ifndef HGRAPHEDITORMAINWINDOW_H
#define HGRAPHEDITORMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class GraphEditorMainWindow;
}
class HGraphEditorMgr;
class HGraphTreeWidget;
class HIconTabWidget;
class HGraphEditorDoc;
class HIconViewEditor;
class HGraphEditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    HGraphEditorMainWindow(HGraphEditorMgr *mgr,QWidget *parent = Q_NULLPTR);
    ~HGraphEditorMainWindow();

public:
    void createActions();
    void createToolBars();
    void createStatusBar();
    void createDockWidget();
    void initGraphEditorMgr();
    void initMainWindow();
private:
    HGraphEditorMgr *pGraphEditorMgr;
    HGraphEditorDoc* pGraphEditorDoc;
    HGraphTreeWidget* pGraphTreeWiget;//左边树结构
    HIconTabWidget* pIconTabWidget;//右边tab结构
    HIconViewEditor* pIconViewEditor;//tab上面的view
    Ui::GraphEditorMainWindow *ui;
};

#endif // HGRAPHEDITORMAINWINDOW_H
