#include <stdio.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv_libs247d.h>


int main()
{
    IplImage * src1 = cvLoadImage("frame_28.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    IplImage * src2 = cvLoadImage("frame_52.bmp", CV_LOAD_IMAGE_GRAYSCALE);

    IplImage * diff12 = cvCreateImage(cvGetSize(src1), src1->depth, src1->nChannels);
    IplImage * cleandiff = cvCreateImage(cvGetSize(src1), src1->depth, src1->nChannels);
    IplImage * dirtydiff = cvCreateImage(cvGetSize(src1), src1->depth, src1->nChannels);

    cvAbsDiff(src1, src2, diff12);
    
    // Opening
    cvErode(diff12, cleandiff, NULL, 1);
    cvDilate(cleandiff, cleandiff, NULL, 1);

    // Closing
    cvDilate(diff12, dirtydiff);
    cvErode(dirtydiff, dirtydiff);

    cvShowImage("abs diff", diff12);
    cvShowImage("clean diff", cleandiff);
    cvShowImage("dirty diff", dirtydiff);

    cvSaveImage("absdiff.png", diff12);
    cvSaveImage("cleandiff.png", cleandiff);
    cvSaveImage("dirtydiff.png", dirtydiff);

    cvSaveImage("src1.png", src1);
    cvSaveImage("src2.png", src2);

    cvWaitKey(0);

    cvReleaseImage(&src1);
    cvReleaseImage(&src2);
    cvReleaseImage(&diff12);
    cvReleaseImage(&cleandiff);
    cvReleaseImage(&dirtydiff);
    cvDestroyAllWindows();

    return 0;
}