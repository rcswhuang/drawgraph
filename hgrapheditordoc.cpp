#include "hgrapheditordoc.h"
#include "publicdata.h"
#include "hgrapheditormgr.h"
#include "hgraph.h"
#include "hicontemplate.h"
#include "hstation.h"
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
    //先找路径，在找文件夹，然后文件夹里面搜索添加完成
    QString iconsPath  = QString(qgetenv("wfsystem_dir"));
#ifdef WIN32
    iconsPath = QProcessEnvironment::systemEnvironment().value("wfsystem_dir");
#else
    iconsPath = "/users/huangw";
#endif
    iconsPath.append("/icons");

    QDir dirIconsPath(iconsPath);
    if(!dirIconsPath.exists())
        return;
    QFileInfoList iconsDirList = dirIconsPath.entryInfoList(QDir::Dirs  | QDir::NoDotAndDotDot);
    foreach(QFileInfo info,iconsDirList)
    {
        if(info.isFile())continue; //icons文件夹下面文件是不读取的
        QString strChildFilePath = iconsPath + "/" + info.fileName();

        //在对应测点类型的文件夹里面搜索添加完成
        QDir dirIconsFilePath(strChildFilePath);
        if(!dirIconsFilePath.exists())
            return;
        QStringList filters;
        filters<<"*.xic";
        dirIconsFilePath.setNameFilters(filters);
        QFileInfoList iconsFileInfoList = dirIconsFilePath.entryInfoList(QDir::Files);
        foreach(QFileInfo info,iconsFileInfoList)
        {
            if(!info.isFile())continue;
            QString strIconTemplateFile = strChildFilePath + "/" + info.fileName();
            QUuid uuid = QUuid(info.fileName());
            HIconTemplate *pIconTemplate = new HIconTemplate(uuid);
            if(!pIconTemplate) continue;
            pIconTemplate->readXml(strIconTemplateFile);
            pIconTemplateList.append(pIconTemplate);
        }
    }
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
    //先找路径，在找文件夹，然后文件夹里面搜索添加完成
    QString iconsPath  = QString(qgetenv("wfsystem_dir"));
#ifdef WIN32
    iconsPath = QProcessEnvironment::systemEnvironment().value("wfsystem_dir");
#else
    iconsPath = "/users/huangw";
#endif
    iconsPath.append("/graph");

    QDir dirIconsPath(iconsPath);
    if(!dirIconsPath.exists())
        return;
    QFileInfoList iconsDirList = dirIconsPath.entryInfoList(QDir::Dirs  | QDir::NoDotAndDotDot);
    foreach(QFileInfo info,iconsDirList)
    {
        if(info.isFile())continue; //icons文件夹下面文件是不读取的
        QString strChildFilePath = iconsPath + "/" + info.fileName();//graph/110kV测试变主接线图 文件夹

        //在对应测点类型的文件夹里面搜索添加完成
        QDir dirIconsFilePath(strChildFilePath);
        if(!dirIconsFilePath.exists())
            continue;
        QStringList filters;
        filters<<"*.grf";
        dirIconsFilePath.setNameFilters(filters);
        QFileInfoList graphsFileInfoList = dirIconsFilePath.entryInfoList(QDir::Files);
        foreach(QFileInfo info,graphsFileInfoList)
        {
            if(!info.isFile())continue;
            QString strGraphName = strChildFilePath + "/" + info.fileName();
            HGraph* pGraph = new HGraph("");
            pGraph->readXmlFile(strGraphName);
            pGraphList.append(pGraph);
        }
    }
}

void HGraphEditorDoc::saveAllGraph()
{
    QString graphsPath = QString(getenv("wfsystem_dir"));;
#ifdef WIN32
    graphsPath = QProcessEnvironment::systemEnvironment().value("wfsystem_dir");
#else
    iconsPath = "/users/huangw";
#endif
    graphsPath.append("/graph");
    QDir dirIconsPath(graphsPath);
    if(!dirIconsPath.exists()) //---huangw 只能上层路径创建子文件夹
        dirIconsPath.mkdir(graphsPath);

    //先扫描一下当前文件夹内所有的画面名称
    QStringList curExistFolderList;
    QFileInfoList iconsFileInfoList = dirIconsPath.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QFileInfo info,iconsFileInfoList)
    {
        if(info.isFile())continue;
        curExistFolderList<<info.fileName();
    }

    for(int i = 0; i < pGraphList.count();i++)
    {
        HGraph* pGraph = (HGraph*)pGraphList[i];
        if(!pGraph)
            continue;
        QString strGraphName = pGraph->getGraphName();
        if((int)-1 != curExistFolderList.indexOf(strGraphName)) //当前画面找到了
        {
            //saveGraph(path,graph)
            QString strFilePath = graphsPath + "/" +strGraphName;
            saveGraph(pGraph,strFilePath);
            curExistFolderList.removeOne(strGraphName);
        }
        else
        {
            //新增一个画面
            QString strFilePath = graphsPath + "/" +strGraphName;
            if(!QDir(strFilePath).exists())
            {
                if(!QDir(graphsPath).mkdir(strFilePath))
                    continue;
            }
            saveGraph(pGraph,strFilePath);
        }
    }

    //最后要把curExistFolderList剩下来的所有文件夹全部删除
    for(int i = 0; i < curExistFolderList.count();i++)
    {
        QString strPath = graphsPath + "/" + curExistFolderList[i];
        QFileInfoList iconsFileInfoList = QDir(strPath).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        foreach(QFileInfo info,iconsFileInfoList)
        {
            if(info.isFile())
                QFile::remove(strPath+"/"+info.fileName());
        }
        dirIconsPath.rmpath(strPath);
    }
}

void HGraphEditorDoc::saveGraph(HGraph* graph,QString& path)
{
    if(NULL == graph)
        return;
    if(pCurGraph)
    {
        if(pCurGraph->getGraphID() == graph->getGraphID())
            pCurGraph->copyTo(graph);
    }

    QDir dirIconsFilePath(path);
    if(!dirIconsFilePath.exists())
        return;
    QString strFileName = path + "/" +  "0.grf";
    //如果有文件存在 就删除
    if(QFile::exists(strFileName))
    {
        QFile::remove(strFileName);
    }
    graph->writeXmlFile(strFileName);
}

int HGraphEditorDoc::importGraph(const QString& name)
{
    QString graphsPath = QString(getenv("wfsystem_dir"));;
#ifdef WIN32
    graphsPath = QProcessEnvironment::systemEnvironment().value("wfsystem_dir");
#else
    iconsPath = "/users/huangw";
#endif
    graphsPath.append("/graph");
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
    if(pCurGraph)
    {
        pCurGraph->clear();
        delete pCurGraph;
        pCurGraph = NULL;
    }
    HGraph* newGraph = new HGraph(name);
    newGraph->setGraphID(getGraphID());
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
