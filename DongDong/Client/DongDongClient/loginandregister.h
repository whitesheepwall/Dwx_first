#ifndef LOGINANDREGISTER_H
#define LOGINANDREGISTER_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class LoginAndRegister;
}

class LoginAndRegister : public QWidget
{
    Q_OBJECT

public:
    explicit LoginAndRegister(QWidget *parent = nullptr);
    ~LoginAndRegister();

    void onLogin();
    void onRegister();
    void loginResult(bool);
    void registerResult(bool);

private:
    Ui::LoginAndRegister *ui;

signals:
    void loginInfo(QString,QString);
    void registerInfo(QString,QString);
};

#endif // LOGINANDREGISTER_H
