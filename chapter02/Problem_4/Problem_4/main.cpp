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
 *《学习OpenCV》第二章第四题
 * 从摄像机读入视频数据，并做缩放变换，将变换结果显示在窗口中。
 * 完成时间：22:18 3/31 星期日 2013
 */

IplImage * doPryDown(IplImage * in, int filter = IPL_GAUSSIAN_5x5)
{
	IplImage * out; 
	if(in->width % 2 == 0 && in->height % 2 == 0)
	{
		out = cvCreateImage( cvSize( in->width/2, in->height/2 ),
			in->depth, in->nChannels);
		cvPyrDown( in, out );

		return out;
	}
	else
	{
		out = cvCreateImage( cvSize( (in->width+1) / 2, (in->height+1)/2 ),
			in->depth, in->nChannels);
		cvPyrDown( in, out );

		return out;
	}

}

int main()
{
	CvCapture* capture;

	// 选择第二个摄像头
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

	while(1)
	{     
		// 读数据
		frame = cvQueryFrame(capture);
				
		if (!frame)
		{
			break;
		}

		// 原始尺寸
		printf("original size: {%d %d}\n", frame->width, frame->height);
		// 缩放变换
		frame_pyrdown = doPryDown( frame, 7 );
		// 变换后的尺寸
		printf("after pyrdown size: {%d %d}\n", frame_pyrdown->width, frame_pyrdown->height);
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