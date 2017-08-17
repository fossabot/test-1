#ifndef DETECT_FUSE_H
#define DETECT_FUSE_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "iostream"
#include "fstream"

using namespace cv;
using namespace std;

class detect_fuse
{
public:
    detect_fuse();
    Mat detect(vector<Rect> vecMask, Mat matRef, Mat matSample, int &numError);
    void init_detect(int h_bin, int s_bin, int h_range, int s_range, int alg, double nguongXet, bool stateAlg);
};

#endif // DETECT_FUSE_H
