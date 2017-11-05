#include "hgraph.h"
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
#include <QTextCodec>
#include "H5IconGui/hicontemplate.h"
#include "H5IconGui/hbaseobj.h"
#include "H5IconGui/hiconobj.h"
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

void HGraph::WriteXmlFile(const QString& fileName)
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


HBaseObj* HGraph::newObj(QString tagName,QString strUuid)
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

HBaseObj* HGraph::newObj(int nObjType,QString strUuid)
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
        pObj = new HIconComplexObj();
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
