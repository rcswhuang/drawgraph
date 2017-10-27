#ifndef HGRAPH_H
#define HGRAPH_H

#include <QObject>
class QPainter;
class HBaseObj;
class HIconTemplate;
class QDataStream;
class QDomElement;
class HIconGraphicsItem;

/*
 * 记录单个画面信息，包含画面所含的模板列表信息，各种图元信息，颜色，背景等等信息
 *
*/
class HGraph : public QObject
{
public:
    HGraph();
    ~HGraph();

public:
    virtual void ReadDataFile(const QString&);

	virtual void WriteDateFile(const QString&);

	virtual void ReadXmlFile(const QString&);

	virtual void WriteXmlFile(const QString&);

	virtual void ReadData(int,QDataStream *d);

	virtual void WriteData(int,QDataStream *d);

	virtual void ReadXml(QDomElement *d);
    
	virtual void WriteXml(QDomElement *d);

    void ReadDynamicXml(QDomElement* d);

    void WriteDynamicXml(QDomElement* d);

    void ReadDynamicData(int,QDataStream* d);

    void WriteDynamicData(int,QDataStream* d);

    void setGraphName(const QString& name);

    QString getGraphName();

    virtual void Draw(QPainter* p);

    virtual void clear();

    virtual void copyTo(HBaseObj* obj);

    virtual void addObj(HBaseObj* pObj);

    virtual void removeObj(HBaseObj* pObj);

    //模板部分
    int iconTemplateNum();

    HIconTemplate* IconTemplateAt(int index);

    virtual HIconTemplate* findIconTemplate(const QUuid& uid);

    virtual HIconTemplate* addIconTemplate(HIconTemplate* temp);

    virtual void removeIconTemplate(HIconTemplate* temp);

    virtual void clearIconTemplate();

    virtual void resetIconTemplate();


    qreal zoomScale();

    void setZoomScale(qreal f);

protected:
    QString strFileName;

    //所有图元信息
    QList<HIconGraphicsItem*> igItemList;

    //所有模板信息
    QList<HIconTemplate*> pIconTemplateList;
    //QList<HStation> stationList;

public:
    QString sName;

    bool bGrid;

    int nGridWidth;

    QColor bgColor;


};

#endif // HGRAPH_H
