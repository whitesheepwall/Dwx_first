#ifndef CHATTHREAD_H
#define CHATTHREAD_H

#include <QObject>
#include <QRunnable>
#include "globalvariable.h"

class ChatThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit ChatThread(QString req, QString id);

protected:
    void run() override;

private:
    QString str;
    QString srcId;

signals:

};

#endif // CHATTHREAD_H
