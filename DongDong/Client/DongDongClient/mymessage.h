#ifndef MYMESSAGE_H
#define MYMESSAGE_H

#include <QWidget>

namespace Ui {
class MyMessage;
}

class MyMessage : public QWidget
{
    Q_OBJECT

public:
    explicit MyMessage(QWidget *parent = nullptr);
    ~MyMessage();

    void setInfo(QString,QString,QPixmap);

private:
    Ui::MyMessage *ui;

    QString messageId;
};

#endif // MYMESSAGE_H
