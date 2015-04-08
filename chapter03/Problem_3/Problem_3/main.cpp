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
 * ���ʱ�䣺1:34 3/2 ������ 2013
 */

#define ROWS 400  // ��������
#define COLS 400  // ��������

// ������ɫ�����ε���������
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
	// ��ͨ���Ķ�ά�ֽ����;���
	CvMat* mat = cvCreateMat(ROWS, COLS, CV_8UC3);

	// ��������ֵ��ʼ��Ϊ�� 
    cvZero(mat);
	// ��þ���Ԫ��(0,0)��ָ��
	uchar *p = (uchar*)cvPtr2D(mat, 0, 0);
	// Ϊ����ֵ����������
	for(int i = 0; i < mat->cols; i++)
	{
		for(int j = 0; j < mat->rows; j++)
		{
			p = (uchar*)cvPtr2D(mat, i, j);
			// ʹ��ָ���м��ͨ��
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