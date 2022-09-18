#include "mychatwidget.h"
#include "ui_mychatwidget.h"

MyChatWidget::MyChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyChatWidget)
{
    ui->setupUi(this);
}

MyChatWidget::~MyChatWidget()
{
    delete ui;
}

void MyChatWidget::setInfo(QString id, QPixmap pic)
{
    this->id = id;
    this->pic = pic;
}

QString MyChatWidget::getId()
{
    return id;
}

QPixmap MyChatWidget::getPic()
{
    return pic;
}

