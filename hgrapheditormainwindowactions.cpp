#include "hgrapheditormainwindow.h"
#include "ui_grapheditormainwindow.h"
#include "hiconapi.h"
#include "hgrapheditormgr.h"
#include "hgrapheditordoc.h"
#include "hgrapheditorview.h"
#include "hgrapheditorop.h"
//文件部分
void HGraphEditorMainWindow::actionNew_clicked()
{

}

void HGraphEditorMainWindow::actionOpen_clicked()
{
    //打开所有的
}

void HGraphEditorMainWindow::actionSave_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorDoc())
        return;
    pGraphEditorMgr->graphEditorDoc()->saveAllGraph();
}

void HGraphEditorMainWindow::actionSaveAs_clicked()
{

}

void HGraphEditorMainWindow::actionPrint_clicked()
{

}

void HGraphEditorMainWindow::actionExit_clicked()
{
    //退出之前需要将curGrpah拷贝到对应的graph里面 然后delete curGraph
}

//编辑部分
void HGraphEditorMainWindow::acitonUndo_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorStack())
        return;
    pGraphEditorMgr->graphEditorStack()->undo();
}

void HGraphEditorMainWindow::actionRedo_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorStack())
        return;
    pGraphEditorMgr->graphEditorStack()->redo();
}

void HGraphEditorMainWindow::actionCut_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->cut();
}

void HGraphEditorMainWindow::actionCopy_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorView())
        return;
    pGraphEditorMgr->graphEditorOp()->copy();
}

void HGraphEditorMainWindow::actionPaste_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorView())
        return;
    pGraphEditorMgr->graphEditorOp()->paste();
}

void HGraphEditorMainWindow::actionDelete_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorView())
        return;
    pGraphEditorMgr->graphEditorOp()->del();
}

//置顶置底
void HGraphEditorMainWindow::actionTop_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->bringToTop();
}

void HGraphEditorMainWindow::actionBottom_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->bringToBottom();
}

//属性
void HGraphEditorMainWindow::actionAttribute_clicked()
{

}

//排列
void HGraphEditorMainWindow::actionAlignTop_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->alignTop();
}

void HGraphEditorMainWindow::actionAlignHCenter_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->alignHCenter();
}

void HGraphEditorMainWindow::actionAlignBottom_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->alignBottom();
}

void HGraphEditorMainWindow::actionAlignLeft_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->alignLeft();
}

void HGraphEditorMainWindow::actionAlignVCenter_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->alignVCenter();
}

void HGraphEditorMainWindow::actionAlignRight_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->alignRight();
}

//旋转
void HGraphEditorMainWindow::actionRotate_clicked()
{

}

void HGraphEditorMainWindow::actionFlipLeft_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->flipLeft90();
}

void HGraphEditorMainWindow::actionFlipRight_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->flipRight90();
}

void HGraphEditorMainWindow::actionFlipHorizon_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->flipHorizon();
}

void HGraphEditorMainWindow::actionFlipVertical_clicked()
{
    if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
        return;
    pGraphEditorMgr->graphEditorOp()->flipVertical();
}

 //字体部分
 void HGraphEditorMainWindow::fontBox_clicked(int nIndex)
 {

 }

 void HGraphEditorMainWindow::fontSizeBox_clicked(int nIndex)
 {

 }

 void HGraphEditorMainWindow::actionBold_clicked(bool bchecked)
 {

 }

 void HGraphEditorMainWindow::actionItalic_clicked(bool bchecked)
 {

 }

 void HGraphEditorMainWindow::actionTextUnder_clicked(bool bchecked)
 {

 }

 //颜色部分
 void HGraphEditorMainWindow::actionColor_clicked()
 {

 }

 void HGraphEditorMainWindow::actionFillColor_clicked()
 {

 }

 void HGraphEditorMainWindow::actionBackPicture_clicked()
 {

 }

 //组合
 void HGraphEditorMainWindow::actionGroup_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->groupObj();
 }

 void HGraphEditorMainWindow::actionUnGroup_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->ungroupObj();
 }

 //选择,工具
 void HGraphEditorMainWindow::actionSelect_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumSelection);
 }

 void HGraphEditorMainWindow::actionText_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumText);
     pGraphEditorMgr->setSelectMode(enumDraw);
 }

 void HGraphEditorMainWindow::actionLine_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumLine);
     pGraphEditorMgr->setSelectMode(enumDraw);
 }

 void HGraphEditorMainWindow::actionRectagle_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumRectangle);
     pGraphEditorMgr->setSelectMode(enumDraw);
 }

 void HGraphEditorMainWindow::actionPolyline_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumPolyline);
     pGraphEditorMgr->setSelectMode(enumDraw);
 }

 void HGraphEditorMainWindow::actionPolygon_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumPolygon);
     pGraphEditorMgr->setSelectMode(enumDraw);
 }

 void HGraphEditorMainWindow::actionCircle_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumCircle);
     pGraphEditorMgr->setSelectMode(enumDraw);
 }

 void HGraphEditorMainWindow::actionArc_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumArc);
     pGraphEditorMgr->setSelectMode(enumDraw);
 }

 //缩放
 void HGraphEditorMainWindow::actionZoomin_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->zoomIn();
 }

 void HGraphEditorMainWindow::actionZoomout_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->zoomOut();
 }

 void HGraphEditorMainWindow::actionZoom_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->zoom();
 }

 void HGraphEditorMainWindow::actionZoomSame_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->zoomSame();
 }

 //等比
 void HGraphEditorMainWindow::actionSameWidth_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->sizeEqualWidth();
 }

 void HGraphEditorMainWindow::actionSameHeight_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->sizeEqualHeight();
 }

 void HGraphEditorMainWindow::actionSame_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->sizeEqualComplete();
 }

 void HGraphEditorMainWindow::actionHSameSpace_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->sizeHEqualSpace();
 }

 void HGraphEditorMainWindow::actionVSameSpace_clicked()
 {
     if(!pGraphEditorMgr || !pGraphEditorMgr->graphEditorOp())
         return;
     pGraphEditorMgr->graphEditorOp()->sizeVEqualSpace();
 }

 //关于
 void HGraphEditorMainWindow::about()
 {

 }
