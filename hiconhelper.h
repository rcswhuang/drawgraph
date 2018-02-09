#ifndef HICONHELPER_H
#define HICONHELPER_H

#include <QPixmap>
#include <QString>
class HIconHelper
{
public:
    HIconHelper();

public:
    static HIconHelper* m_pInstance;
    static HIconHelper* Instance();

public:
    QPixmap iconPixmap(const QString& strType,const QString& uuid,const QSizeF &size = QSizeF(0,0),int nCurPattern = 0);

};

#endif // HICONHELPER_H
