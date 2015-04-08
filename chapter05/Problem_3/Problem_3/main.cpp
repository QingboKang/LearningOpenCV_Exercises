#include <stdio.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv_libs247d.h>

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
    char * file_path = "Fig0333(a)(test_pattern_blurring_orig).tif";

    IplImage * src = cvLoadImage( file_path, CV_LOAD_IMAGE_UNCHANGED );

    // a
    IplImage * img_gaussian_nine_one = cvCreateImage( 
        cvGetSize(src), src->depth, src->nChannels );
    IplImage * img_gaussian_nine_four = cvCreateImage( 
        cvGetSize(src), src->depth, src->nChannels );
    IplImage * img_gaussian_nine_six = cvCreateImage( 
        cvGetSize(src), src->depth, src->nChannels );

   // printf("ksize: 3 3, sigma1: 0.\n");
    cvSmooth(src, img_gaussian_nine_one, CV_GAUSSIAN, 9, 9, 1);
    //printf("\n--------------------------------------------------\n");
 //   printf("ksize: 9 9, sigma1: 4.\n");
    cvSmooth(src, img_gaussian_nine_four, CV_GAUSSIAN, 9, 9, 4);
   // printf("\n--------------------------------------------------\n");
    cvSmooth(src, img_gaussian_nine_six, CV_GAUSSIAN, 9, 9, 6);

    // b
    IplImage * img_gaussian_zero_one = cvCreateImage( 
        cvGetSize(src), src->depth, src->nChannels );
    IplImage * img_gaussian_zero_four = cvCreateImage( 
        cvGetSize(src), src->depth, src->nChannels );
    IplImage * img_gaussian_zero_six = cvCreateImage( 
        cvGetSize(src), src->depth, src->nChannels );

    cvSmooth( src, img_gaussian_zero_one, CV_GAUSSIAN, 0, 0, 1);
    cvSmooth( src, img_gaussian_zero_four, CV_GAUSSIAN, 0, 0, 4 );
    cvSmooth( src, img_gaussian_zero_six, CV_GAUSSIAN, 0, 0, 6 );

    double dMSE, dPSNR;
    calculateGrayImgsPSNR(img_gaussian_nine_one, img_gaussian_zero_one, dMSE, dPSNR);
    printf("param3 = 1: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);
    calculateGrayImgsPSNR(img_gaussian_nine_four, img_gaussian_zero_four, dMSE, dPSNR);
    printf("param3 = 4: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);
    calculateGrayImgsPSNR(img_gaussian_nine_six, img_gaussian_zero_six, dMSE, dPSNR);
    printf("param3 = 6: MSE: %f\tPSNR: %f\n", dMSE, dPSNR);

    cvShowImage("src", src);
    cvShowImage("gaussian_nine_one", img_gaussian_nine_one);
    cvSaveImage("gaussian_nine_one.png", img_gaussian_nine_one);
    cvShowImage("gaussian_nine_four", img_gaussian_nine_four);
    cvSaveImage("gaussian_nine_four.png", img_gaussian_nine_four);
    cvShowImage("gaussian_nine_six", img_gaussian_nine_six);
    cvSaveImage("gaussian_nine_six.png", img_gaussian_nine_six);
    cvShowImage("gaussian_zero_one", img_gaussian_zero_one);
    cvSaveImage("gaussian_zero_one.png", img_gaussian_zero_one);

    cvShowImage("gaussian_zero_four", img_gaussian_zero_four);
    cvSaveImage("gaussian_zero_four.png", img_gaussian_zero_four);

    cvShowImage("gaussian_zero_six", img_gaussian_zero_six);
    cvSaveImage("gaussian_zero_six.png", img_gaussian_zero_six);

    cvSaveImage("src.png", src);

    
    cvWaitKey(0);

    cvReleaseImage(&src);
    cvDestroyAllWindows(); 

    return 0;
}