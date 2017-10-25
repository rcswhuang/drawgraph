#ifndef HGRAPH_H
#define HGRAPH_H

#include <QObject>
class QPainter;
class HBaseObj;
class HIconTemplate;
class QDataStream;
class QDomElement;
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

	virtual void Draw(QPainter* p);

    virtual void clear();


    virtual void addObj(HBaseObj* pObj);

    virtual void removeObj(HBaseObj* pObj);

    qreal zoomScale();

    void setZoomScale(qreal f);

public:
    QString sName;

    bool bGrid;

    int nGridWidth;

    QList<HIconTemplate*> pIconTemplateList;
};

#endif // HGRAPH_H
