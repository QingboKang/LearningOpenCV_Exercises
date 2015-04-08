#include <opencv/cv.h> 
#include <opencv/highgui.h>
#include <opencv_libs.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <assert.h> 
#include <math.h> 
#include <float.h> 
#include <limits.h> 
#include <time.h> 
#include <ctype.h>

/*
 *《学习OpenCV》第四章第八题  
 * 完成时间：3:43 10/5 星期六 2013  
 * 作者：qdsclove@163.com
 */    


/* Result window title */
#define  WND_RESULT   "result"

static CvMemStorage* storage = 0; 
static CvHaarClassifierCascade* cascade = 0;

void detect_and_draw( IplImage* image );

const char* cascade_name = 
    "haarcascade_frontalface_alt.xml"; 

/* Skull image */
IplImage* g_skullImage;

/* Trackbar initial value */
int g_trackbar_value = 5;
/* Tackbar total values */
int g_trackbar_total = 10;

/* Alpha blend's alpha */
double g_alpha = 0;

/* Trackbar callback function */
void switch_callback(int pos)
{
    printf("Trackbar event.  pos: %d\n", pos);
    g_alpha = (double)pos / g_trackbar_total;
}

int main( ) 
{ 
    CvCapture* capture = cvCreateCameraCapture(0);
    if(!capture)
    {
        printf("Camera error.\n");
        return -1;
    }
    cascade_name = "haarcascade_frontalface_alt2.xml"; 
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 ); 

    if( !cascade ) 
    { 
        printf( "ERROR: Could not load classifier cascade\n" ); 
        return -1; 
    } 

    storage = cvCreateMemStorage(0); 
    cvNamedWindow( WND_RESULT, 1 ); 

    IplImage* frame = cvQueryFrame(capture);
    if(!frame)
    {
        return -1;
    }

    // Load skull image
    IplImage* skullImage = cvLoadImage("skull.jpg", CV_LOAD_IMAGE_UNCHANGED);
    if(!skullImage)
    {
        printf("ERROR: Can't load skull image.\n");
        return -1;
    }
    g_skullImage = cvCreateImage(cvGetSize(skullImage), skullImage->depth, 3);
    if(skullImage->nChannels != 3)
    {
        cvCvtColor(skullImage, g_skullImage, CV_GRAY2BGR);
    }
    else
    {
        cvCopy(skullImage, g_skullImage);
    }

    // Create trackbar
    cvCreateTrackbar("Switch", WND_RESULT, &g_trackbar_value, 
        g_trackbar_total, switch_callback );
    // Initialize Alpha-Blend's alpha
    g_alpha = (double)g_trackbar_value / g_trackbar_total;

    char c;
    while( 1 ) 
    { 
        frame = cvQueryFrame(capture);
        if(!frame)
        {
            return -1;
        }

        detect_and_draw( frame ); 

        c = cvWaitKey(50);
        if(c == 27)
        {
            break;
        }
    }

    cvDestroyWindow(WND_RESULT); 
    cvReleaseImage(&frame);
    cvReleaseCapture(&capture);
    cvReleaseImage(&skullImage);
    cvReleaseImage(&g_skullImage);

    return 0; 
}


void detect_and_draw(IplImage* img ) 
{ 
    double scale=1.2; 
    static CvScalar colors[] = { 
        {{0,0,255}},{{0,128,255}},{{0,255,255}},{{0,255,0}}, 
        {{255,128,0}},{{255,255,0}},{{255,0,0}},{{255,0,255}} 
    };//Just some pretty colors to draw with

    //Image Preparation 
    // 
    IplImage* gray = cvCreateImage(cvSize(img->width,img->height),8,1); 
    IplImage* small_img=cvCreateImage(cvSize(cvRound(img->width/scale),cvRound(img->height/scale)),8,1); 
    cvCvtColor(img,gray, CV_BGR2GRAY); 
    cvResize(gray, small_img, CV_INTER_LINEAR);

    cvEqualizeHist(small_img,small_img); //直方图均衡

    //Detect objects if any 
    // 
    cvClearMemStorage(storage); 
    double t = (double)cvGetTickCount(); 
    CvSeq* objects = cvHaarDetectObjects(small_img, 
        cascade, 
        storage, 
        1.1, 
        2, 
        0/*CV_HAAR_DO_CANNY_PRUNING*/, 
        cvSize(30,30));

    t = (double)cvGetTickCount() - t; 
    printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

    //Loop through found objects and draw boxes around them 
    for(int i=0;i<(objects? objects->total:0);++i) 
    { 
        CvRect* r=(CvRect*)cvGetSeqElem(objects,i); 

        CvRect r_scale = cvRect( r->x * scale, r->y * scale, r->width * scale, r->height * scale );
        // 改变骷髅头图像大小
        IplImage * skullResize = cvCreateImage( cvSize(r_scale.width, r_scale.height),
            g_skullImage->depth, g_skullImage->nChannels );
        cvResize(g_skullImage, skullResize, CV_INTER_LINEAR);

        // 将人脸检测区域用骷髅头图像代替
        cvSetImageROI(img, r_scale);
        // b -- Alpha blend 
        printf("Alpha: %f\t", g_alpha);
        cvAddWeighted( img, g_alpha, skullResize, 1.0 - g_alpha, 0.0, img);
       /* This is a part
        cvCopy(skullResize, img); */
        cvResetImageROI(img);
        cvReleaseImage(&skullResize);

     //   cvRectangle(img, cvPoint(r->x*scale,r->y*scale), cvPoint((r->x+r->width)*scale,(r->y+r->height)*scale), colors[i%8]); 
    } 
 /*   for( int i = 0; i < (objects? objects->total : 0); i++ ) 
    { 
        CvRect* r = (CvRect*)cvGetSeqElem( objects, i ); 
        CvPoint center; 
        int radius; 
        center.x = cvRound((r->x + r->width*0.5)*scale); 
        center.y = cvRound((r->y + r->height*0.5)*scale); 
        radius = cvRound((r->width + r->height)*0.25*scale); 
        cvCircle( img, center, radius, colors[i%8], 3, 8, 0 ); 
    } */

    cvShowImage( WND_RESULT, img ); 
    cvReleaseImage(&gray); 
    cvReleaseImage(&small_img); 
}