#include "hgraph.h"
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
HGraph::HGraph(const QString& name)
    :sName(name)
{

}

HGraph::~HGraph()
{

}

void HGraph::setGraphID(int id)
{
    nGraphID = id;
}

//获取ID
int HGraph::getGraphID()
{
    return nGraphID;
}

void HGraph::ReadDataFile(const QString&)
{

}

 void HGraph::WriteDateFile(const QString&)
{

}

 void HGraph::ReadData(int,QDataStream *d)
 {

 }

 void HGraph::WriteData(int,QDataStream *d)
 {

 }

 //先打开文件
void HGraph::ReadXmlFile(const QString& fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QTextStream dsm(&file);
    QDomDocument doc;
    dsm.setCodec("GB2312");
    QString errorString;
    int errorLine;
    int errorColumn;
    if(!doc.setContent(&file,false,&errorString,&errorLine,&errorColumn))
    {
        file.close();
        return;
    }
    QDomElement root = doc.documentElement();
    if(root.isNull())
        return;
    ReadXml(&root);
    file.close();

    sName = fileName;
}

//读具体的信息
void HGraph::ReadXml(QDomElement *d)
{
  //分为本身属性，模板部分，动态数据部分
}

void HGraph::WriteXmlFile(const QString&)
{

}

void HGraph::WriteXml(QDomElement *d)
{

}

void HGraph::ReadDynamicXml(QDomElement* d)
{

}

void HGraph::WriteDynamicXml(QDomElement* d)
{

}

void HGraph::ReadDynamicData(int,QDataStream* d)
{

}

void HGraph::WriteDynamicData(int,QDataStream* d)
{

}

void HGraph::setGraphName(const QString& name)
{

}

QString HGraph::getGraphName()
{

}

void HGraph::Draw(QPainter* p)
{

}

void HGraph::clear()
{

}

void HGraph::copyTo(HBaseObj* obj)
{

}

void HGraph::addObj(HBaseObj* pObj)
{

}

void HGraph::removeObj(HBaseObj* pObj)
{

}

//模板部分
int HGraph::iconTemplateNum()
{

}

HIconTemplate* HGraph::IconTemplateAt(int index)
{

}

HIconTemplate* HGraph::findIconTemplate(const QUuid& uid)
{

}

HIconTemplate* HGraph::addIconTemplate(HIconTemplate* temp)
{

}

void HGraph::removeIconTemplate(HIconTemplate* temp)
{

}

void HGraph::clearIconTemplate()
{

}

void HGraph::resetIconTemplate()
{

}
