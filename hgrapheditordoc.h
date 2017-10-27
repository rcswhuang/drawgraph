﻿#ifndef HGRAPHEDITORDOC_H
#define HGRAPHEDITORDOC_H

#include <QObject>
class HStation;
class HBaseObj;
class HIconGraphicsItem;
class HIconTemplate;
class QDomElement;
class HStation;
class HGraph;
//主要是对所有文档的管理类，包含所有模板文件，图符文件，以及图形文件
//具备一些相关的操作来支持对应的管理对象
//图形管理文档对象 主要目标是图形文件的管理，对于厂站、模板信息都有对应的对象进行操作

/*
 * 初始化 加载所有厂站的信息，所有画面的信息，所有模板的信息(考虑启动画面)
 * 当新建一个新画面时，提供HGraph对象，初始化这个HGraph对象(包含背景等信息)
 *
*/
class HGraphEditorDoc : QObject
{
public:
    HGraphEditorDoc();
    virtual ~HGraphEditorDoc();

public:
    //加载厂站信息
    void loadStation();

    //加载模板信息
    void loadIconTemplate();

    //加载画面信息
    void loadAllGraph();

    //新建画面
    void addGraph(const QString& name);

    //删除画面
    void delGraph(const QString& name,const int id);

    //打开画面
    void openGraph(const QString& name,const int id);

    //拷贝画面
    void copyGraph(HGraph* graph);

protected:
    //厂站列表
    QList<HStation*> pStationList;

    //模板列表 读取模板信息后保存的
    QList<HIconTemplate*> pIconTemplateList;

    //图形文件列表
    QList<HGraph*> pGraphList;

};

#endif // HGRAPHEDITORDOC_H
