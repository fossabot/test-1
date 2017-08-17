#include "dialogsampconf.h"
#include "ui_dialogsampconf.h"
#include <QDebug>

DialogSampConf::DialogSampConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogSampConf)
{

    ui->setupUi(this);

    ui->lbText->setText(sText);
    qDebug() << "ok: " << sText;

    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(close()));
    t->start(1000);

}

DialogSampConf::~DialogSampConf()
{
    qDebug() << "close";
    delete ui;
}

void DialogSampConf::SetText(QString text)
{
    this->ui->lbText->setText(text);
}

void DialogSampConf::SetAlpha(int iAlpha)
{

}
void DialogSampConf::SetTimer(int iTimer)
{

}

void DialogSampConf::paintEvent(QPaintEvent *event)
{
    QColor backgroundColor = palette().light().color().cyan();
    backgroundColor.setAlpha(150);
    QPainter customPainter(this);
    customPainter.fillRect(rect(),backgroundColor);
}



