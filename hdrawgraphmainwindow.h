#ifndef HDRAWGRAPHMAINWINDOW_H
#define HDRAWGRAPHMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class HDrawGraphMainWindow;
}

class HDrawGraphMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HDrawGraphMainWindow(QWidget *parent = 0);
    ~HDrawGraphMainWindow();

private:
    Ui::HDrawGraphMainWindow *ui;
};

#endif // HDRAWGRAPHMAINWINDOW_H
