#include <highgui.h>
#include <cv.h>
#include <stdio.h>

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
 *《学习OpenCV》第三章第五题
 * 完成时间：2:49 3/2 星期六 2013
 */

// 图像高宽
#define IMG_SIZE   420

// 边界宽度
#define BORDER_WIDTH 10

int main()
{
	// 创建8位单通道图像
	IplImage * image = cvCreateImage(cvSize(IMG_SIZE, IMG_SIZE), 8, 1);

	// 初始化置零
	cvZero(image);
	
	// 灰度值
	int value = 0;
	// ROI区域大小
	int rect_size = IMG_SIZE;
	for(int length = IMG_SIZE; length >= 0 && value <= 200; length -= BORDER_WIDTH, value += 10)
	{
		cvSetImageROI(image, cvRect(IMG_SIZE - length, IMG_SIZE- length, rect_size, rect_size));

		rect_size -= 2 *BORDER_WIDTH;
		cvSet(image, cvScalarAll(value));
		cvResetImageROI(image);
	}

	cvNamedWindow("img", 1);
	cvShowImage("img", image);

	cvWaitKey(0);
	cvReleaseImage(&image);
	cvDestroyAllWindows();

	return 0;
}