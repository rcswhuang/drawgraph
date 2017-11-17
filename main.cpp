#include "hgrapheditormainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include "hgrapheditormgr.h"
class HGraphEditorMgr;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/pixmaps/splash.png");

    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();

    HGraphEditorMgr* pMgr = new HGraphEditorMgr;
    // Loading station
    //pMgr->loadStation();
    splash->showMessage(QStringLiteral("加载数据库信息"));
    qApp->processEvents();

    //loading templates
    pMgr->loadTemplates();
    splash->showMessage(QStringLiteral("加载画面模板信息"));
    qApp->processEvents();

    //loading all graphs
    pMgr->loadGraphs();
    splash->showMessage(QStringLiteral("加载所有画面信息"));
    qApp->processEvents();


    HGraphEditorMainWindow* w = new HGraphEditorMainWindow(pMgr);
    w->show();
    splash->finish(w);
    return a.exec();
}
