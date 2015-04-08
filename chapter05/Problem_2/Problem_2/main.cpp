#include <stdio.h>
#include <opencv/highgui.h>
#include <opencv/cv.h> 
#include <opencv_libs.h>


/*
 *《学习OpenCV》第五章第二题  
 * 完成时间：21:43 10/13 星期日 2013  
 * 作者：qdsclove@163.com
 */

/* Image Size */
#define   IMG_SIZE   100

/*
 * Window Title
 */
#define   WNDTITLE_IMAGE   "source image"
#define   WNDTITLE_FIVE    "5*5 Gaussian"
#define   WNDTITLE_NINE    "9*9 Gaussian"
#define   WNDTITLE_FIVE_TEICE   "5*5 Gaussian Twice"

/*
 * function: calculate MSE & PSNR of two GrayScale(8-bit depth & one channel) images.
 * param: img1 -- the first image.
 * param: img2 -- the second image.
 * param: dMSE -- the MSE of two images(output)
 * param: dPSNR -- the PSNR of two images(output)
 * return: 0 -- success;  others -- failed.
 */
int calculateGrayImgsPSNR(IplImage* img1, IplImage* img2, double& dMSE, double& dPSNR)
{
    if( !img1 || !img2 || 
        img1->nChannels != 1 ||
        img2->nChannels != 1 || 
        img1->depth != img2->depth ||
        img1->width != img2->width || 
        img1->height != img2->height )
    {
        return -1;
    }
    int width = img1->width;
    int height = img1->height;

    // calculate MSE of the two images
    double dSumOfSquares = 0;
    for(int i = 0; i < height; i++)
    {
        char* pdata1 = img1->imageData + i * img1->widthStep;
        char* pdata2 = img2->imageData + i *img2->widthStep;
        for(int j = 0; j < width; j++ )
        {
            uchar value1 = *(pdata1 + j);
            uchar value2 = *(pdata2 + j);

            double square = pow( (double)(value1 - value2), 2 );
            dSumOfSquares += square;
        }
    }

    dMSE = dSumOfSquares / (width * height);

    // this is means the two images are strictly same. 
    if(dMSE == 0)
    {
        dPSNR = -1;
        return 0;
    }
    int iDepth = img1->depth;
    int iMAX = pow( 2., iDepth) - 1;

    dPSNR = 20 * log10(iMAX / (sqrt(dMSE)));

    return 0;
}


int main()
{
    IplImage* image = cvCreateImage( cvSize(IMG_SIZE, IMG_SIZE), IPL_DEPTH_8U, 1 );
    IplImage* dst_five_gaussian = cvCreateImage( cvGetSize(image), image->depth, image->nChannels );
    IplImage* dst_nine_gaussian = cvCreateImage( cvGetSize(image), image->depth, image->nChannels );
    IplImage* dst_twice_five_gaussian = cvCreateImage(  cvGetSize(image), image->depth, image->nChannels );
    
    // 全部像素置零
    cvZero(image);
    // 设置中心像素为255
    cvSet2D(image, IMG_SIZE/2, IMG_SIZE/2, cvScalarAll(255));

    // 5*5 高斯滤波
    cvSmooth(image, dst_five_gaussian, CV_GAUSSIAN, 5, 5);
    // 9*9 高斯滤波
    cvSmooth(image, dst_nine_gaussian, CV_GAUSSIAN, 9, 9);
    // 5*5高斯滤波 第二次
    cvSmooth(dst_five_gaussian, dst_twice_five_gaussian, CV_GAUSSIAN, 5, 5);

    cvNamedWindow(WNDTITLE_IMAGE, CV_WINDOW_NORMAL);
    cvNamedWindow(WNDTITLE_FIVE, CV_WINDOW_NORMAL);
    cvNamedWindow(WNDTITLE_NINE, CV_WINDOW_NORMAL);
    cvNamedWindow(WNDTITLE_FIVE_TEICE, CV_WINDOW_NORMAL);

    cvShowImage(WNDTITLE_IMAGE, image);
    cvShowImage(WNDTITLE_FIVE, dst_five_gaussian);
    cvShowImage(WNDTITLE_NINE, dst_nine_gaussian);
    cvShowImage(WNDTITLE_FIVE_TEICE, dst_twice_five_gaussian);

    cvSaveImage("source.bmp", image);
    cvSaveImage("5_5_gaussian.bmp", dst_five_gaussian);
    cvSaveImage("9_9_gaussian.bmp", dst_nine_gaussian);
    cvSaveImage("5_5_gaussian_twice.bmp", dst_twice_five_gaussian);

    // c part
    double dMSE = 0, dPSNR = 0;
    calculateGrayImgsPSNR(dst_nine_gaussian, dst_twice_five_gaussian, dMSE, dPSNR);
    printf("9*9 GAUSSIAN & 5*5 GAUSSIAN Twice: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);

    cvWaitKey(0);

    cvReleaseImage(&image);
    cvReleaseImage(&dst_five_gaussian);
    cvReleaseImage(&dst_nine_gaussian);
    cvDestroyAllWindows();

    return 0;
}