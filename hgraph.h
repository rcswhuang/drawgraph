#ifndef HGRAPH_H
#define HGRAPH_H

#include <QObject>
#include "publicdata.h"
#include "hiconapi.h"
#include "hicontemplate.h"
#include "hiconsymbol.h"
class QPainter;
class QDataStream;
class QDomElement;
/*
 * 记录单个画面信息，包含画面所含的模板列表信息，各种图元信息，颜色，背景等等信息
 *
*/
class HGraph : public QObject
{
public:
    HGraph(const QString& name);
    ~HGraph();
public:
    void setGraphID(int id);
    int getGraphID();
    void setModify(bool modify);
    bool getModify();
public:
    virtual void readDataFile(const QString&);

    virtual void writeDateFile(const QString&);

    virtual void readXmlFile(const QString&);

    virtual void writeXmlFile(const QString&);

    virtual void readData(int,QDataStream *d);

    virtual void writeData(int,QDataStream *d);

    virtual void readXml(QDomElement *d);
    
    virtual void writeXml(QDomElement *d);


    void setGraphName(const QString& name);

    QString getGraphName();

    QString getFillColor();

    void Draw(QPainter* p);

    void clear();

    void copyTo(HGraph* graph);

    HBaseObj* newObj(int nObjType,const QString &strUuid = QString());

    HBaseObj* newObj(QString tagName,const QString &strUuid = QString());

    void addObj(HBaseObj* pObj);

    void removeObj(HBaseObj* pObj);

    int getObjID();

    bool findObjID(int nObjID);

    //模板部分
    int iconTemplateNum();

    HIconTemplate* IconTemplateAt(int index);

    virtual HIconTemplate* findIconTemplate(const QUuid& uid);

    virtual void addIconTemplate(HIconTemplate* temp);

    virtual void removeIconTemplate(HIconTemplate* temp);

    virtual void clearIconTemplate();

    virtual void resetIconTemplate();


    //qreal zoomScale();

    //void setZoomScale(qreal f);

protected:
    QString sName; //名称

    int nGraphID;//画面ID

    int nGraphWidth;

    int nGraphHeight;

    int nRefreshInterval; //刷新间隔

    QString strFillColor;//背景填充颜色

    bool bModify; //是否修改，不需要保存

    QList<HBaseObj*> pObjList;//所有图元信息

    //缩放比例

    //所有模板信息
    QList<HIconTemplate*> pIconTemplateList;
    //QList<HStation> stationList;





};

#endif // HGRAPH_H
