#include "hgrapheditordoc.h"
#include "publicdata.h"
#include "hgrapheditormgr.h"
#include "hgraph.h"
#include "hstation.h"
//图形文件存储类
HGraphEditorDoc::HGraphEditorDoc(HGraphEditorMgr* mgr)
    :pGraphEditorMgr(mgr)
{

}

//加载厂站信息
void HGraphEditorDoc::loadStation()
{
    //读取厂站信息
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

}

//加载画面信息
void HGraphEditorDoc::loadAllGraph()
{

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

//新建画面
HGraph* HGraphEditorDoc::addGraph(const QString& name)
{
    HGraph* newGraph = new HGraph(name);
    newGraph->setGraphID(getGraphID());
    pGraphList.append(newGraph);
    return newGraph;
}

//删除画面(名字可以唯一，但内部采用id)
void HGraphEditorDoc::delGraph(const QString& name,const int id)
{
    HGraph* graph = findGraph(id);
    if(!graph)
        return;
    pGraphList.removeOne(graph);
    delete graph;
    graph = NULL;
}

//打开画面
void HGraphEditorDoc::openGraph(const QString& name,const int id)
{

}

//拷贝画面
void HGraphEditorDoc::copyGraph(HGraph* graph)
{

}

//????
QList<HIconTemplate*> HGraphEditorDoc::getIconTemplateList()
{
    return pIconTemplateList;
}
