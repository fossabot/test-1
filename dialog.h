#ifndef DIALOG_H
#define DIALOG_H

#include <string.h>
#include <QDialog>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QTimer>

#include "gpiocontrol.h"
#include "dialogsampconf.h"
#include "my_speaker.h"


using namespace cv;
/////////////////////////////////////////////////////////////////////////////////
namespace Ui {
class Dialog;
}

/////////////////////////////////////////////////////////////////////////////////
class Dialog : public QDialog
{
    Q_OBJECT


public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    GPIOControl         *mGPIOControl;
    my_speaker          *mMy_Speaker;

private:
    Ui::Dialog          *ui;
    DialogSampConf      *dialogSampConf;
    void CaptureSample();

    // cac ham va bien xu ly webcam
    VideoCapture        capwebcam;
    Mat                 matOriginal;
    Mat                 matProcessed;
    QTimer              *tmrTimer;
    int                 laymau;
    int                 height;
    int                 width;
    int                 cameraid;
    bool                dialogsamconf_isopen;
    vector<Rect>    vectorRect; //mask of current dock
public slots:
    void processFrameAnhUpdateGUI();
    void onButtonPress(int);



private slots:
    void getMaskInfo();
    void drawMask(cv::Mat dest, cv::Mat source);
    void on_btnSampConf_clicked();
    void on_btnStatitics_clicked();
    void key_release();
};

//////////////////////////////////////////////////////////////////////////////////
#endif // DIALOG_H
