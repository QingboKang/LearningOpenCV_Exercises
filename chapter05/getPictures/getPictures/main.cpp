#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv_libs.h>


#define   SAVEIMGDIR      "frame/"
/*
 * Frame Size (Width & Height)
 */
#define   FRAMEWIDTH       800 
#define   FRAMEHEIGHT      600

int main()
{
    CvCapture* capture = cvCreateCameraCapture(1);
    if(!capture)
    {
        printf("Camera error.\n");
        return -1;
    }

    // set camera property
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, FRAMEWIDTH);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, FRAMEHEIGHT);

    IplImage* frame = cvQueryFrame(capture);
    IplImage* frame_prior = cvCreateImage(
        cvGetSize(frame), frame->depth, frame->nChannels);
    IplImage * diff12 = cvCreateImage(
        cvGetSize(frame), frame->depth, frame->nChannels);

    if(!frame)
    {
        return -1;
    }

    cvCopy(frame, frame_prior);
    char c;
    char file_name[128];
    int count_frame = 0;
    while( 1 ) 
    { 
        frame = cvQueryFrame(capture);
        if(!frame)
        {
            return -1;
        }

        cvAbsDiff(frame, frame_prior, diff12);
        cvCopy(frame, frame_prior);
        sprintf(file_name, "%sframe_%d.bmp", SAVEIMGDIR, count_frame++ );
       cvSaveImage(file_name, frame);
        printf("%d: %s\n", count_frame, file_name);

        cvShowImage("diff", diff12);
        c = cvWaitKey(50);
        if(c == 27)
        {
            break;
        }
    }

    cvDestroyAllWindows();
    cvReleaseImage(&frame);
}