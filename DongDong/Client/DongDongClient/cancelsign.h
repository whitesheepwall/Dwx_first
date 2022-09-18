#ifndef CANCELSIGN_H
#define CANCELSIGN_H

#include <QLabel>
#include <QMouseEvent>

class CancelSign : public QLabel
{
    Q_OBJECT
public:
    explicit CancelSign(QWidget *parent = nullptr);

    void mouseReleaseEvent(QMouseEvent*);
    void enterEvent(QEnterEvent*);
    void leaveEvent(QEvent*);

signals:
    void clicked();
};

#endif // CANCELSIGN_H
