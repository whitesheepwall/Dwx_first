#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QLineEdit>
#include <QMouseEvent>

class SearchWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent*);

signals:
    void changePage();
    void pressEnter(QString);

};

#endif // SEARCHWIDGET_H
