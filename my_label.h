#ifndef MY_LABEL_H
#define MY_LABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>
#include <sys/time.h>

class my_label : public QLabel
{
    Q_OBJECT
public:
    explicit my_label(QWidget *parent = 0);
    
    void mouseReleaseEvent(QMouseEvent *ev);
    double dTime;
    struct timeval  tv1, tv2;
signals:
    void mouse_release();
public slots:
    
};

#endif // MY_LABEL_H
