#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H
#include <QString>
#include <QTcpSocket>
#include <QMap>
#include <QHash>
#include <QMutex>
#include <QPixmap>
#include <QBuffer>
#include <QQueue>

class GlobalVariable
{
public:
    GlobalVariable();

    static QMap<QString,QTcpSocket*> clientRequestMap;
    static QHash<QString,QMap<QString,QTcpSocket*>> groupsMembersList;
    static QMutex clientMutex;
    static QMutex groupMutex;
    static QMutex msgQMutex;

    static QByteArray defaultGroupIcon;
    static QByteArray defaultHeadIcon;
    static QQueue<QMap<QString,QString>> msgQ;

    static void initVariable();
};

#endif // GLOBALVARIABLE_H
