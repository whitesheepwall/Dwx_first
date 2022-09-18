#ifndef INFORMATION_H
#define INFORMATION_H

#include <QMainWindow>

namespace Ui {
class Information;
}

class Information : public QMainWindow
{
    Q_OBJECT

public:
    explicit Information(QWidget *parent = nullptr);
    ~Information();

private:
    Ui::Information *ui;
};

#endif // INFORMATION_H
