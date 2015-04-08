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

#define ROWS 400
#define COLS 400

/*
 *��ѧϰOpenCV�������µڶ���
 * ���ʱ�䣺0:22 3/2 ������ 2013
 */
int main()
{
	// ����һ����ͨ����ά���󣬴�СΪROWS*COLS
	CvMat* mat = cvCreateMat(ROWS, COLS, CV_8UC3);
	
	// ��������ֵ��ʼ��Ϊ�� 
	for(int i = 0; i < mat->cols; i++)
	{
		for(int j = 0;j < mat->rows; j++)
		{
			cvSet2D(mat, i, j, cvScalarAll(0));
		}
	}

	// ��һ��Բ
	cvCircle(mat, cvPoint(ROWS/2, COLS/2), ROWS/4, cvScalar(255, 255, 0), 3, 8, 0);

    // ��ʾͼ��
	cvShowImage("img", mat);
	cvWaitKey(0);

	cvReleaseMat(&mat);
	cvDestroyAllWindows();

	return 0;
}