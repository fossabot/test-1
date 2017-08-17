#ifndef GPIOCONTROL_H
#define GPIOCONTROL_H
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <QThread>


using namespace std;

class GPIOControl : public QThread
{
    Q_OBJECT
public:
    explicit GPIOControl(QObject *parent = 0);
    void run();
    bool stop;
    int idemtimer;
    void InitGPIOShuter(int gpio);
    void InitGPIOSample(int gpio);
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

private:


signals:
    void ButtonPress(int);
public slots:

    
};

#endif // GPIOCONTROL_H
