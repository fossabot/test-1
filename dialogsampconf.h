#ifndef DIALOGSAMPCONF_H
#define DIALOGSAMPCONF_H

#include <QWidget>
#include <QtGui>
#include <QtCore>

namespace Ui {
class DialogSampConf;
}

class DialogSampConf : public QWidget
{
    Q_OBJECT

public:
    explicit DialogSampConf(QWidget *parent = 0);
    ~DialogSampConf();
    QString sText;
    void SetText(QString);
    void SetAlpha(int iAlpha);
    void SetTimer(int iTimer);
    
private slots:




private:
    Ui::DialogSampConf *ui;

protected:
    void paintEvent(QPaintEvent* event);
};

#endif // DIALOGSAMPCONF_H
