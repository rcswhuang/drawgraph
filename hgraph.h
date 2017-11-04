#ifndef HGRAPH_H
#define HGRAPH_H

#include <QObject>
class QPainter;
class HBaseObj;
class HIconTemplate;
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

    //设置ID
    void setGraphID(int id);

    //获取ID
    int getGraphID();
public:
    virtual void ReadDataFile(const QString&);

	virtual void WriteDateFile(const QString&);

	virtual void ReadXmlFile(const QString&);

	virtual void WriteXmlFile(const QString&);

	virtual void ReadData(int,QDataStream *d);

	virtual void WriteData(int,QDataStream *d);

    virtual void readXml(QDomElement *d);
    
    virtual void writeXml(QDomElement *d);


    void setGraphName(const QString& name);

    QString getGraphName();

    void Draw(QPainter* p);

    void clear();

    void copyTo(HBaseObj* obj);

    HBaseObj* newObj(int nObjType);

    HBaseObj* newObj(QString tagName);

    void addObj(HBaseObj* pObj);

    void removeObj(HBaseObj* pObj);

    //模板部分
    int iconTemplateNum();

    HIconTemplate* IconTemplateAt(int index);

    virtual HIconTemplate* findIconTemplate(const QUuid& uid);

    virtual HIconTemplate* addIconTemplate(HIconTemplate* temp);

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
    //所有图元信息
    QList<HBaseObj*> pObjList;

    //所有模板信息
    QList<HIconTemplate*> pIconTemplateList;
    //QList<HStation> stationList;





};

#endif // HGRAPH_H
