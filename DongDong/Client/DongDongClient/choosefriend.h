#ifndef CHOOSEFRIEND_H
#define CHOOSEFRIEND_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class ChooseFriend;
}

class ChooseFriend : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseFriend(QWidget *parent = nullptr);
    ~ChooseFriend();

    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEnterEvent*);
    void leaveEvent(QEvent*);
    void setInfo(QString,QString,QPixmap);
    void changeChooseState();
    bool isChoosed;

    QString id;
    QString remark;
    QPixmap pic;//头像
    QPixmap pix;//选中标识

private:
    Ui::ChooseFriend *ui;

signals:
    void choosed(QString,QString,QPixmap);
    void cancelChoosed(QString);
};

#endif // CHOOSEFRIEND_H
