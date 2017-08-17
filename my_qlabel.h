#ifndef MY_QLABEL_H
#define MY_QLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>


class My_QLabel : public QLabel
{
    Q_OBJECT
public:
    explicit My_QLabel(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *ev);
signals:
    void mouse_Press();
};

#endif // MY_QLABEL_H
