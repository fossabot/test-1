#ifndef MY_SPEAKER_H
#define MY_SPEAKER_H

#include <QThread>
#define MAX_BUF 64
class my_speaker : public QThread
{
    Q_OBJECT
    ~my_speaker();
public:
    explicit my_speaker(QObject *parent = 0);
    void run();
    bool stop;
    bool play;
    int fd;
    void speakerInit(int gpio = 67);

signals:
    //void ButtonPress(int);
public slots:
    
};

#endif // MY_SPEAKER_H
