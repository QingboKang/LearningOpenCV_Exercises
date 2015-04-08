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
 *《学习OpenCV》第三章第三题
 * 完成时间：1:34 3/2 星期六 2013
 */

#define ROWS 400  // 矩阵行数
#define COLS 400  // 矩阵列数

// 定义绿色长方形的两个顶点
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
	// 三通道的二维字节类型矩阵
	CvMat* mat = cvCreateMat(ROWS, COLS, CV_8UC3);

	// 将所有数值初始化为零 
    cvZero(mat);
	// 获得矩阵元素(0,0)的指针
	uchar *p = (uchar*)cvPtr2D(mat, 0, 0);
	// 为矩阵赋值，画长方形
	for(int i = 0; i < mat->cols; i++)
	{
		for(int j = 0; j < mat->rows; j++)
		{
			p = (uchar*)cvPtr2D(mat, i, j);
			// 使其指向中间的通道
			p++;
			if(isThePointInRectangle(i, j) == true)
			{
				*p = 255;
			}
			p++;
		}
	}

	cvShowImage("img", mat);
	cvWaitKey(0);

	cvReleaseMat(&mat);
	cvDestroyAllWindows(); 

	return 0;
}