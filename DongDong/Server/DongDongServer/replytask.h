#ifndef REPLYTASK_H
#define REPLYTASK_H

#include <QObject>
#include <globalvariable.h>

class ReplyTask : public QObject
{
    Q_OBJECT
public:
    explicit ReplyTask(QObject *parent = nullptr);

    void working();

signals:

};

#endif // REPLYTASK_H
