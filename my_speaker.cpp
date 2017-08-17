#include "my_speaker.h"
#include <fcntl.h>
#include <QMutex>

my_speaker::my_speaker(QObject *parent) :
    QThread(parent)
{

    stop = false;
    play = false;
    speakerInit();
}

my_speaker::~my_speaker()
{
    // giai phong GPIO dang su dung
    char buf[MAX_BUF];

    fd = open("/sys/class/gpio/unexport", O_WRONLY);

    sprintf(buf, "%d", 67);

    write(fd, buf, strlen(buf));

    close(fd);
}

void my_speaker::run()
{



    // Set GPIO high status
    write(fd, "1", 1);

    // delay
    msleep(20);

    // Set GPIO low status
    write(fd, "0", 1);


    this->play = false;

}

void my_speaker::speakerInit(int gpio)
{

    char buf[MAX_BUF];

    fd = open("/sys/class/gpio/export", O_WRONLY);

    sprintf(buf, "%d", gpio);

    write(fd, buf, strlen(buf));

    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);

    // Set out direction
    write(fd, "out", 3);
    // Set in direction
    //write(fd, "in", 2);

    sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);

    fd = open(buf, O_WRONLY);
}
