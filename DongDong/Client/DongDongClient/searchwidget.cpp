#include "searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent)
    : QLineEdit{parent}
{
    setStyleSheet("QLineEdit{"
                  "background-color: rgb(226, 226, 226);"
                  "border-radius:5px;}");
    setFixedSize(200,25);
    setPlaceholderText("搜索");
}

void SearchWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit changePage();
    }
}
