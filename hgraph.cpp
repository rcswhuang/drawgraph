﻿#include "hgraph.h"
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
#include <QTextCodec>

HGraph::HGraph(const QString& name)
    :sName(name)
{
    nGraphID = 0;
    nGraphWidth = 1000;
    nGraphHeight = 1000;
    nRefreshInterval = 3000;
    strFillColor = "#000000";
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

//画面文件需要同时保存到二进制文件和xml文件?
void HGraph::readDataFile(const QString&)
{

}

 void HGraph::writeDateFile(const QString&)
{

}

//可以通过datafile保存到二进制文件 也可以作为复制拷贝功能来用
void HGraph::readData(int v,QDataStream *d)
{
    QString s;
    *d>>s;
    sName = s;
    int n;
    *d>>n;
    nGraphID = n;
    *d>>n;
    nGraphWidth=n;
    *d>>n;
    nGraphHeight=n;
    *d>>n;
    nRefreshInterval = n;
    *d>>s;
    strFillColor = s;

    *d>>n;
    int nCount = n;
    for(int i = 0; i < nCount;i++)
    {
        *d>>s;
        QUuid uuid(s);
        HIconTemplate* iconTemplate = new HIconTemplate(uuid);
        if(iconTemplate)
            iconTemplate->readData(v,d);
    }

    *d>>n;
    nCount = n;
    for(int i = 0; i < nCount;i++)
    {
        quint8 type;
        *d>>type;
        QString uuid = QString();
        if(type == enumComplex)
        {
           *d>>s;
            uuid = s;
        }
        HBaseObj* pObj = newObj(type,uuid);
        pObj->readData(d);
    }
}

void HGraph::writeData(int n,QDataStream *d)
{
    *d<<sName;
    *d<<nGraphID;
    *d<<nGraphWidth;
    *d<<nGraphHeight;
    *d<<nRefreshInterval;
    *d<<strFillColor;

    *d<<pIconTemplateList.count();
    for(int i = 0; i < pIconTemplateList.count();i++)
    {
        HIconTemplate* iconTemplate = (HIconTemplate*)pIconTemplateList[i];
        QString struuid;
        if(!iconTemplate)
        {
            continue;
        }
        struuid = iconTemplate->getUuid().toString();
        *d<<struuid;
        iconTemplate->writeData(n,d);
    }

    *d<<pObjList.count();
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        quint8 type = pObj->getShapeType();
        *d<<type;
        if(type == enumComplex)
        {
            QString struuid = ((HIconComplexObj*)pObj)->iconTemplate()->getUuid().toString();
            *d<<struuid;
        }
        pObj->writeData(d);
    }
}

 //先打开文件
void HGraph::readXmlFile(const QString& fileName)
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
    readXml(&root);
    file.close();

    sName = fileName;
}

//读具体的信息
void HGraph::readXml(QDomElement *d)
{
  //分为本身属性，模板部分，动态数据部分
    if(!d)
        return;
    sName = d->attribute("GraphName");
    nGraphID = d->attribute("GraphID").toInt();
    nGraphWidth = d->attribute("GraphWidth").toInt();
    nGraphHeight = d->attribute("GraphHeight").toInt();
    nRefreshInterval = d->attribute("RefreshInterval").toInt();
    strFillColor = d->attribute("FillColor");

    //读取模板的信息
    //clear();
    QDomElement templateDom = d->namedItem("IconTemplates").toElement();
    QDomNode n = templateDom.firstChild();
    for(int i = 0;!n.isNull();n=n.nextSibling(),i++)
    {
        QDomElement e = n.toElement();
        QUuid uuid = QUuid(e.attribute("UUID"));
        HIconTemplate *pIconTemplate = new HIconTemplate(uuid);//需要定义一个不带uuid的参数
        if(!pIconTemplate) continue;
        pIconTemplate->readXml(&e);
        pIconTemplateList.append(pIconTemplate);
    }


    QDomElement elementsDom = d->namedItem("Elements").toElement();
    QDomNode n1 = elementsDom.firstChild();
    for(int i = 0;!n1.isNull();n1=n.nextSibling(),i++)
    {
        QDomElement e = n1.toElement();
        QString strTagName = e.tagName();
        QString strUuid = e.attribute("Uuid");
        HBaseObj* pObj = newObj(strTagName,strUuid);
        if(!pObj) continue;
        pObj->readXml(&e);
        pObjList.append(pObj);
    }
}

void HGraph::writeXmlFile(const QString& fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return;
    QTextStream dsm(&file);
    QDomDocument doc;
    QTextCodec* c = QTextCodec::codecForLocale();
    QString strLocal = QString("version=\"1.0\" encoding=\"GB2312\"");
    QDomProcessingInstruction instruct = doc.createProcessingInstruction("xml",strLocal);
    doc.appendChild(instruct);
    QDomElement root = doc.createElement("IconTemplate");
    if(root.isNull())
        return;
    doc.appendChild(root);
    writeXml(&root);
    dsm.setCodec("GB2312");
    doc.save(dsm,4);
    file.close();
}

void HGraph::writeXml(QDomElement *d)
{
    if(!d)
        return;
    d->setAttribute("GraphName",sName);
    d->setAttribute("GraphID",nGraphID);
    d->setAttribute("GraphWidth",nGraphWidth);
    d->setAttribute("GraphHeight",nGraphHeight);
    d->setAttribute("RefreshInterval",nRefreshInterval);
    d->setAttribute("FillColor",strFillColor);

    //再创建模板的xml结构
    QDomElement templateDom = d->ownerDocument().createElement("IconTemplates");
    d->appendChild(templateDom);
    for(int i = 0; i < pIconTemplateList.count();i++)
    {
        HIconTemplate* iconTemplate = (HIconTemplate*)pIconTemplateList[i];
        iconTemplate->writeXml(&templateDom);
    }

    //创建复杂和简单图元
    QDomElement elementDom = d->ownerDocument().createElement("Elements");
    d->appendChild(elementDom);
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->writeXml(&elementDom);
    }
}


void HGraph::setGraphName(const QString& name)
{
    sName = name;
}

QString HGraph::getGraphName()
{
    return sName;
}

QString HGraph::getFillColor()
{
    return strFillColor;
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


HBaseObj* HGraph::newObj(QString tagName,const QString &strUuid)
{
    quint8 drawShape = enumNo;
    if(tagName == "Line")
        drawShape = enumLine;
    else if(tagName == "Rectangle")
        drawShape = enumRectangle;
    else if(tagName == "Ellipse")
        drawShape = enumEllipse;
    else if(tagName == "Circle")
        drawShape = enumCircle;
    else if(tagName == "Polyline")
        drawShape = enumPolyline;
    else if(tagName == "Arc")
        drawShape = enumArc;
    else if(tagName == "Pie")
        drawShape = enumPie;
    else if(tagName == "Text")
        drawShape = enumText;
    else if(tagName == "Polygon")
        drawShape = enumPolygon;
    else if(tagName == "ComplexObj")
        drawShape = enumComplex;
    return newObj(drawShape,strUuid);
}

HBaseObj* HGraph::newObj(int nObjType,const QString &strUuid)
{
    HBaseObj* pObj = NULL;
    if(nObjType == enumLine)
    {
        pObj = new HLineObj();
    }
    else if(nObjType == enumRectangle)
    {
        pObj = new HRectObj();
    }
    else if(nObjType == enumEllipse)
    {
        pObj = new HEllipseObj();
    }
    else if(nObjType == enumCircle)
    {
        pObj = new HCircleObj();
    }
    else if(nObjType == enumPolygon)
    {
        pObj = new HPolygonObj();
    }
    else if(nObjType == enumPolyline)
    {
        pObj = new HPolylineObj();
    }
    else if(nObjType == enumArc)
    {
        pObj = new HArcObj();
    }
    else if(nObjType == enumPie)
    {
        pObj = new HPieObj();
    }
    else if(nObjType == enumText)
    {
        pObj = new HTextObj();
    }
    else if(nObjType == enumComplex)
    {
        //先获取icontemplate
        HIconTemplate* icontemp = findIconTemplate(QUuid(strUuid));
        pObj = new HIconComplexObj(icontemp);
    }
    pObj->setShapeType((DRAWSHAPE)nObjType);

    return pObj;
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
    return pIconTemplateList.count();
}

HIconTemplate* HGraph::IconTemplateAt(int index)
{
    return pIconTemplateList[index];
}

HIconTemplate* HGraph::findIconTemplate(const QUuid& uid)
{
    QString struuid = uid.toString();
    for(int i = 0; i < pIconTemplateList.count();i++)
    {
        HIconTemplate* iconTemplate = (HIconTemplate*)pIconTemplateList[i];
        if(iconTemplate->getUuid() == struuid)
            return iconTemplate;
    }
    return NULL;
}

HIconTemplate* HGraph::addIconTemplate(HIconTemplate* temp)
{
    HIconTemplate *iconTemplate = NULL;
    iconTemplate = findIconTemplate(temp->getUuid());
    if(iconTemplate)
        return iconTemplate;
    iconTemplate = new HIconTemplate(temp->getUuid());
    iconTemplate->copyTo(temp);
    return iconTemplate;
}

void HGraph::removeIconTemplate(HIconTemplate* temp)
{
    HIconTemplate *iconTemplate = NULL;
    iconTemplate = findIconTemplate(temp->getUuid());
    if(iconTemplate)
    {
        pIconTemplateList.removeOne(iconTemplate);
        delete iconTemplate;
        iconTemplate = NULL;
    }
}

void HGraph::clearIconTemplate()
{
    while(!pIconTemplateList.isEmpty())
        delete pIconTemplateList.takeFirst();
    pIconTemplateList.clear();
}

void HGraph::resetIconTemplate()
{
    clearIconTemplate();
}
