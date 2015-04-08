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
 *��ѧϰOpenCV�������µ�����
 * ���ʱ�䣺2:13 3/2 ������ 2013
 */

// ͼ���ߺ궨��
#define IMG_WIDTH  400
#define IMG_HEIGHT 400

// �������� 
CvPoint point1 = cvPoint(80, 20);
CvPoint point2 = cvPoint(160, 80);

// Help function
// ��鵱ǰ���Ƿ���Ҫ���ĳ�����������
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
	// ����һ����ͨ��RGBͼ��
	IplImage* image = cvCreateImage(cvSize(IMG_WIDTH, IMG_HEIGHT), 8, 3 );
	
	// ������Ԫ��ȫ������
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