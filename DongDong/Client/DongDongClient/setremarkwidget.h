#ifndef SETREMARKWIDGET_H
#define SETREMARKWIDGET_H

#include <QWidget>

namespace Ui {
class SetRemarkWidget;
}

class SetRemarkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetRemarkWidget(QWidget *parent = nullptr);
    ~SetRemarkWidget();

    void setOriginalRemark(QString);

private:
    Ui::SetRemarkWidget *ui;

signals:
    void newRemark(QString);
};

#endif // SETREMARKWIDGET_H
