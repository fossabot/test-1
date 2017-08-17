/**
  ******************************************************************************
  * @file    fuse.cpp
  * @author  Nguyen Trung Tin - Thai Tuan Phuc
  * @version V1.0.0
  * @date    10/03/2014
  * @brief   file nay chua ham main va dinh nghia cac ham phuc vu cho viec detect cau chi.
  *
  ******************************************************************************
*/
#include "detect_fuse.h"

/* Private valiable -----------------------------------------------------------*/
int     h_b;     // bien nay quy dinh so Bins cua histogram.
int     s_b;
int     h_r;
int     s_r;

double  nguong; // nguon de quyet dinh do khop cua 2 histogram

bool state;
int  algSelect;
/* Private define -------------------------------------------------------------*/

/* Private funtion prototyle --------------------------------------------------*/

//void
//them_thongsovaoanh(Mat &img, double threshold, int h_Bins, int nguongduyet);
//void
//them_kytuvaoanh(Mat &img, String text, Point toado);

/**
 *  @brief: ham nay giup kiem tra su khac biet giua hai anh dua vao histogram.
 *  @param:
 *          + Mat matAnh1: anh muon kiem tra thu 1.
 *          + Mat matAnh2: anh muon kiem tra thu 2.
 *  @retval: double: do khac biet giua hai anh.
*/
double compareImageUsingHistogram(Mat matAnh1,Mat matAnh2);

/**
 *  @brief: ham ve dau x.
 *  @param:
 *          + matResult: anh muon ve dau x.
 *          + toado: toa do dinh tren ben trai dau x.
 *  @retval:
*/
void draw_x(Mat &matResult,Rect toado);

detect_fuse::detect_fuse()
{


}

/**
 *  @brief: Ham nay set cac thong so cau hinh cho thuan toan so sanh.
 *  @param:
 *          + int h_bin       : so bin cua histogram cho kenh h
 *          + int s_bin       : so bin cua histogram cho kenh s
 *          + int h_range     : so range cua histogram cho kenh h
 *          + int s_range     : so range cua histogram cho kenh s
 *          + double nguongXet: nguong xet cua thuat toan.
 *  @retval: void
*/
void detect_fuse::init_detect(int h_bin, int s_bin, int h_range, int s_range, int alg, double nguongXet, bool stateAlg)
{
    h_b       = h_bin;
    s_b       = s_bin;

    h_r       = h_range;
    s_r       = s_range;

    nguong    = nguongXet;

    algSelect = alg;
    state     = stateAlg;
}


/**
 *  @brief: ham nay giup doc anh cau chi, xu ly va dua ra ket luan ve anh.
 *  @param:
 *          + vector<Rect> vecMask;  - chua toa do cac chi tiet cua mau.
 *          + Mat matRef             - chua anh muon kiem tra.
 *          + Mat matSample;         - chua anh mau dung de kiem tra (anh dung).
 *  @retval: (Mat) anh sau khi xac dinh cac vi tri cau chi co lap dung hay khong.
*/
Mat detect_fuse::detect(vector<Rect> vecMask, Mat matRef, Mat matSample, int &numError)
{

    numError = 0;
    // Khai bao bien
    Mat matResult = matRef;             //  chua anh ket qua sau khi kiem tra. kich thuoc bang anh vao
    Mat matSrcTest;                     // anh cau chi muon kiem tra
    Mat matDesTest;                     // anh cau chi dung.

    double compare_cost;
    // duyet qua cac phan tu, lay ra va so sanh su khac biet.

    for(unsigned int i = 1; i < vecMask.size() ; i++)
    {

        usleep(5);
        // thu nho vung chon
        Rect toado      = vecMask[i];

        // lay ra cac vung chua cau chi ung voi cau chi thu i.
        matSrcTest      = matRef(toado);
        matDesTest      = matSample(toado);


        // so sanh histogram hai chi tiet cau chi.
        compare_cost    = compareImageUsingHistogram(matSrcTest,matDesTest);

        // dua vao nguong khac biet do nguoi dung cau hinh de nhan xet su khac biet

        bool result = true;


        if (state == 0){
            if (compare_cost > nguong){
                result = false;
            }
        }
        else {
            if (compare_cost < nguong){
                result = false;
            }
        }

        if (!result)
        {
            draw_x(matResult,toado); //danh dau cau chi sai
            cout << i << " sai: " << compare_cost  << " -- " << nguong << endl;
            numError++;
        }
        else
        {
            cout << i << ": " << compare_cost << endl;
        }
    }
    // Tra ve anh ket qua.
    return matResult;
}

/**
 *  @brief: ham nay giup kiem tra su khac biet giua hai anh dua vao histogram.
 *  @param:
 *          + Mat matAnh1: anh muon kiem tra thu 1.
 *          + Mat matAnh2: anh muon kiem tra thu 2.
 *  @retval: double: do khac biet giua hai anh.
*/
double compareImageUsingHistogram(Mat matAnh1,Mat matAnh2)
{
    // khai bao bien
    Mat hsv_base;
    Mat hsv_test1;

    // Histograms
    MatND hist_anh1;
    MatND hist_test1;

    // Chuyen sang he mau HSV
    //-- anh 1
    cvtColor( matAnh1, hsv_base, CV_BGR2HSV );
    //-- anh 2
    cvtColor( matAnh2, hsv_test1, CV_BGR2HSV );

    //int histSize[] = { h_b, s_b};
    int histSize[] = { h_b, 10};

    // hue varies from 0 to 256,
    float h_ranges[] = { 0, h_r + 1 };
    float s_ranges[] = { 0, s_r + 1 };

    const float* ranges[] = { h_ranges, s_ranges};

    // Use the o-th and 1-st channels
    int channels[] = {0,1};

    // Calculate the histograms for the HSV images
    calcHist( &hsv_base, 1, channels, Mat(), hist_anh1, 2, histSize, ranges, true, false );
    normalize( hist_anh1, hist_anh1, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false );
    normalize( hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat() );

    double KQSoSanhHist = compareHist( hist_anh1, hist_test1, algSelect);



//        cout << "CORREL:        " << compareHist( hist_anh1, hist_test1, CV_COMP_CORREL) << endl;
//        cout << "CHIQR:         " << compareHist( hist_anh1, hist_test1, CV_COMP_CHISQR) << endl;
//        cout << "INTERSECT:     " << compareHist( hist_anh1, hist_test1, CV_COMP_INTERSECT) << endl;
//        cout << "BHATTACHARYYA  " << compareHist( hist_anh1, hist_test1, CV_COMP_BHATTACHARYYA) << endl;


    return KQSoSanhHist;
}


/**
 *  @brief: ham ve dau x.
 *  @param:
 *          + matResult: anh muon ve dau x.
 *          + toado: toa do dinh tren ben trai dau x.
 *  @retval:
*/
void draw_x(Mat &matResult,Rect toado)
{
    Point point1, point2;

    //thiet lap line 1
    point1.x = toado.x;
    point1.y = toado.y;

    point2.x = toado.x + toado.width;
    point2.y = toado.y + toado.height;

    line(matResult, point1, point2, Scalar(0, 0, 255), 1, 8, 0);

    //thiet lap line 2
    point1.x += toado.width;
    point2.x -= toado.width;

    line(matResult, point1, point2, Scalar(0, 0, 255), 1, 8, 0);
}


/*-----------------------------  END FILE  -----------------------------*/
