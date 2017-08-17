#include "dialog.h"
#include "ui_dialog.h"
#include "detect_fuse.h"
#include "configadapter.h"
#include <QKeyEvent>
#include "QDebug"
#include <sys/time.h>
#include "my_label.h"
#include "my_speaker.h"


configAdapter config; //thong so cau hinh trong config.xml
detect_fuse detecter; //ham detect fuse.

bool isCaptureSample = false;
/////////////////////////////////////////////////////////////////////////////////
/// \brief Dialog::Dialog
/// \param parent
///
Dialog::Dialog(QWidget *parent) : QDialog(parent),    ui(new Ui::Dialog)
{
    ui          ->setupUi(this);

    dialogsamconf_isopen = false;

    // thiet lap background cho chuong trinh
    QPalette    pal;
    pal.setBrush(this->backgroundRole() , QBrush( QImage("/home/root/CODE/images/bg.png") ) );
    this        ->setPalette(pal);

    //init detecter;
    detecter.init_detect(config.histogram_bin_h, config.histogram_bin_s, config.histogram_range_h, config.histogram_range_s,config.algselect, config.histogram_nguong, config.compare);

    // thiet lap GPIO nut bam chup cho chuong trinh
    mGPIOControl    = new GPIOControl(this);
    connect(mGPIOControl, SIGNAL(ButtonPress(int)), this, SLOT(onButtonPress(int)));
    mGPIOControl    ->start();

    // thiet lap su kien click cua lbVideo
    connect(ui->lbVideo, SIGNAL(mouse_release()), this, SLOT(key_release()));

    // thiet lap speaker
    mMy_Speaker =   new my_speaker(this);
    mMy_Speaker     ->start();


    // mo webcam thong so duoc lay tu file config.xml
    capwebcam.open(config.cameraid);

    //set default value of system for camera
    capwebcam.set(CV_CAP_PROP_FRAME_WIDTH , config.imgWidth);
    capwebcam.set(CV_CAP_PROP_FRAME_HEIGHT, config.imgHeight);
    qDebug() << config.imgWidth;
    qDebug() << config.imgHeight;

    // tat auto focus, va thiet lap focus
    if (config.autofocus){
        system("v4l2-ctl --verbose --set-ctrl=focus_auto=1");
    } else {
        system("v4l2-ctl --verbose --set-ctrl=focus_auto=0");

        // set focus = 85, ung voi khoang cach cua ong chup
        QString focus_absolute = QString("v4l2-ctl -d 0 -c focus_absolute=%1").arg(config.focusabs);
        system(focus_absolute.toLatin1().data());
    }

     system("v4l2-ctl --set-ctrl=white_balance_temperature_auto=0");


    // kiem tra mo webcam thanh cong
    if (capwebcam.isOpened() == false)
    {
        // xuat thong bao cho nguoi su dung biet, khong mo duoc webcam
        ui->lbVideo->setText("error: capwebcam can not accessed successfully");
        return;
    }

    //get allMask;
    getMaskInfo();

    // khoi tao timer
    tmrTimer = new QTimer(this);

    // tao ket noi
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrameAnhUpdateGUI()));
    tmrTimer->start(config.timeStart);


}

/////////////////////////////////////////////////////////////////////////////////
/// \brief Dialog::processFrameAnhUpdateGUI
///
void Dialog::processFrameAnhUpdateGUI()
{
    // lay du lieu tu webcam
    capwebcam >> matOriginal;

    // kiem tra lay du lieu thanh cong
    if (matOriginal.empty() == true)
        return;

    Mat temp;
    // chuyen he mau bgr2rgb
    cv::cvtColor(matOriginal, temp, CV_BGR2RGB);

    //ve mask
    drawMask(temp, temp);

    // tao QImage tu cvMAt
    QImage qimgOriginal((uchar*)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);

    // in anh nhan duoc tu webcam ra GUI
    ui->lbVideo->setPixmap(QPixmap::fromImage(qimgOriginal.scaled(config.widthScale, config.heightScale,Qt::IgnoreAspectRatio, Qt::FastTransformation)));


}

void Dialog::onButtonPress(int number)
{
    //qDebug() << number;
    // phat tieng beep
    mMy_Speaker->start();

    // kiem tra dialog statitics dang mo thi tat di
    if (dialogsamconf_isopen == true)
    {
        // khoi dong lai camera
        tmrTimer->start(config.timeStart);

        // huy dialog statitics
        dialogSampConf->close();
        delete dialogSampConf;

        dialogsamconf_isopen = false;
    }
    else
    {
        if (number == 3)
        {
            qDebug() <<"change isCaptureSample";
            isCaptureSample = false;
            on_btnSampConf_clicked();
        }
        else if (number == 2)
        {
            on_btnSampConf_clicked();
        }
        else
        {
            if (isCaptureSample && number == 1 && tmrTimer->isActive() == true)
            {
                CaptureSample();
            }
            else
            {
                ui->lbnumber->setText("0");

                // kiem tra nguoi dung nhan nut lan 1.
                if (number == 1 && tmrTimer->isActive() == true)
                {
                    qDebug() <<"Xy Ly Anh. tmrTimer stopped" << number ;
                    //enter button
                    tmrTimer->stop();
                    Mat temp;
                    int numError = 0;

                    //lay hinh mau.
                    Mat samp = imread("/home/root/CODE/images/d01.png", 1);

                    temp = detecter.detect(vectorRect, matOriginal, samp, numError);
                    /* Hien thi anh chup duoc len tren giao dien */
                    // chuyen he mau bgr2rgb de show len QtLabel

                    cv::cvtColor(temp, temp, CV_BGR2RGB);

                    // tao QImage tu cvMAt
                    QImage qimgOriginal((uchar*)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);

                    // in anh nhan duoc tu webcam ra GUI
                    ui->lbVideo->setPixmap(QPixmap::fromImage(qimgOriginal.scaled(config.widthScale, config.heightScale ,Qt::IgnoreAspectRatio, Qt::FastTransformation)));

                    // kiem tra neu co loi thi thay doi mau background cua lbSample
                    if (numError > 0)
                    {
                        ui->lbisSample->setStyleSheet("QLabel { background-color : red; color : blue; }");
                        ui->lbnumber->setText(QString::number(numError));
                    }

                    // lay du lieu tu webcam
                    capwebcam >> matOriginal;
                    //imwrite("d01.png", matOriginal);
                }
                // kiem tra nguoi dung nhan nut lan 2.
                else if (number == 1 && tmrTimer->isActive() == false)
                {
                    // reset mau cho lbSample
                    ui->lbisSample->setStyleSheet("QLabel { background-color : none; color : none; }");

                    // khoi dong lai timer
                    tmrTimer->start(config.timeStart);
                }

            }
        }
    }

}

/////////////////////////////////////////////////////////////////////////////////
/// \brief Dialog::~Dialog
///
Dialog::~Dialog()
{
    // huy cac khoi tao cho webcam
    mGPIOControl->stop = true;
    mGPIOControl->exit();

    mMy_Speaker->exit();

    tmrTimer    ->stop();

    capwebcam.release();

    delete ui;
}




void Dialog::getMaskInfo(){
    QFile* file = new QFile("/home/root/CODE/images/d01.svg");

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug("Can not read this XML File SVG");
    }
    else {
        QXmlStreamReader xml(file);

        while(!xml.atEnd() && !xml.hasError()){
            QXmlStreamReader::TokenType token = xml.readNext();

            if (token == QXmlStreamReader::StartDocument){
                continue;
            }

            if (token == QXmlStreamReader::StartElement){
                if (xml.name() == "rect"){
                    QXmlStreamAttributes atr = xml.attributes();

                    Rect temp;

                    temp.x      = atr.value("x").toString().toInt();
                    temp.y      = atr.value("y").toString().toInt();
                    temp.height = atr.value("height").toString().toInt();
                    temp.width  = atr.value("width").toString().toInt();

                    vectorRect.push_back(temp);
                }
            }
        }
    }

    file->close();
}

void Dialog::drawMask(Mat dest, Mat source){
    dest = source;

    for (int i = 1; i < (int)vectorRect.size(); i++){
        rectangle(dest, vectorRect[i],Scalar(0,255,0),1,8,0); // ve mau xanh
    }

}



void Dialog::on_btnSampConf_clicked()
{
    // phat tieng beep
    mMy_Speaker->start();

    // kiem tra trang thai nut lay mau
    if (isCaptureSample)
    {
        //------------  Dialog Thong Bao
        // khoi tao dialog
        dialogSampConf = new DialogSampConf(this);
        dialogSampConf->SetText("Detect Mode!");
        dialogSampConf->showFullScreen();
        //---------------

        isCaptureSample = false;

        this->ui->lbisSample ->setText("Detect Mode!");

    }
    else
    {
        //------------  Dialog Thong Bao
        // khoi tao dialog
        dialogSampConf = new DialogSampConf(this);
        dialogSampConf->SetText("Capture Sample Mode!");
        dialogSampConf->showFullScreen();
        //---------------

        //set bien isSample = true
        isCaptureSample = true;

        this->ui->lbisSample->setText("Cap Sample!");

    }
}

void Dialog::CaptureSample()
{

    if (!imwrite("/home/root/CODE/images/d01.png", matOriginal))
    {
        qDebug() << "error write image sample!";
    }

    //------------  Dialog Thong Bao
    // khoi tao dialog
    dialogSampConf = new DialogSampConf(this);
    dialogSampConf->SetText("Success Capture Sample!");
    dialogSampConf->showFullScreen();
    //---------------

    this->ui->lbisSample->setText("Success Samp!");

}

void Dialog::on_btnStatitics_clicked()
{
    // phat tieng beep
    mMy_Speaker->start();

    // tat camera
    tmrTimer->stop();

    // khoi tao dialog statiticss
    dialogSampConf = new DialogSampConf(0);
    dialogSampConf->showFullScreen();
    dialogsamconf_isopen = true;
}

void Dialog::key_release()
{

    // phat tieng beep
    mMy_Speaker->start();

    // kiem tra trang thai lbvideo
    if (tmrTimer->isActive() == true)
    {
        // dung timer lai
        tmrTimer->stop();

        //------------  Dialog Thong Bao
        // khoi tao dialog
        dialogSampConf = new DialogSampConf(this);
        dialogSampConf->SetText("View Sample Mode!");
        dialogSampConf->showFullScreen();
        //---------------

        // doc anh mau len
        Mat samp = imread("/home/root/CODE/images/d01.png", 1);

        // chuyen he mau rgb sang bgr
        cv::cvtColor(samp, samp, CV_BGR2RGB);

        // tao QImage tu cvMAt
        QImage qimgOriginal((uchar*)samp.data, samp.cols, samp.rows, samp.step, QImage::Format_RGB888);

        // in anh nhan duoc tu webcam ra GUI
        ui->lbVideo->setPixmap(QPixmap::fromImage(qimgOriginal.scaled(config.widthScale, config.heightScale ,Qt::IgnoreAspectRatio, Qt::FastTransformation)));

        // thong bao trang thai
        ui->lbisSample->setText("View Samp!");
    }
    else
    {
        // thong bao trang thai
        if (isCaptureSample == true)
        {
            //------------  Dialog Thong Bao
            // khoi tao dialog
            dialogSampConf = new DialogSampConf(this);
            dialogSampConf->SetText("Capture Sample Mode!");
            dialogSampConf->showFullScreen();
            //---------------

            ui->lbisSample->setText("Cap Samp!");
        }
        else
        {
            //------------  Dialog Thong Bao
            // khoi tao dialog
            dialogSampConf = new DialogSampConf(this);
            dialogSampConf->SetText("Detect Mode!");
            dialogSampConf->showFullScreen();
            //---------------

            ui->lbisSample->setText("Detect Mode!");
        }
        tmrTimer->start(config.timeStart);
    }
}
