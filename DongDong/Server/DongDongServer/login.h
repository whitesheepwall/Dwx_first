#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private:
    Ui::Login *ui;

signals:
    void managerLoginSuccess();
};

#endif // LOGIN_H
