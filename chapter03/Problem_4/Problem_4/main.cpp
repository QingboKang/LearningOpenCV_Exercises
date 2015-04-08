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
 *《学习OpenCV》第三章第四题
 * 完成时间：2:13 3/2 星期六 2013
 */

// 图像宽高宏定义
#define IMG_WIDTH  400
#define IMG_HEIGHT 400

// 两个顶点 
CvPoint point1 = cvPoint(80, 20);
CvPoint point2 = cvPoint(160, 80);

// Help function
// 检查当前点是否在要画的长方形区域内
bool isThePointInRectangle(int x, int y)
{
	if( (x <= point2.x) && (y <= point2.y) 
		& (x >= point1.x) && (y >= point1.y))
		return true;
	else 
		return false;
}


int main()
{
	// 创建一个三通道RGB图像
	IplImage* image = cvCreateImage(cvSize(IMG_WIDTH, IMG_HEIGHT), 8, 3 );
	
	// 将矩阵元素全部置零
	cvZero(image);
	uchar* p;
	for(int i = 0; i < image->height; i++)
	{
		for(int j = 0; j < image->width; j++)
		{
		    p = cvPtr2D(image, i, j, 0);
			if(isThePointInRectangle(i, j) == true)
			{
				*(++p) = 255;
			}
		}
	}
	cvNamedWindow("img", 1);
	cvShowImage("img", image);
	cvWaitKey(0);

	cvReleaseImage(&image);
	cvDestroyWindow("img");

	return 0; 
}