#include "hdrawgraphmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HDrawGraphMainWindow w;
    w.show();

    return a.exec();
}
