#include "cancelsign.h"

CancelSign::CancelSign(QWidget *parent)
    : QLabel{parent}
{
    setCursor(QCursor(Qt::PointingHandCursor));

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(216,216,216));
    palette.setColor(QPalette::WindowText, QColor(255,255,255));
    this->setPalette(palette);
}

void CancelSign::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton){
        emit clicked();
    }
}

void CancelSign::enterEvent(QEnterEvent *e)
{
    if(e->type() == QEnterEvent::Enter){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(200,200,200));
        this->setPalette(palette);
    }
}

void CancelSign::leaveEvent(QEvent *e)
{
    if(e->type() == QEvent::Leave){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(216,216,216));
        this->setPalette(palette);
    }
}
