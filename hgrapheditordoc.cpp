#include "hgrapheditordoc.h"
#include "publicdata.h"
#include "hgrapheditormgr.h"
#include "hgraph.h"
#include "hicontemplate.h"
#include "hstation.h"
#include "hgraphhelper.h"
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QProcessEnvironment>
//图形文件存储类
HGraphEditorDoc::HGraphEditorDoc(HGraphEditorMgr* mgr)
    :pGraphEditorMgr(mgr)
{
    pCurGraph = NULL;
}

HGraphEditorDoc::~HGraphEditorDoc()
{

}

//加载厂站信息
void HGraphEditorDoc::loadStation()
{
    //读取厂站信息
    /*
    FILEHANDLE fileHandle;
    memset(&fileHandle,0,sizeof(FILEHANDLE));
    DATAFILEHEADER dataFileHandle;

    openDB(FILE_TYPE_STATION);
    loadDataFileHeader(FILE_TYPE_STATION,&dataFileHandle);
    //int wStation = 0;
    for(int i = 0 ; i < dataFileHandle.wTotal; i++)
    {
        HStation* pStation = new HStation;
        if(!pStation)
            break;

        if ( false == loadDBRecord(FILE_TYPE_STATION, ++fileHandle.wStation, &pStation->station ) )
        {
            delete pStation;
            pStation=NULL;
            break;
        }
        if(false == pStation->loadData(fileHandle))
        {
            delete pStation;
            pStation = NULL;
            break;
        }
        pStationList.append(pStation);
    }
    closeDB(FILE_TYPE_STATION);
    */
}


//厂站ID获取厂站
HStation* HGraphEditorDoc::getStation(quint16 wStationID)
{
    for(int i = 0; i < pStationList.count();i++)
    {
        HStation* pStation = pStationList[i];
        if(wStationID == pStation->getNo())
            return pStation;
    }
    return NULL;
}

//厂站地址获取厂站
HStation* HGraphEditorDoc::getRtu(quint16 wStationAddress)
{
    for(int i = 0; i < pStationList.count();i++)
    {
        HStation* pStation = pStationList[i];
        if(wStationAddress == pStation->getAddress())
            return pStation;
    }
    return NULL;
}

//索引厂站
HStation* HGraphEditorDoc::findStation(int nIndex)
{
    return pStationList.value(nIndex);
}

//加载模板信息
void HGraphEditorDoc::loadIconTemplate()
{
    HGraphHelper::Instance()->loadIconTemplate(&pIconTemplateList);
}

//寻找模板
HIconTemplate* HGraphEditorDoc::findIconTemplate(const QString &uuid)
{
    for(int i = 0; i < pIconTemplateList.count();i++)
    {
        HIconTemplate* iconTemplate = (HIconTemplate*)pIconTemplateList[i];
        if(iconTemplate->getUuid().toString() == uuid)
            return iconTemplate;
    }
    return NULL;
}

//加载画面信息
void HGraphEditorDoc::loadAllGraph()
{
    HGraphHelper::Instance()->loadAllGraph(&pGraphList);

    //加载完画面之后需要更新每一个画面里面的图元控件
    updateGraphList();

}

//更新所有画面里面的模板信息
void HGraphEditorDoc::updateGraphList()
{
    for(int i = 0; i < pGraphList.count();i++)
    {
        HGraph* pGraph = (HGraph*)pGraphList[i];
        if(!pGraph) continue;
        HIconTemplate* findTemp = NULL;
        for(int j = 0; j < pGraph->pIconTemplateList.count();j++)
        {
            HIconTemplate* pGraphTemp = (HIconTemplate*)pGraph->pIconTemplateList[j];
            if(!pGraphTemp)
                continue;
            findTemp = findIconTemplate(pGraphTemp->getUuid().toString());
            if(findTemp)
            {
                findTemp->copyTo(pGraphTemp);
            }
            for(int k = 0; k < pGraph->getObjList().count();k++)
            {
                HBaseObj* pObj = (HBaseObj*)pGraph->getObjList().at(k);
                if(pObj->getShapeType() == enumComplex)
                {
                    HIconObj* pObj1 = (HIconObj*)pObj;//
                    if(!pObj1->iconTemplate()->getUuid().toString().compare(findTemp->getUuid().toString()))
                    {
                        findTemp->getSymbol()->copyTo(pObj1->getIconSymbol());
                        pObj1->updateResize();
                        int a = 0;
                    }
                }
            }
        }
    }
}

void HGraphEditorDoc::saveAllGraph()
{
    HGraphHelper::Instance()->saveAllGraph(&pGraphList,getCurGraph());
}

void HGraphEditorDoc::saveCurGraph()
{
    if(!pCurGraph)
    {
        return;
    }
    HGraph* graph = findGraph(pCurGraph->getGraphID());
    if(!graph)
        return;
    pCurGraph->copyTo(graph);
    HGraphHelper::Instance()->saveGraph(graph);
}

int HGraphEditorDoc::importGraph(const QString& name)
{
    char szGraphPath[128];
    getDataFilePath(DFPATH_GRAPH,szGraphPath);
    QString graphsPath = QString(szGraphPath);
    //需要判一下 这个文件是否已经在graph里面了
    bool bFind = false;
    QString strNewFile = name;
    QFileInfo graphInfo(name);
    QString strGraphName = graphInfo.fileName();
    QString strFolderPath = name.left(name.lastIndexOf('/',-(strGraphName.length()+1)));
    strFolderPath = strFolderPath.left(strFolderPath.lastIndexOf('/',-1));
    if(QDir::toNativeSeparators(strFolderPath).compare(QDir::toNativeSeparators(graphsPath)) == 0)
        bFind = true;
    QString strFolder;
    if(!bFind)
    {
        strFolder = name.section('/',-2,-2);

        //搜索是否有重名的文件夹
        int nCount = 0;
        QDir graphsDir(graphsPath);
        QFileInfoList graphFolderList = graphsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach(QFileInfo info,graphFolderList)
        {
            if(info.isFile())continue;
            if(info.fileName().contains(strFolder))
                nCount++;
        }

        if(!QDir(strFolder).exists())
        {   if(nCount > 0)
            {
                strFolder = strFolder + QString("_%2").arg(nCount);
            }
            if(!graphsDir.mkdir(strFolder))
                return (int)-1;
        }
        //将源文件拷贝过来 如果失败了，还需要删除文件夹
        strNewFile = graphsPath + "/" + strFolder + "/0.grf";
        if(!QFile::copy(name,strNewFile))
            return false;
    }
    HGraph* pGraph = new HGraph("");
    if(!pGraph)
        return (int)-1;
    if(!pGraph->readXmlFile(strNewFile))
    {
        delete pGraph;
        pGraph = NULL;
        return (int)-1;
    }
    if(pGraph->getGraphName().compare(strFolder) != 0)
        pGraph->setGraphName(strFolder);
    //在里面寻找graphID 如果找到了，graphID就要更改了
    if(findGraph(pGraph->getGraphID()))
        pGraph->setGraphID(getGraphID());
    pGraph->writeXmlFile(strNewFile);//更新完要写进去
    pGraphList.append(pGraph);

    return pGraph->getGraphID();
}

//获取一个新graph的ID
int HGraphEditorDoc::getGraphID()
{
    int nID = 0;
    while(findGraph(nID)!=NULL)
        nID++;
    return nID;
}

//按照ID查找画面
HGraph* HGraphEditorDoc::findGraph(int graphID)
{
    HGraph *graph = NULL;
    QList<HGraph*>::Iterator graphIterator;
    for(graphIterator = pGraphList.begin();graphIterator != pGraphList.end();graphIterator++)
    {
        graph = *graphIterator;
        if(graph->getGraphID() == graphID)
            return graph;
    }
    return NULL;
}

HGraph* HGraphEditorDoc::findGraph(const QString& graphName)
{
    HGraph *graph = NULL;
    QList<HGraph*>::Iterator graphIterator;
    for(graphIterator = pGraphList.begin();graphIterator != pGraphList.end();graphIterator++)
    {
        graph = *graphIterator;
        if(graph->getGraphName() == graphName)
            return graph;
    }
    return NULL;
}

//新建画面
HGraph* HGraphEditorDoc::addGraph(const QString& name)
{
    if(!pGraphEditorMgr) return NULL;
    if(pCurGraph)
    {
        pCurGraph->clear();
        delete pCurGraph;
        pCurGraph = NULL;
    }
    HGraph* newGraph = new HGraph(name);
    newGraph->setGraphID(getGraphID());
    newGraph->setGraphHeight(pGraphEditorMgr->getLogicRect().height());
    newGraph->setGraphWidth(pGraphEditorMgr->getLogicRect().width());
    pGraphList.append(newGraph);

    pCurGraph = new HGraph("tempGraph");
    newGraph->copyTo(pCurGraph);
    return newGraph;
}

//删除画面(名字可以唯一，但内部采用id)
bool HGraphEditorDoc::delGraph(const QString& name,const int id)
{
    HGraph* graph = findGraph(id);
    if(!graph)
        return false;
    pGraphList.removeOne(graph);
    delete graph;
    graph = NULL;

    if(pCurGraph)
    {
        delete pCurGraph;
        pCurGraph = NULL;
    }
    return true;
}

//打开画面
bool HGraphEditorDoc::openGraph(const QString& name,const int id)
{
    HGraph* graph = findGraph(id);
    if(!graph)
        return false;

    if(pCurGraph)
    {
        if(pCurGraph->getGraphID() == graph->getGraphID())
            return true;
        pCurGraph->clear();
        delete pCurGraph;
        pCurGraph = NULL;
    }

    pCurGraph = new HGraph("tempGraph");
    if(!pCurGraph) return false;
    graph->copyTo(pCurGraph);
    return true;
}

//拷贝画面
void HGraphEditorDoc::copyGraph(HGraph* graph)
{

}

HGraph* HGraphEditorDoc::getCurGraph()
{
    return pCurGraph;
}


QList<HIconTemplate*> HGraphEditorDoc::getIconTemplateList()
{
    return pIconTemplateList;
}

QList<HGraph*> HGraphEditorDoc::getGraphList()
{
    return pGraphList;
}

//判断graph文件是否修改
bool HGraphEditorDoc::isGraphModify()
{
    //只需要判断当前画面是否存在修改
    if(!pCurGraph)
        return false;
    return pCurGraph->getModify();
}
