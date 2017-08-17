#include "my_qlabel.h"

My_QLabel::My_QLabel(QWidget *parent) :
    QLabel(parent)
{
    qDebug() << "khoi tao";
}

void My_QLabel::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "click" + ev->x();
    emit mouse_Press();
}


