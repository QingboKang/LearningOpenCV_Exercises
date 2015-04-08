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
 * ���ʱ�䣺3:42 3/2 ������  2013
 */

#define NEW_IMG_WIDTH  200
#define NEW_IMG_HEIGHT 300

int main()
{
	// ����ͼ��
	IplImage* lena = cvLoadImage("lena.png", 0);

	// ����ͼ��ͷ
	IplImage* img_1 = cvCreateImageHeader(cvSize( NEW_IMG_WIDTH, NEW_IMG_HEIGHT), 
		lena->depth, lena->nChannels );
	IplImage* img_2 = cvCreateImageHeader(cvSize( NEW_IMG_WIDTH, NEW_IMG_HEIGHT), 
		lena->depth, lena->nChannels );
	// ������ͼ��ͷ��imageData
	img_1->imageData = (char*) cvPtr2D(lena, 5, 10);
	img_2->imageData = (char*) cvPtr2D(lena, 300, 310);
	
	//cvShowImage("img_1", img_1);
	cvNot( img_2, img_2);
	cvNot(img_1, img_1);

	cvShowImage("img", lena);
	
	cvWaitKey(0);
	cvReleaseImage(&lena);
	cvDestroyAllWindows();


	return 0;
}