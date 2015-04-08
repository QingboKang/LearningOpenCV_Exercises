#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv_libs.h>
#include <math.h>

/*
 *《学习OpenCV》第五章第一题  
 * 完成时间：18:37 10/13 星期日 2013  
 * 作者：qdsclove@163.com
 */

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
    const char * FILE_PATH = "Fig0333(a)(test_pattern_blurring_orig).tif";

    IplImage* src = cvLoadImage(FILE_PATH, CV_LOAD_IMAGE_UNCHANGED);

    if(!src)
    {
        printf("Load image error.\n");
        return -1;
    }

    // Get the source image's size
    CvSize srcSize = cvGetSize(src);

    // 3 * 3
    IplImage* dst_three_gaussian = cvCreateImage(srcSize, src->depth, src->nChannels);
    // 5 * 5
    IplImage* dst_five_gaussian = cvCreateImage(srcSize, src->depth, src->nChannels);
    // 9 * 9
    IplImage* dst_nine_gaussian = cvCreateImage(srcSize, src->depth, src->nChannels);
    // 11 * 11
    IplImage* dst_eleven_gaussian = cvCreateImage(srcSize, src->depth, src->nChannels);
    // twice 5 * 5
    IplImage* dst_twice_five_gaussian = cvCreateImage( srcSize, src->depth, src->nChannels );


    if( !dst_three_gaussian || !dst_five_gaussian ||
        !dst_nine_gaussian || !dst_eleven_gaussian ||
        !dst_twice_five_gaussian )
    {
        printf("Create image error.\n");
        return -1;
    }

    cvSmooth(src, dst_three_gaussian, CV_GAUSSIAN, 3, 3);
    cvSmooth(src, dst_five_gaussian, CV_GAUSSIAN, 5, 5);
    cvSmooth(src, dst_nine_gaussian, CV_GAUSSIAN, 9, 9);
    cvSmooth(src, dst_eleven_gaussian, CV_GAUSSIAN, 11, 11);
    cvSmooth( dst_five_gaussian, dst_twice_five_gaussian, CV_GAUSSIAN, 5, 5 );

    cvShowImage("src", src);
    cvShowImage("src - GAUSSIAN 3*3", dst_three_gaussian);
    cvShowImage("src - GAUSSIAN 5*5", dst_five_gaussian);
    cvShowImage("src - GAUSSIAN 9*9", dst_nine_gaussian);
    cvShowImage("src - GAUSSIAN 11*11", dst_eleven_gaussian);
    cvShowImage("src - GAUSSIAN 5*5 Twice", dst_twice_five_gaussian );

    // calculate the MSE and PSNR of the two images.
    double dMSE, dPSNR;
    // part a:
    calculateGrayImgsPSNR(src, dst_three_gaussian, dMSE, dPSNR);
    printf("source image & 3*3 GAUSSIAN: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);
    calculateGrayImgsPSNR(src, dst_five_gaussian, dMSE, dPSNR);
    printf("source image & 5*5 GAUSSIAN: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);
    calculateGrayImgsPSNR(src, dst_nine_gaussian, dMSE, dPSNR);
    printf("source image & 9*9 GAUSSIAN: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);
    calculateGrayImgsPSNR(src, dst_eleven_gaussian, dMSE, dPSNR);
    printf("source image & 11*11 GAUSSIAN: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);

    // part b
    puts("---------------------------\n");
    calculateGrayImgsPSNR(src, dst_eleven_gaussian, dMSE, dPSNR);
    printf("source image & eleven: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);
    calculateGrayImgsPSNR(src, dst_twice_five_gaussian, dMSE, dPSNR);
    printf("source image & twice five: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);
    calculateGrayImgsPSNR(dst_eleven_gaussian, dst_twice_five_gaussian, dMSE, dPSNR);
    printf("eleven & twice five: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);

    cvWaitKey(0);
    cvReleaseImage(&src);
    cvReleaseImage(&dst_three_gaussian);
    cvReleaseImage(&dst_five_gaussian);
    cvReleaseImage(&dst_nine_gaussian);
    cvReleaseImage(&dst_eleven_gaussian);
    cvReleaseImage(&dst_twice_five_gaussian);
    cvDestroyAllWindows();

    return 0;
}