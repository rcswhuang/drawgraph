#ifndef HGRAPHEDITORMAINWINDOW_H
#define HGRAPHEDITORMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class GraphEditorMainWindow;
}

class HGraphEditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HGraphEditorMainWindow(QWidget *parent = 0);
    ~HGraphEditorMainWindow();

private:
    Ui::GraphEditorMainWindow *ui;
};

#endif // HGRAPHEDITORMAINWINDOW_H
