#include "hgrapheditormainwindow.h"
#include "ui_grapheditormainwindow.h"
#include "hiconapi.h"
#include "hgrapheditormgr.h"
#include "hgrapheditordoc.h"
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

}

void HGraphEditorMainWindow::actionRedo_clicked()
{

}

void HGraphEditorMainWindow::actionCut_clicked()
{

}

void HGraphEditorMainWindow::actionCopy_clicked()
{

}

void HGraphEditorMainWindow::actionPaste_clicked()
{

}

void HGraphEditorMainWindow::actionDelete_clicked()
{

}

//置顶置底
void HGraphEditorMainWindow::actionTop_clicked()
{

}

void HGraphEditorMainWindow::actionBottom_clicked()
{

}

//属性
void HGraphEditorMainWindow::actionAttribute_clicked()
{

}

//排列
void HGraphEditorMainWindow::actionAlignTop_clicked()
{

}

void HGraphEditorMainWindow::actionAlignHCenter_clicked()
{

}

void HGraphEditorMainWindow::actionAlignBottom_clicked()
{

}

void HGraphEditorMainWindow::actionAlignLeft_clicked()
{

}

void HGraphEditorMainWindow::actionAlignVCenter_clicked()
{

}

void HGraphEditorMainWindow::actionAlignRight_clicked()
{

}

//旋转
void HGraphEditorMainWindow::actionRotate_clicked()
{

}

void HGraphEditorMainWindow::actionFilpLeft_clicked()
{

}

void HGraphEditorMainWindow::actionFlipRight_clicked()
{

}

void HGraphEditorMainWindow::actionFlipHorizon_clicked()
{

}

void HGraphEditorMainWindow::actionFlipVertical_clicked()
{
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

 }

 void HGraphEditorMainWindow::actionUnGroup_clicked()
 {

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
 }

 void HGraphEditorMainWindow::actionLine_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumLine);
 }

 void HGraphEditorMainWindow::actionRectagle_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumRectangle);
 }

 void HGraphEditorMainWindow::actionPolyline_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumPolyline);
 }

 void HGraphEditorMainWindow::actionPolygon_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumPolygon);
 }

 void HGraphEditorMainWindow::actionCircle_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumCircle);
 }

 void HGraphEditorMainWindow::actionArc_clicked()
 {
     if(!pGraphEditorMgr)
         return;
     pGraphEditorMgr->setDrawShape(DRAWSHAPE::enumArc);
 }

 //缩放
 void HGraphEditorMainWindow::actionZoomin_clicked()
 {

 }

 void HGraphEditorMainWindow::actionZoomout_clicked()
 {

 }

 void HGraphEditorMainWindow::actionZoom_clicked()
 {

 }

 void HGraphEditorMainWindow::actionZoomSame_clicked()
 {

 }

 //等比
 void HGraphEditorMainWindow::actionSameWidth_clicked()
 {

 }

 void HGraphEditorMainWindow::actionSameHeight_clicked()
 {

 }

 void HGraphEditorMainWindow::actionSame_clicked()
 {

 }

 void HGraphEditorMainWindow::actionHSameSpace_clicked()
 {

 }

 void HGraphEditorMainWindow::actionVSameSpace_clicked()
 {

 }

 //关于
 void HGraphEditorMainWindow::about()
 {

 }
