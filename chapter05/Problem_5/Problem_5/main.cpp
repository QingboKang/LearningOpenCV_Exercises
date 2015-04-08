#include <opencv\highgui.h>
#include <opencv_libs.h>
#include <opencv\cv.h>

int main()
{
    IplImage * src1 = cvLoadImage("frame_87.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    IplImage * src2 = cvLoadImage("frame_65.bmp", CV_LOAD_IMAGE_GRAYSCALE);

    if( !src1 || !src2 )
    {
        printf("Load image error.\n");
        return -1;
    }

    IplImage * diff12 = cvCreateImage(cvGetSize(src1), src1->depth, src1->nChannels);

    cvAbsDiff(src1, src2, diff12);

    cvThreshold(diff12, diff12, 134, 255, CV_THRESH_BINARY);
    cvMorphologyEx(diff12, diff12, NULL, NULL, CV_MOP_OPEN, 1 );
    cvShowImage("abs diff", diff12);

    cvWaitKey(0);
    cvMorphologyEx(diff12, diff12, NULL, NULL, CV_MOP_OPEN, 4 );
    cvShowImage("abs diff", diff12);

    cvWaitKey(0);
    
    cvReleaseImage(&src1);
    cvReleaseImage(&src2);
    cvReleaseImage(&diff12);
    cvDestroyAllWindows();

    return 0;
}