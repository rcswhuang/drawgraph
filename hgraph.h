#ifndef HGRAPH_H
#define HGRAPH_H

#include <QObject>
class QPainter;
class HBaseObj;
class HIconTemplate;
class HGraph : public QObject
{
public:
    HGraph();
    ~HGraph();

public:
    virtual void Read();

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
