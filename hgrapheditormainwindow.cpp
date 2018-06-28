#if defined(_MSC_VER) &&(_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif
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
#include <QFileInfo>
#include <QComboBox>
#include <QMessageBox>
#include <QDir>
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
    setWindowTitle("测试界面");
}

HGraphEditorMainWindow::~HGraphEditorMainWindow()
{
    delete ui;
}

void HGraphEditorMainWindow::createActions()
{

    m_pAlignGroup = new QActionGroup(this);
    m_pEqualGroup = new QActionGroup(this);
    m_pDivideGroup = new QActionGroup(this);
    m_pFlipGroup = new QActionGroup(this);
    m_pEditAndBringGroup = new QActionGroup(this);
    m_pEditAndBringGroup->setEnabled(false);
    //文件部分
    connect(ui->actionNew_N,SIGNAL(triggered(bool)),this,SLOT(actionNew_clicked()));
    connect(ui->actionOpen_O,SIGNAL(triggered(bool)),this,SLOT(actionOpen_clicked()));
    connect(ui->actionSave_S,SIGNAL(triggered(bool)),this,SLOT(actionSave_clicked()));
    connect(ui->actionSave_As,SIGNAL(triggered(bool)),this,SLOT(actionSaveAs_clicked()));
    connect(ui->actionPrint_P,SIGNAL(triggered(bool)),this,SLOT(actionPrint_clicked()));
    connect(ui->actionExit_E,SIGNAL(triggered(bool)),this,SLOT(actionExit_clicked()));

    //undo redo部分
    connect(ui->actionUndo_U,SIGNAL(triggered(bool)),this,SLOT(acitonUndo_clicked()));
    connect(ui->actionRedo_R,SIGNAL(triggered(bool)),this,SLOT(actionRedo_clicked()));

    //编辑部分
    connect(ui->actionCut_X,SIGNAL(triggered(bool)),this,SLOT(actionCut_clicked()));
    connect(ui->actionCopy_C,SIGNAL(triggered(bool)),this,SLOT(actionCopy_clicked()));
    connect(ui->actionPaste_V,SIGNAL(triggered(bool)),this,SLOT(actionPaste_clicked()));
    connect(ui->actionDelete,SIGNAL(triggered(bool)),this,SLOT(actionDelete_clicked()));
    m_pEditAndBringGroup->addAction(ui->actionCut_X);
    m_pEditAndBringGroup->addAction(ui->actionCopy_C);
    //m_pEditAndBringGroup->addAction(ui->actionPaste_V);
    //ui->actionPaste_V->setEnabled(true);
    m_pEditAndBringGroup->addAction(ui->actionDelete);


    //置顶置底
    connect(ui->actionTop_T,SIGNAL(triggered(bool)),this,SLOT(actionTop_clicked()));
    connect(ui->actionBottom_B,SIGNAL(triggered(bool)),this,SLOT(actionBottom_clicked()));
    m_pEditAndBringGroup->addAction(ui->actionTop_T);
    m_pEditAndBringGroup->addAction(ui->actionBottom_B);



    //属性
    connect(ui->actionAttribute,SIGNAL(triggered(bool)),this,SLOT(actionAttribute_clicked()));

    //排列
    connect(ui->actionAlignTop,SIGNAL(triggered(bool)),this,SLOT(actionAlignTop_clicked()));
    connect(ui->actionAlignHCenter,SIGNAL(triggered(bool)),this,SLOT(actionAlignHCenter_clicked()));
    connect(ui->actionAlignBottom,SIGNAL(triggered(bool)),this,SLOT(actionAlignBottom_clicked()));
    connect(ui->actionAlignLeft,SIGNAL(triggered(bool)),this,SLOT(actionAlignLeft_clicked()));
    connect(ui->actionAlignVCenter,SIGNAL(triggered(bool)),this,SLOT(actionAlignVCenter_clicked()));
    connect(ui->actionAlignRight,SIGNAL(triggered(bool)),this,SLOT(actionAlignRight_clicked()));
    m_pAlignGroup->addAction(ui->actionAlignTop);
    m_pAlignGroup->addAction(ui->actionAlignHCenter);
    m_pAlignGroup->addAction(ui->actionAlignBottom);
    m_pAlignGroup->addAction(ui->actionAlignLeft);
    m_pAlignGroup->addAction(ui->actionAlignVCenter);
    m_pAlignGroup->addAction(ui->actionAlignRight);
    m_pAlignGroup->setEnabled(false);

    //旋转
    connect(ui->actionRotate,SIGNAL(triggered(bool)),this,SLOT(actionRotate_clicked()));
    connect(ui->actionFilpLeft,SIGNAL(triggered(bool)),this,SLOT(actionFlipLeft_clicked()));
    connect(ui->actionFlipRight,SIGNAL(triggered(bool)),this,SLOT(actionFlipRight_clicked()));
    connect(ui->actionFlipHorizon,SIGNAL(triggered(bool)),this,SLOT(actionFlipHorizon_clicked()));
    connect(ui->actionFlipHorizon,SIGNAL(triggered(bool)),this,SLOT(actionFlipVertical_clicked()));
    m_pFlipGroup->addAction(ui->actionRotate);
    m_pFlipGroup->addAction(ui->actionFilpLeft);
    m_pFlipGroup->addAction(ui->actionFlipRight);
    m_pFlipGroup->addAction(ui->actionFlipHorizon);
    m_pFlipGroup->addAction(ui->actionFlipVertical);
    m_pFlipGroup->setEnabled(false);


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
    QActionGroup * actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actionText);
    actionGroup->addAction(ui->actionLine);
    actionGroup->addAction(ui->actionRectangle);
    actionGroup->addAction(ui->actionPolyline);
    actionGroup->addAction(ui->actionPolygon);
    actionGroup->addAction(ui->actionCircle);
    actionGroup->addAction(ui->actionArc);
    actionGroup->addAction(ui->actionSelect);


    //缩放
    connect(ui->actionZoomin,SIGNAL(triggered(bool)),this,SLOT(actionZoomin_clicked()));
    connect(ui->actionZoomout,SIGNAL(triggered(bool)),this,SLOT(actionZoomout_clicked()));
    connect(ui->actionZoom,SIGNAL(triggered(bool)),this,SLOT(actionZoom_clicked()));
    connect(ui->actionZoomSame,SIGNAL(triggered(bool)),this,SLOT(actionZoomSame_clicked()));


    //等比
    connect(ui->actionSameWidth,SIGNAL(triggered(bool)),this,SLOT(actionSameWidth_clicked()));
    connect(ui->actionSameHeight,SIGNAL(triggered(bool)),this,SLOT(actionSameHeight_clicked()));
    connect(ui->actionSame,SIGNAL(triggered(bool)),this,SLOT(actionSame_clicked()));
    m_pEqualGroup->addAction(ui->actionSameWidth);
    m_pEqualGroup->addAction(ui->actionSameHeight);
    m_pEqualGroup->addAction(ui->actionSame);
    m_pEqualGroup->setEnabled(false);
    connect(ui->actionHSameSpace,SIGNAL(triggered(bool)),this,SLOT(actionHSameSpace_clicked()));
    connect(ui->actionVSameSpace,SIGNAL(triggered(bool)),this,SLOT(actionVSameSpace_clicked()));
    m_pDivideGroup->addAction(ui->actionHSameSpace);
    m_pDivideGroup->addAction(ui->actionVSameSpace);
    m_pDivideGroup->setEnabled(false);

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
    //ui->resDockWidget->setMaximumWidth(10);

    pGraphTreeWidget = new HGraphTreeWidget(pGraphEditorMgr);
    ui->fileDockWidget->setWidget(pGraphTreeWidget);
    connect(pGraphTreeWidget,SIGNAL(graphNew(const QString&)),this,SLOT(New(const QString&)));//新建
    connect(pGraphTreeWidget,SIGNAL(graphOpen(const QString&,const int)),this,SLOT(Open(const QString&,const int)));//打开
    connect(pGraphTreeWidget,SIGNAL(graphDel(const QString&,const int )),this,SLOT(Del(const QString&,const int)));//删除
    connect(pGraphTreeWidget,SIGNAL(graphImport(const QString&)),this,SLOT(ImportFile(const QString&)));
    connect(pGraphTreeWidget,SIGNAL(graphImportPath(const QString&)),this,SLOT(ImportGraphFolder(const QString&)));
}

void HGraphEditorMainWindow::initGraphEditorMgr()
{

}

void HGraphEditorMainWindow::initMainWindow()
{
    //pGraphEditorScene = new HGraphEditorScene(pGraphEditorMgr);
    pGraphEditorView = new HGraphEditorView(ui->centralWidget);
    pGraphEditorView->setObjectName(QStringLiteral("画图系统"));
    pGraphEditorView->setFrameShape(QFrame::NoFrame);
    pGraphEditorView->setFrameShadow(QFrame::Plain);
    pGraphEditorView->setLineWidth(0);

    ui->gridLayout->addWidget(pGraphEditorView,0,1,1,1);
    pGraphEditorMgr->setGraphEditorView(pGraphEditorView);
    connect(pGraphEditorMgr->graphEditorScene(),SIGNAL(itemInserted(int)),this,SLOT(itemInserted(int)));
    connect(pGraphEditorMgr->graphEditorScene(),SIGNAL(selectItemChanged(int)),this,SLOT(selectItemChanged(int)));
    connect(pGraphEditorMgr->graphEditorScene(),SIGNAL(mousePosChanged(const QPointF&)),this,SLOT(viewMousePosChanged(const QPointF&)));

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
    pGraphEditorMgr->delGraphSceneItem();
    pGraphEditorMgr->New(graphName);
    pGraphTreeWidget->addGraphTreeWidgetItem();
    pGraphEditorMgr->refreshView();
}

void HGraphEditorMainWindow::Open(const QString& name,const int id)
{
    //主要要先保存，然后删除当前的，然后新建graph对象(mgr->doc)，然后树新增
    if(!pGraphEditorMgr)
        return;

    //如果有修改的
    if(pGraphEditorMgr->isGraphModify())
    {
        if(QMessageBox::Yes == QMessageBox::information(NULL,QStringLiteral("提醒"),QStringLiteral("需要保存当前的画面文件吗？"),QMessageBox::Yes|QMessageBox::No))
        {

             pGraphEditorMgr->Save();
        }
    }

    //先删除原来的，在打开文件，最后显示
    pGraphEditorMgr->delGraphSceneItem();
    pGraphEditorMgr->Open(name,id);
    pGraphEditorMgr->openGraphScene();
    pGraphEditorMgr->refreshView();
}

void HGraphEditorMainWindow::ImportFile(const QString& name)
{
    if(!pGraphEditorMgr)
        return;
    //对文件进行判断
    QFileInfo fileInfo(name);
    QString strName = fileInfo.completeSuffix();
    if(strName.compare("grf") != 0)
    {
        if(QMessageBox::Ok == QMessageBox::information(NULL,QStringLiteral("警告"),QStringLiteral("画面后缀为grf,请修改"),QMessageBox::Yes))
        {
             return;
        }
    }
    int graphID = pGraphEditorMgr->ImportFile(name);
    if((int)-1 == graphID)
    {
        //提示导入失败
        return;
    }
    if(pGraphEditorMgr->isGraphModify())
    {
        if(QMessageBox::Ok == QMessageBox::information(NULL,QStringLiteral("提醒"),QStringLiteral("需要保存当前的画面文件吗？"),QMessageBox::Yes|QMessageBox::No))
        {
             //Save();
        }
    }
    //通知属性控件刷新
    pGraphEditorMgr->Open("",graphID);
    pGraphTreeWidget->addGraphTreeWidgetItem();
    pGraphEditorMgr->refreshView();
}


void HGraphEditorMainWindow::ImportGraphFolder(const QString& path)
{
    //判断文件个数是否过多
    QDir graphsDir(path);
    QFileInfoList graphFolderList = graphsDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    if(graphFolderList.count() > 1 || graphFolderList.count() == 0)
    {

    }
    foreach(QFileInfo info,graphFolderList)
    {
    }

    QString strName = QFileInfo(graphFolderList[0]).fileName();
    ImportFile(path + "/" + strName);
}

void HGraphEditorMainWindow::Del(const QString& graphName,const int graphID)
{
    if(!pGraphEditorMgr)
        return;
    if(QMessageBox::No == QMessageBox::information(NULL,QStringLiteral("提醒"),QStringLiteral("确定需要删除该画面吗？"),QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }
    pGraphEditorMgr->delGraphSceneItem();
    pGraphEditorMgr->Del(graphName,graphID);
    pGraphTreeWidget->delGraphTreeWidgetItem();
    pGraphEditorMgr->refreshView();

}

void HGraphEditorMainWindow::itemInserted(int type)
{
    ui->actionSelect->setChecked(true);
    emit ui->actionSelect->triggered();
}

void HGraphEditorMainWindow::selectItemChanged(int nSelectMode)
{
    m_pFlipGroup->setEnabled(true);
    m_pEditAndBringGroup->setEnabled(true);
    if(nSelectMode == SELECT_MODE_NO)
    {
        m_pAlignGroup->setEnabled(false);
        m_pEqualGroup->setEnabled(false);
        m_pDivideGroup->setEnabled(false);
        m_pFlipGroup->setEnabled(false);
        m_pEditAndBringGroup->setEnabled(false);
    }
    else if(nSelectMode == SELECT_MODE_SINGLE)
    {
        m_pAlignGroup->setEnabled(false);
        m_pEqualGroup->setEnabled(false);
        m_pDivideGroup->setEnabled(false);
    }
    else if(nSelectMode == SELECT_MODE_2MULTIPLE || nSelectMode == SELECT_MODE_MULTIPLE)
    {
        m_pAlignGroup->setEnabled(true);
        m_pEqualGroup->setEnabled(true);
        m_pDivideGroup->setEnabled(false);
        if(nSelectMode == SELECT_MODE_MULTIPLE)
            m_pDivideGroup->setEnabled(true);
    }
}

void HGraphEditorMainWindow::viewMousePosChanged(const QPointF &logPos)
{

    //QString strViewPos = QString("x=%1,y=%2").arg((int)pos.x()).arg((int)pos.y());
    QString strLogicPos =  QString("log:x=%1,y=%2").arg(logPos.x(),0,'f',1).arg(logPos.y(),0,'f',1);
    statusBar()->showMessage(strLogicPos );
}
