#include <highgui.h>
#include <cv.h>
#include "opencv_libs.h"

#pragma comment (lib,"opencv_calib3d231d.lib")
#pragma comment (lib,"opencv_contrib231d.lib")
#pragma comment (lib,"opencv_core231d.lib")
#pragma comment (lib,"opencv_features2d231d.lib")
#pragma comment (lib,"opencv_flann231d.lib")
#pragma comment (lib,"opencv_gpu231d.lib")
#pragma comment (lib,"opencv_haartraining_engined.lib")
#pragma comment (lib,"opencv_highgui231d.lib")
#pragma comment (lib,"opencv_imgproc231d.lib")
#pragma comment (lib,"opencv_legacy231d.lib")
#pragma comment (lib,"opencv_ml231d.lib")
#pragma comment (lib,"opencv_objdetect231d.lib")
#pragma comment (lib,"opencv_ts231d.lib")
#pragma comment (lib,"opencv_video231d.lib")

/*
 *《学习OpenCV》第四章第三题a
 * 完成时间：22:17 3/30 星期六 2013
 */

/* 矩形框 */
CvRect rect;

bool draw = false;   // 标记是否在画

IplImage* img;
IplImage * temp;
IplImage * original;

void draw_rect(IplImage* img, CvRect rect)
{
	cvRectangle( img, 
		cvPoint( rect.x, rect.y ),
		cvPoint( rect.x + rect.width, rect.y + rect.height),
		cvScalar( 0x00, 0x00, 0xff) );
	printf("draw\n");
}

// 鼠标回调函数
void my_mouse_callback( int event, int x, int y, int flags, void* param)
{
	IplImage* image = (IplImage*) param;

	switch( event )
	{
	case CV_EVENT_MOUSEMOVE:
		{
			if(draw)
			{
				rect.width = x - rect.x;
				rect.height = y - rect.y;
			}
		}
		break;
	case CV_EVENT_LBUTTONDOWN:
		{
			draw = true;
			rect = cvRect( x, y, 0, 0 );
		}
		break;
	case CV_EVENT_LBUTTONUP:
		{
			draw = false;
			if(rect.width < 0)
			{
				rect.x += rect.width;
				rect.width *= -1;
			}
			if(rect.height < 0)
			{
				rect.y += rect.height;
				rect.height *= -1;
			}
			// draw
			draw_rect(image, rect);
		}
		break;
		// 在右键按下时清除
	case CV_EVENT_RBUTTONDOWN:
		cvCopyImage(original, img);
		printf("clear.\n");
		break;
	}
}

int main()
{
	img = cvLoadImage( "lena.bmp", 1 );

	rect = cvRect( -1, -1, 0, 0);
	
	// 副本
    temp = cvCloneImage( img );
	original = cvCloneImage(img);
	
	cvNamedWindow("draw rect");
	cvSetMouseCallback("draw rect", my_mouse_callback, (void*)img);

	while(1)
	{
		cvCopyImage(img, temp);

		if(draw)
		{
			draw_rect( temp , rect );
		}

		cvShowImage( "draw rect", temp);

		if(cvWaitKey(15) == 27)
			break;
	}
	cvReleaseImage(&img);
	cvReleaseImage(&temp);
	cvReleaseImage(&original);
	cvDestroyAllWindows();

	return 0;
}