#include <QtCore>
#include "gpiocontrol.h"
#include "sys/time.h"
#include <fcntl.h>



GPIOControl::GPIOControl(QObject *parent) :
    QThread(parent)
{

}

void GPIOControl::run()
{

    // khai bao cho gpio_shutter
    struct timeval  tv1_shutter, tv2_shutter;
    gettimeofday(&tv1_shutter, NULL);
    double thoigian_shutter = 0;

    this->stop = false;     // reset gia tri.
    bool flag_shutter = true;       // flag kiem tra lan nhan nut,
    int fd_shutter;
    int gpio_shutter = 26;
    char value_shutter = '0';
    char buf_shutter[MAX_BUF];

    // khai bao cho gpio_sample
    int gpio_sample = 65;
    int fd_sample;
    char buf_sample[MAX_BUF];
    char value_sample = '0';
    char value_sample_pre;
    bool flag_sample = true;

    // init gpio
    InitGPIOShuter(gpio_shutter);
    InitGPIOSample(gpio_sample);


    // doc du lieu cho gpio_shutter
    sprintf(buf_shutter, "/sys/class/gpio/gpio%d/value", gpio_shutter);
    fd_shutter = open(buf_shutter, O_RDONLY);

    // doc du lieu cho gpio_sample
    sprintf(buf_sample, "/sys/class/gpio/gpio%d/value", gpio_sample);
    fd_sample = open(buf_sample, O_RDONLY);

    while(1)
    {
        QMutex mutex;
        mutex.lock();
        if (this->stop) break;
        mutex.unlock();

        // doc gia tri gpio_shutter
        read(fd_shutter, &value_shutter, 1);

        // doc gia tri gpio_sample
        read(fd_sample, &value_sample, 1);

        // lay value lan dau
        if (flag_sample == true)
        {
            value_sample_pre = value_sample;
        }

        // kiem tra trang thai nut sample cho lan chay dau tien
        //qDebug() << flag_sample << value_sample;
        if (flag_sample == true && value_sample == '1')
        {
            emit ButtonPress(3);
            qDebug() << "gui lan dau......................";
            value_sample_pre = value_sample;
        }
        else if (value_sample != value_sample_pre)
        {
            // tinh thoi gian
            gettimeofday(&tv2_shutter, NULL);
            thoigian_shutter = (double) (tv2_shutter.tv_usec - tv1_shutter.tv_usec) / 1000000 +
                                    (double) (tv2_shutter.tv_sec - tv1_shutter.tv_sec);

            // cap nhat thoi gian moi cho tv1
            gettimeofday(&tv1_shutter, NULL);

            if (thoigian_shutter > 0.25)
            {
                emit ButtonPress(2);
            }
        }

        flag_sample = false;

        value_sample_pre = value_sample;
        //qDebug() << "sample" << value_sample;

        if(value_shutter == '0')
        {
             // Current GPIO status low
        }
        else
        {
             // Current GPIO status high
            if (flag_shutter == false)
            {
                // tinh thoi gian
                gettimeofday(&tv2_shutter, NULL);
                thoigian_shutter = (double) (tv2_shutter.tv_usec - tv1_shutter.tv_usec) / 1000000 +
                                        (double) (tv2_shutter.tv_sec - tv1_shutter.tv_sec);

                // cap nhat thoi gian moi cho tv1
                gettimeofday(&tv1_shutter, NULL);

                //qDebug() << "thoigian: " << thoigian_shutter;
                if (thoigian_shutter > 0.25)
                {
                    // kiem tra thoi gian du lau
                    emit ButtonPress(1);
                }
            }
            else
                flag_shutter = false;
        }
        fflush(stdout);
        lseek(fd_shutter, 0, SEEK_SET);
        lseek(fd_sample, 0, SEEK_SET);
    }

    close(fd_shutter);
}

void GPIOControl::InitGPIOShuter(int gpio = 26)
{
    int fd;

    char buf[MAX_BUF];

    fd = open("/sys/class/gpio/export", O_WRONLY);

    sprintf(buf, "%d", gpio);

    write(fd, buf, strlen(buf));

    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);

    // Set out direction
    //write(fd, "out", 3);
    // Set in direction
    write(fd, "in", 2);

    sprintf(buf, "/sys/class/gpio/gpio%d/edge", gpio);

    fd = open(buf, O_WRONLY);

    write(fd, "falling", 7);

    close(fd);
}

void GPIOControl::InitGPIOSample(int gpio = 65)
{
    int fd;

    char buf[MAX_BUF];

    fd = open("/sys/class/gpio/export", O_WRONLY);

    sprintf(buf, "%d", gpio);

    write(fd, buf, strlen(buf));

    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);

    // Set out direction
    //write(fd, "out", 3);
    // Set in direction
    write(fd, "in", 2);

    sprintf(buf, "/sys/class/gpio/gpio%d/edge", gpio);

    fd = open(buf, O_WRONLY);

    write(fd, "falling", 7);

    close(fd);
}

