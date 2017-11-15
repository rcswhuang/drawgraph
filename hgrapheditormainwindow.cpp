#include "hgrapheditormainwindow.h"
#include "ui_grapheditormainwindow.h"
#include "hgrapheditormgr.h"
#include "hgrapheditorview.h"
#include "hgrapheditordoc.h"
#include "hgrapheditorscene.h"
#include "hgraphtreewidget.h"
#include "hicontabwidget.h"
#include "hiconvieweditor.h"
#include "hfonthelper.h"
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QDebug>
HGraphEditorMainWindow::HGraphEditorMainWindow(HGraphEditorMgr *pMgr,QWidget *parent)
:QMainWindow (parent),
ui(new Ui::GraphEditorMainWindow)
{
    ui->setupUi(this);
    pGraphEditorMgr = pMgr,
    initMainWindow();

    createToolBars();
    createStatusBar();
    createActions();
    createDockWidget();
    showMaximized();
}

HGraphEditorMainWindow::~HGraphEditorMainWindow()
{
    delete ui;
}

void HGraphEditorMainWindow::createActions()
{

    //文件部分
    connect(ui->actionNew_N,SIGNAL(triggered(bool)),this,SLOT(actionNew_clicked()));
    connect(ui->actionOpen_O,SIGNAL(triggered(bool)),this,SLOT(actionOpen_clicked()));
    connect(ui->actionSave_S,SIGNAL(triggered(bool)),this,SLOT(actionSave_clicked()));
    connect(ui->actionSave_As,SIGNAL(triggered(bool)),this,SLOT(actionSaveAs_clicked()));
    connect(ui->actionPrint_P,SIGNAL(triggered(bool)),this,SLOT(actionPrint_clicked()));
    connect(ui->actionExit_E,SIGNAL(triggered(bool)),this,SLOT(actionExit_clicked()));

    //编辑部分
    connect(ui->actionUndo_U,SIGNAL(triggered(bool)),this,SLOT(acitonUndo_clicked()));
    connect(ui->actionRedo_R,SIGNAL(triggered(bool)),this,SLOT(actionRedo_clicked()));
    connect(ui->actionCopy_C,SIGNAL(triggered(bool)),this,SLOT(actionCut_clicked()));
    connect(ui->actionCopy_C,SIGNAL(triggered(bool)),this,SLOT(actionCopy_clicked()));
    connect(ui->actionPaste_V,SIGNAL(triggered(bool)),this,SLOT(actionPaste_clicked()));
    connect(ui->actionDelete,SIGNAL(triggered(bool)),this,SLOT(actionDelete_clicked()));

    //置顶置底
    connect(ui->actionTop_T,SIGNAL(triggered(bool)),this,SLOT(actionTop_clicked()));
    connect(ui->actionBottom_B,SIGNAL(triggered(bool)),this,SLOT(actionBottom_clicked()));

    //属性
    connect(ui->actionAttribute,SIGNAL(triggered(bool)),this,SLOT(actionAttribute_clicked()));

    //排列
    connect(ui->actionAlignTop,SIGNAL(triggered(bool)),this,SLOT(actionAlignTop_clicked()));
    connect(ui->actionAlignHCenter,SIGNAL(triggered(bool)),this,SLOT(actionAlignHCenter_clicked()));
    connect(ui->actionAlignBottom,SIGNAL(triggered(bool)),this,SLOT(actionAlignBottom_clicked()));
    connect(ui->actionAlignLeft,SIGNAL(triggered(bool)),this,SLOT(actionAlignLeft_clicked()));
    connect(ui->actionAlignVCenter,SIGNAL(triggered(bool)),this,SLOT(actionAlignVCenter_clicked()));
    connect(ui->actionAlignRight,SIGNAL(triggered(bool)),this,SLOT(actionAlignRight_clicked()));

    //旋转
    connect(ui->actionRotate,SIGNAL(triggered(bool)),this,SLOT(actionRotate_clicked()));
    connect(ui->actionFilpLeft,SIGNAL(triggered(bool)),this,SLOT(actionFilpLeft_clicked()));
    connect(ui->actionFlipRight,SIGNAL(triggered(bool)),this,SLOT(actionFlipRight_clicked()));
    connect(ui->actionFlipHorizon,SIGNAL(triggered(bool)),this,SLOT(actionFlipHorizon_clicked()));
    connect(ui->actionFlipHorizon,SIGNAL(triggered(bool)),this,SLOT(actionFlipVertical_clicked()));


    //字体部分的actions()
    connect(pFontBox,SIGNAL(currentIndexChanged(int)),this,SLOT(fontBox_clicked(int)));
    connect(pFontSizeBox,SIGNAL(currentIndexChanged(int)),this,SLOT(fontSizeBox_clicked(int)));
    connect(ui->actionBold,SIGNAL(triggered(bool)),this,SLOT(actionBold_clicked(bool)));
    connect(ui->actionItalic,SIGNAL(triggered(bool)),this,SLOT(actionItalic_clicked(bool)));
    connect(ui->actionTextUnder,SIGNAL(triggered(bool)),this,SLOT(actionTextUnder_clicked(bool)));

    //颜色部分
    connect(ui->actionColor,SIGNAL(triggered(bool)),this,SLOT(actionColor_clicked()));
    connect(ui->actionFillColor,SIGNAL(triggered(bool)),this,SLOT(actionFillColor_clicked()));
    connect(ui->actionBackPicture,SIGNAL(triggered(bool)),this,SLOT(actionBackPicture_clicked()));

    //组合
    connect(ui->actionGroup,SIGNAL(triggered(bool)),this,SLOT(actionGroup_clicked()));
    connect(ui->actionUngroup,SIGNAL(triggered(bool)),this,SLOT(actionUnGroup_clicked()));


    //选择,工具
    connect(ui->actionSelect,SIGNAL(triggered(bool)),this,SLOT(actionSelect_clicked()));
    connect(ui->actionText,SIGNAL(triggered(bool)),this,SLOT(actionText_clicked()));
    connect(ui->actionLine,SIGNAL(triggered(bool)),this,SLOT(actionLine_clicked()));
    connect(ui->actionRectangle,SIGNAL(triggered(bool)),this,SLOT(actionRectagle_clicked()));
    connect(ui->actionPolyline,SIGNAL(triggered(bool)),this,SLOT(actionPolyline_clicked()));
    connect(ui->actionPolygon,SIGNAL(triggered(bool)),this,SLOT(actionPolygon_clicked()));
    connect(ui->actionCircle,SIGNAL(triggered(bool)),this,SLOT(actionCircle_clicked()));
    connect(ui->actionArc,SIGNAL(triggered(bool)),this,SLOT(actionArc_clicked()));


    //缩放
    connect(ui->actionZoomin,SIGNAL(triggered(bool)),this,SLOT(actionZoomin_clicked()));
    connect(ui->actionZoomout,SIGNAL(triggered(bool)),this,SLOT(actionZoomout_clicked()));
    connect(ui->actionZoom,SIGNAL(triggered(bool)),this,SLOT(actionZoom_clicked()));
    connect(ui->actionZoomSame,SIGNAL(triggered(bool)),this,SLOT(actionZoomSame_clicked()));


    //等比
    connect(ui->actionSameWidth,SIGNAL(triggered(bool)),this,SLOT(actionSameWidth_clicked()));
    connect(ui->actionSameHeight,SIGNAL(triggered(bool)),this,SLOT(actionSameHeight_clicked()));
    connect(ui->actionSame,SIGNAL(triggered(bool)),this,SLOT(actionSame_clicked()));
    connect(ui->actionHSameSpace,SIGNAL(triggered(bool)),this,SLOT(actionHSameSpace_clicked()));
    connect(ui->actionVSameSpace,SIGNAL(triggered(bool)),this,SLOT(actionVSameSpace_clicked()));

    //关于
    connect(ui->actionAbout_A,SIGNAL(triggered(bool)),this,SLOT(about()));
}

void HGraphEditorMainWindow::createToolBars()
{

   //字体 默认的字体都是可以Bold italic
   pFontBox = new QComboBox(ui->fontBar);
   QStringList fontList = HFontHelper::Instance()->fontFamilies();
   pFontBox->insertItems(0,fontList);
   pFontBox->setCurrentIndex(0);
   ui->fontBar->insertWidget(ui->actionBold,pFontBox);

   //大小
   pFontSizeBox = new QComboBox(ui->fontBar);
   pFontSizeBox->clear();
   QString strFont = pFontBox->currentText();
   QList<int> fontSize = HFontHelper::Instance()->pointSizes(strFont);
   for(int i = 0;i < fontSize.count();i++)
   {
       QString str = QString("%1").arg(fontSize[i]);
       pFontSizeBox->insertItem(i,str,QVariant(fontSize[i]));
   }
   ui->fontBar->insertWidget(ui->actionBold,pFontSizeBox);

   //最后插入文本编辑框
   pTextEdit = new QLineEdit(ui->fontBar);
   ui->fontBar->addWidget(pTextEdit);
}

void HGraphEditorMainWindow::createFontBar()
{

}

void HGraphEditorMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void HGraphEditorMainWindow::createDockWidget()
{
    pIconTabWidget = new HIconTabWidget(pGraphEditorMgr);
    //connect(pIconTabWidget,SIGNAL(iconSelected(const QString&,const QString&,const QString&,int)),SLOT(iconTemplateSelected(const QString&,const QString&,const QString&,int)));
    ui->resDockWidget->setWidget(pIconTabWidget);

    pGraphTreeWidget = new HGraphTreeWidget(pGraphEditorMgr);
    ui->fileDockWidget->setWidget(pGraphTreeWidget);
    connect(pGraphTreeWidget,SIGNAL(graphNew(const QString&)),this,SLOT(New(const QString&)));//新建
    connect(pGraphTreeWidget,SIGNAL(graphOpen(const QString&,const int)),this,SLOT(Open(const QString&,const int)));//打开
    connect(pGraphTreeWidget,SIGNAL(graphDel(const QString&,const int )),this,SLOT(Del(const QString&,const int)));//删除
}

void HGraphEditorMainWindow::initGraphEditorMgr()
{

}

void HGraphEditorMainWindow::initMainWindow()
{
    pGraphEditorView = new HGraphEditorView(ui->centralWidget);
    pGraphEditorView->setObjectName(QStringLiteral("画图系统"));
    pGraphEditorView->setFrameShape(QFrame::NoFrame);
    pGraphEditorView->setFrameShadow(QFrame::Plain);
    pGraphEditorView->setLineWidth(0);

    ui->gridLayout->addWidget(pGraphEditorView,0,1,1,1);
    pGraphEditorMgr->setGraphEditorView(pGraphEditorView);

}

void HGraphEditorMainWindow::New(const QString& graphName)
{
    //主要要先保存，然后删除当前的，然后新建graph对象(mgr->doc)，然后树新增
    if(!pGraphEditorMgr)
        return;
    bool bfind = pGraphEditorMgr->findGraphByName(graphName);
    if(bfind)
    {
        QMessageBox::information(this,QStringLiteral("提醒"),QStringLiteral("已经存在相同名字的图形文件，请修改名称"),QMessageBox::Ok);
        return;
    }

    //如果有修改的
    if(pGraphEditorMgr->isGraphModify())
    {
        if(QMessageBox::Ok == QMessageBox::information(NULL,QStringLiteral("提醒"),QStringLiteral("需要保存当前的画面文件吗？"),QMessageBox::Yes|QMessageBox::No))
        {

             //Save();
        }
    }

    //view 或者 scene里面要清除掉所有内容
    pGraphEditorMgr->New(graphName);
    pGraphTreeWidget->addGraphTreeWidgetItem();
}

void HGraphEditorMainWindow::Open(const QString&,const int graphID)
{

}

void HGraphEditorMainWindow::Del(const QString&,const int graphID)
{

}


