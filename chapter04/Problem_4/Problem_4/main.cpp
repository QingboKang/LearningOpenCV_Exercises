#include <highgui.h>
#include <cv.h>
#include "opencv_libs.h"

/*
 *《学习OpenCV》第四章第四题
 * 完成时间：1:36 3/31 星期日 2013
 */

int g_slider_position = 0;
CvCapture* g_capture = NULL;

int i = 0;
int g_slider_position1 = 0;
bool t = true;


void onTrackbarSlide(int pos)
{
	cvSetCaptureProperty ( g_capture, CV_CAP_PROP_POS_FRAMES, pos );
	i = pos;
}

void switch_callback(int position)
{
	if(position == 0)
	{
		t = true;
	}
	else
	{
		t = false;
	}
}

int main()
{
	cvNamedWindow("Video", CV_WINDOW_AUTOSIZE);
	
	g_capture = cvCaptureFromFile("1111.avi");
	int frames = (int)cvGetCaptureProperty( g_capture, CV_CAP_PROP_FRAME_COUNT );

	if(frames != 0)
	{
		cvCreateTrackbar( "Position", "Video", &g_slider_position, frames, onTrackbarSlide);
		cvCreateTrackbar( "Position1", "Video", &g_slider_position1, 1, switch_callback);
	}
	cvSetTrackbarPos("Position", "Video", 1 );
	cvSetTrackbarPos("Position1", "Video", 0 );

	IplImage * frame;
	while( (frame= cvQueryFrame(g_capture)) != NULL)
	{
		char c = cvWaitKey(50);
		if(c == 27)
			break;
		cvShowImage("Video", frame);
		cvSetTrackbarPos("Position", "Video", i);

		if(t)
		{
			i = i+ 10;  // 10为步长
			cvSetCaptureProperty( g_capture, CV_CAP_PROP_POS_FRAMES, i );
		}
		else
		{
			cvSetCaptureProperty( g_capture, CV_CAP_PROP_POS_FRAMES, i );
		}
	}

	cvReleaseCapture(&g_capture);
	cvReleaseImage(&frame);
	cvDestroyAllWindows();

	return 0;
}