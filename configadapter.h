#ifndef CONFIGADAPTER_H
#define CONFIGADAPTER_H

#include <QFile>
#include <qxmlstream.h>
#include "iostream"

using namespace std;

class configAdapter
{
public:
    configAdapter();
    configAdapter(char* urlFile);

    // image configure
    int imgHeight;
    int imgWidth;
    int heightScale;
    int widthScale;

    // App config
    unsigned char cameraid;
    unsigned char timeStart;

    //Alg
    unsigned char histogram_bin_s;
    unsigned char histogram_bin_h;
    unsigned char histogram_range_s;
    unsigned char histogram_range_h;

    unsigned char algselect;
    float         histogram_nguong;
    bool          compare;
    //camera config
    bool          autofocus;
    unsigned char focusabs;
    bool          lightstate;

    //customs
    float         delayTime;
    const char*   backGroundURL;
};

#endif // CONFIGADAPTER_H
