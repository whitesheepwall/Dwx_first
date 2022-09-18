#include "globalvariable.h"

GlobalVariable::GlobalVariable()
{

}

QMap<QString,QTcpSocket*> GlobalVariable::clientRequestMap;
QHash<QString,QMap<QString,QTcpSocket*>> GlobalVariable::groupsMembersList;
QMutex GlobalVariable::clientMutex;
QMutex GlobalVariable::groupMutex;
QMutex GlobalVariable::msgQMutex;

QByteArray GlobalVariable::defaultGroupIcon;
QByteArray GlobalVariable::defaultHeadIcon;
QQueue<QMap<QString,QString>> GlobalVariable::msgQ;

void GlobalVariable::initVariable()
{
    QPixmap pix;
    pix.load(":/headIcon/res/groupIcon.png");
    //GlobalVariable::pix1->load(":/headIcon/res/groupIcon.png");
    QBuffer buffer2(&GlobalVariable::defaultGroupIcon);
    buffer2.open(QIODevice::WriteOnly);
    //GlobalVariable::pix1->save(&buffer2,"png");
    pix.save(&buffer2,"png");

    pix.load(":/headIcon/res/defaultHeadIcon.png");
    //GlobalVariable::pix1->load(":/headIcon/res/defaultHeadIcon.png");
    QBuffer buffer(&GlobalVariable::defaultHeadIcon);
    buffer.open(QIODevice::WriteOnly);
    //GlobalVariable::pix1->save(&buffer,"png");
    pix.save(&buffer,"png");
}
