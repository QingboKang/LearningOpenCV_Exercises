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
    IplImage* src = cvLoadImage("src.png", CV_LOAD_IMAGE_UNCHANGED);

    // c:0 0 1 9
    IplImage* gaussian_one_nine = cvCreateImage(        
        cvGetSize(src), src->depth, src->nChannels );
    // d:0 0 9 1
    IplImage* gaussian_nine_one = cvCreateImage( 
        cvGetSize(src), src->depth, src->nChannels );
    // e:0 0 1 9 --> 0 0 9 1
    IplImage* gaussian_twice = cvCreateImage( 
        cvGetSize(src), src->depth, src->nChannels );
    // f:
    IplImage* gaussian_fpart = cvCreateImage( 
        cvGetSize(src), src->depth, src->nChannels );


    printf("0 0 1 9\n");
    cvSmooth(src, gaussian_one_nine, CV_GAUSSIAN, 0, 0, 1, 9);
    printf("\n0 0 9 1\n");
    cvSmooth(src, gaussian_nine_one, CV_GAUSSIAN, 0, 0, 9, 1);
    printf("\n0 0 1 9 --> 0 0 9 1\n");
    // 7 55 1 9
    cvSmooth(src, gaussian_twice, CV_GAUSSIAN, 0, 0, 1, 9);
    // 55 7 9 1
    cvSmooth(gaussian_twice, gaussian_twice, CV_GAUSSIAN, 0, 0, 9, 1);

    // 55 55 9 9
    cvSmooth(src, gaussian_fpart, CV_GAUSSIAN, 0, 0, 9, 9);
    
    //cvSmooth(gaussian_fpart, gaussian_fpart, CV_GAUSSIAN, 9, 9, 0, 0);
    
    double dMSE = 0, dPSNR = 0;
    calculateGrayImgsPSNR(gaussian_twice, gaussian_fpart, dMSE, dPSNR);
    printf("MSE: %f\tPSNR: %f\n", dMSE, dPSNR);

    cvShowImage("src", src);
    cvShowImage("one_nine", gaussian_one_nine);
    cvSaveImage( "0019.png", gaussian_one_nine );
    cvShowImage("nine_one", gaussian_nine_one);
    cvSaveImage("0091.png", gaussian_nine_one);
    cvShowImage("twice", gaussian_twice);
    cvSaveImage("twice.png", gaussian_twice);
    cvShowImage("fpart", gaussian_fpart);
    cvSaveImage( "fpart.png", gaussian_fpart );
    cvWaitKey(0);

    cvDestroyAllWindows();
    cvReleaseImage(&src);
    return 0;
}