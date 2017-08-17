#ifndef DIALONGSAMPCONF_H
#define DIALONGSAMPCONF_H

#include <QWidget>

namespace Ui {
class DialongSampConf;
}

class DialongSampConf : public QWidget
{
    Q_OBJECT
    
public:
    explicit DialongSampConf(QWidget *parent = 0);
    ~DialongSampConf();
    
private:
    Ui::DialongSampConf *ui;
};

#endif // DIALONGSAMPCONF_H
