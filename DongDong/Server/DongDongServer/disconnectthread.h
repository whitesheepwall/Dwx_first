#ifndef DISCONNECTTHREAD_H
#define DISCONNECTTHREAD_H
#include <QObject>
#include <QRunnable>
#include <globalvariable.h>
#include <QTcpSocket>

class DisconnectThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit DisconnectThread(QTcpSocket *t);

protected:
    void run() override;

private:
    QTcpSocket *temp;

signals:
};

#endif // DISCONNECTTHREAD_H
