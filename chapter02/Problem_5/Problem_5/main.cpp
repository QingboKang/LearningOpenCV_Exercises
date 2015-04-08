#include <highgui.h>
#include <cv.h>

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
 *《学习OpenCV》第二章第五题
 * 从摄像机读入视频数据，加入滚动条，可以动态调节缩放比例。
 * 完成时间：23:05 3/31 星期日 2013
 */

int g_slider_position = 0;   // 记录滚动条位置的全局变量
int g_scaling = 2;    // 记录缩放比例的全局变量

void onTrackbarSlide(int pos)
{
	// 改变缩放比例
	g_scaling = pos + 2;
}

IplImage * doPryDown(IplImage * in, int filter = IPL_GAUSSIAN_5x5)
{
	IplImage * out; 
	
	CvSize newSize = cvSize( (int)( in->width / 2 ),(int) (in->height / 2 ) );
	out = cvCreateImage( newSize, in->depth, in->nChannels);
	cvPyrDown( in, out );
	return out;
}

int main()
{
	CvCapture* capture;

	// 选择第一个摄像头
	capture = cvCreateCameraCapture(0);
	if(capture == NULL)
	{
		printf("Camera error.\texit.\n");
		exit(-1);
	}

	// 创建窗口
	cvNamedWindow("Camera", 0);
	
	IplImage *frame = cvQueryFrame(capture);
	IplImage *frame_pyrdown;
	
	if (!frame)
	{
		return 0;
	}
	// 创建滑动条
	cvCreateTrackbar( "scaling", "Camera", &g_slider_position, 
		6, onTrackbarSlide );

	while(1)
	{     
		// 读数据
		frame = cvQueryFrame(capture);
				
		if (!frame)
		{
			break;
		}
		// 缩放变换(2-8之间)
		frame_pyrdown = doPryDown( frame, 7);
		for(int i = 2; i < g_scaling; i++)
		{
			frame_pyrdown = doPryDown( frame_pyrdown, 7);
		}
	
		printf("目前缩放比例: %d\t(%d, %d)\n", g_scaling,
			frame_pyrdown->width, frame_pyrdown->height);

		// 显示出来
		cvShowImage("Camera", frame_pyrdown);

		char c = cvWaitKey(50);
		if (c == 27)
		{
			break;
		}
	}

	// 释放资源
	cvReleaseCapture(&capture);
	cvReleaseImage( &frame);
	cvReleaseImage( &frame_pyrdown);
	cvDestroyWindow("Camera");

	return 0;
}