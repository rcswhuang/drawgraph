#include "hgrapheditormainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HGraphEditorMainWindow w;
    w.show();

    return a.exec();
}
