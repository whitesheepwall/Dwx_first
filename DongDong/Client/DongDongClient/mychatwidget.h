#ifndef MYCHATWIDGET_H
#define MYCHATWIDGET_H

#include <QWidget>

namespace Ui {
class MyChatWidget;
}

class MyChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyChatWidget(QWidget *parent = nullptr);
    ~MyChatWidget();
    void setInfo(QString, QPixmap);
    QString getId();
    QPixmap getPic();

private:
    Ui::MyChatWidget *ui;

    QString id;
    QPixmap pic;
};

#endif // MYCHATWIDGET_H
