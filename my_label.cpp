#include "my_label.h"

my_label::my_label(QWidget *parent) :
    QLabel(parent)
{
    dTime = 0;
    gettimeofday(&tv1, NULL);
}

void my_label::mouseReleaseEvent(QMouseEvent *ev)
{

    // tinh thoi gian
    gettimeofday(&tv2, NULL);
    dTime = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec);

    // cap nhat thoi gian moi cho tv1
    gettimeofday(&tv1, NULL);
    qDebug() << dTime;
    if (dTime > 0.3)
    {
        emit mouse_release();
    }
}
