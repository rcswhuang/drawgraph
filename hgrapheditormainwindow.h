#ifndef HGRAPHEDITORMAINWINDOW_H
#define HGRAPHEDITORMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class GraphEditorMainWindow;
}
class HGraphEditorMgr;
class HGraphTreeWidget;
class HGraphEditorScene;
class HGraphEditorView;
class HIconTabWidget;
class HGraphEditorDoc;
class HIconViewEditor;
class QLineEdit;
class QComboBox;
class QActionGroup;
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
    void createFontBar();


public slots:

   void New(const QString&);
   void Open(const QString&,const int graphID);
   void Del(const QString&,const int graphID);
   void ImportGraphFolder(const QString&);
   void ImportFile(const QString&);

   //文件部分
   void actionNew_clicked();
   void actionOpen_clicked();
   void actionSave_clicked();
   void actionSaveAs_clicked();
   void actionPrint_clicked();
   void actionExit_clicked();
   
   //编辑部分
   void acitonUndo_clicked();
   void actionRedo_clicked();
   void actionCut_clicked();
   void actionCopy_clicked();
   void actionPaste_clicked();
   void actionDelete_clicked();
   
   //置顶置底
   void actionTop_clicked();
   void actionBottom_clicked();
   
   //属性
   void actionAttribute_clicked();
   
   //排列
   void actionAlignTop_clicked();
   void actionAlignHCenter_clicked();
   void actionAlignBottom_clicked();
   void actionAlignLeft_clicked();
   void actionAlignVCenter_clicked();
   void actionAlignRight_clicked();
   
   //旋转
   void actionRotate_clicked();
   void actionFlipLeft_clicked();
   void actionFlipRight_clicked();
   void actionFlipHorizon_clicked();
   void actionFlipVertical_clicked();
   
    //字体部分
    void fontBox_clicked(int nIndex);
    void fontSizeBox_clicked(int nIndex);
    void actionBold_clicked(bool bchecked = false);
    void actionItalic_clicked(bool bchecked = false);
    void actionTextUnder_clicked(bool bchecked = false);

    //颜色部分
    void actionColor_clicked();
    void actionFillColor_clicked();
    void actionBackPicture_clicked();
    
    //组合
    void actionGroup_clicked();
    void actionUnGroup_clicked();
    
    //选择,工具
    void actionSelect_clicked();
    void actionText_clicked();
    void actionLine_clicked();
    void actionRectagle_clicked();
    void actionPolyline_clicked();
    void actionPolygon_clicked();
    void actionCircle_clicked();
    void actionArc_clicked();
    
    //缩放
    void actionZoomin_clicked();
    void actionZoomout_clicked();
    void actionZoom_clicked();
    void actionZoomSame_clicked();

    //等比
    void actionSameWidth_clicked();
    void actionSameHeight_clicked();
    void actionSame_clicked();
    void actionHSameSpace_clicked();
    void actionVSameSpace_clicked();

    //关于
    void about();

public slots:
    void itemInserted(int);
    void viewMousePosChanged(const QPointF &logPos);
    void selectItemChanged(int);

private:
    HGraphEditorMgr *pGraphEditorMgr;
    //HGraphEditorScene *pGraphEditorScene;
    HGraphEditorView* pGraphEditorView;
    HGraphTreeWidget* pGraphTreeWidget;//左边树结构
    HIconTabWidget* pIconTabWidget;//右边tab结构
    HIconViewEditor* pIconViewEditor;//tab上面的view
    Ui::GraphEditorMainWindow *ui;


    QActionGroup* m_pAlignGroup;
    QActionGroup* m_pEqualGroup;
    QActionGroup* m_pDivideGroup;
    QActionGroup* m_pFlipGroup;
    QActionGroup* m_pEditAndBringGroup;
    QComboBox* pFontBox;
    QComboBox* pFontSizeBox;
    QLineEdit* pTextEdit;
};

#endif // HGRAPHEDITORMAINWINDOW_H
