#ifndef SETINFOMATION_H
#define SETINFOMATION_H

#include <QWidget>
#include <QFileDialog>

namespace Ui {
class SetInfomation;
}

class SetInfomation : public QWidget
{
    Q_OBJECT

public:
    explicit SetInfomation(QWidget *parent = nullptr);
    ~SetInfomation();

    void setInfo(QString,QString,QString,QString);
    void setPic(QPixmap);

private:
    Ui::SetInfomation *ui;
    QString id;
    QString nickname;
    QString sex;
    QString signature;
    QString fileName;

signals:
    void newInfo(QString,QString,QString);
    void newPic(QString,QPixmap);
};

#endif // SETINFOMATION_H
