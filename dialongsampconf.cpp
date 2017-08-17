#include "dialongsampconf.h"
#include "ui_dialongsampconf.h"

DialongSampConf::DialongSampConf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialongSampConf)
{
    ui->setupUi(this);
}

DialongSampConf::~DialongSampConf()
{
    delete ui;
}
