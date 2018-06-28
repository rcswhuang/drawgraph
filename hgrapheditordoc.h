#ifndef HGRAPHEDITORDOC_H
#define HGRAPHEDITORDOC_H

#include <QObject>
class HStation;
class HBaseObj;
class HIconGraphicsItem;
class HIconTemplate;
class QDomElement;
class HStation;
class HGraph;
class HGraphEditorMgr;
//主要是对所有文档的管理类，包含所有模板文件，图符文件，以及图形文件
//具备一些相关的操作来支持对应的管理对象
//图形管理文档对象 主要目标是图形文件的管理，对于厂站、模板信息都有对应的对象进行操作

/*
 * 初始化 加载所有厂站的信息，所有画面的信息，所有模板的信息(考虑启动画面)
 * 初始化临时HGraph
 * 当新建一个新画面时，提供HGraph对象，初始化这个HGraph对象(包含背景等信息)，返回给HGraph
 *临时HGraph的内容提供给view/scene,供显示。
 * 打开画面，删除都是如此，代码此处实现，可以供其他对象调用对应接口
*/
class HGraphEditorDoc : QObject
{
public:
    HGraphEditorDoc(HGraphEditorMgr* mgr);
    virtual ~HGraphEditorDoc();

public:
    //加载厂站信息
    void loadStation();

    //厂站ID获取厂站
    HStation* getStation(quint16 wStationID);

    //厂站地址获取厂站
    HStation* getRtu(quint16 wStationAddress);

    //索引厂站
    HStation* findStation(int nIndex);

    //加载模板信息
    void loadIconTemplate();

    //寻找模板 uuid
    HIconTemplate* findIconTemplate(const QString& uuid);

    //加载画面信息
    void loadAllGraph();

    //保存画面信息
    void saveAllGraph();

    //或者画面ID
    int getGraphID();

    //查找画面
    HGraph* findGraph(int graphID);

    //
    HGraph* findGraph(const QString& graphName);

    //新建画面
    HGraph* addGraph(const QString& name);

    //删除画面
    bool delGraph(const QString& name,const int id);

    //打开画面
    bool openGraph(const QString& name,const int id);

    //拷贝画面
    void copyGraph(HGraph* graph);

    //保存画面
    void saveCurGraph();

    //导入画面
    int importGraph(const QString& name);

    //更新所有画面里面的模板信息
    void updateGraphList();

    //获取模板对象列表
    QList<HIconTemplate*> getIconTemplateList();

    //获取画面列表
    QList<HGraph*> getGraphList();

    //获取当前画面
    HGraph* getCurGraph();

    //判断graph文件是否修改
    bool isGraphModify();

public:
    //厂站列表
    QList<HStation*> pStationList;

    //模板列表 读取模板信息后保存的
    QList<HIconTemplate*> pIconTemplateList;

    //图形文件列表
    QList<HGraph*> pGraphList;

protected:

    HGraphEditorMgr* pGraphEditorMgr;

    //当前文件
    HGraph* pCurGraph;

};

#endif // HGRAPHEDITORDOC_H
