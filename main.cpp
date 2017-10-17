#include "hgrapheditormainwindow.h"
#include <QApplication>
#include "hgrapheditormgr.h"
class HGraphEditorMgr;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HGraphEditorMgr* pMgr = new HGraphEditorMgr;
    HGraphEditorMainWindow* w = new HGraphEditorMainWindow(pMgr);
    w->show();

    return a.exec();
}
