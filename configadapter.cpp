#include "configadapter.h"

configAdapter::configAdapter()
{
    QFile* file = new QFile("/home/root/CODE/config.xml");

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)){
        printf("Can not open this file!\n");
        return;
    }

    //cout << "doc dc config ne" << endl;
    QXmlStreamReader xml(file);

    while(!xml.atEnd() && !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartDocument){
            continue;
        }

        if (token == QXmlStreamReader::StartElement){
            if (xml.name() == "imageconfig"){ //get image config value
                QXmlStreamAttributes atr = xml.attributes();

                cameraid    = atr.value("cameraid").toString().toUInt();
                timeStart   = atr.value("timeStart").toString().toUInt();

                imgHeight   = atr.value("height").toString().toInt();
                imgWidth    = atr.value("width").toString().toInt();
            }

            if (xml.name() == "imageshow"){ //get image show config info
                QXmlStreamAttributes atr = xml.attributes();

                heightScale = atr.value("heightscale").toString().toUInt();
                widthScale  = atr.value("widthscale").toString().toUInt();
            }

            if (xml.name() == "delaybutton"){
                QXmlStreamAttributes atr = xml.attributes();

                delayTime = atr.value("time").toString().toFloat();
            }

            if (xml.name() == "backgroundimage"){
                QXmlStreamAttributes atr = xml.attributes();

                backGroundURL = atr.value("url").toString().toStdString().c_str();
            }

            if (xml.name() == "property"){
                QXmlStreamAttributes atr = xml.attributes();

                if (atr.value("id").toString() == "h_bin"){
                    //doc histogram Bin of channel H
                    histogram_bin_h   = atr.value("value").toString().toUInt();
                }

                if (atr.value("id").toString() == "s_bin"){
                    //doc histogram Bin of channel S
                    histogram_bin_s = atr.value("value").toString().toUInt();
                }

                if (atr.value("id").toString() == "h_range"){
                    //doc histogram range of channel h
                    histogram_range_h   = atr.value("value").toString().toUInt();
                }

                if (atr.value("id").toString() == "s_range"){
                    //doc nguong
                    histogram_range_s = atr.value("value").toString().toUInt();
                }
            }

            if (xml.name() == "Algorithms"){
                QXmlStreamAttributes atr = xml.attributes();

                algselect = atr.value("select").toString().toUInt();
            }


            if (xml.name() == "alg"){
                QXmlStreamAttributes atr = xml.attributes();

                if (atr.value("value").toString().toUInt() == algselect){
                    histogram_nguong = atr.value("thr").toString().toDouble();
                    compare          = atr.value("compare").toString().toUInt();
                }
            }

            if (xml.name() == "camerafocus"){
                QXmlStreamAttributes atr = xml.attributes();

                autofocus  = atr.value("autofocus").toString()      == "on"? true: false;
                lightstate = atr.value("value").toString().toUInt() == 1   ? true: false;
                focusabs   = atr.value("value").toString().toUInt();
            }
        }
    }

    file->close();
}
