#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QWidget>
#include <QMouseEvent>
#include "config.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDateTime>

namespace Ui {
class Conversation;
}

class Conversation : public QWidget
{
    Q_OBJECT

public:
    explicit Conversation(QWidget *parent = nullptr);
    ~Conversation();

    void mouseReleaseEvent(QMouseEvent*);
    void setInfo(QString, QString, QPixmap, QString, QString);
    void setType(int);
    void updateInfo(QString,QString);
    void enterEvent(QEnterEvent*);
    void leaveEvent(QEvent*);
    bool getIsChoosed();
    void setIsChoosed(bool);
    QPixmap getPic();

private:
    Ui::Conversation *ui;
    QString id;
    QString remark;
    QPixmap pic;
    QString time;
    QString content;
    int type;

    bool isChoosed;

signals:
    void friendInfo(QString,QString,QPixmap, int);
    void isChoosedChanged();
};

#endif // CONVERSATION_H
