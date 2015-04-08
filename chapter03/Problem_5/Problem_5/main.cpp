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
 * ���ʱ�䣺2:49 3/2 ������ 2013
 */

// ͼ��߿�
#define IMG_SIZE   420

// �߽���
#define BORDER_WIDTH 10

int main()
{
	// ����8λ��ͨ��ͼ��
	IplImage * image = cvCreateImage(cvSize(IMG_SIZE, IMG_SIZE), 8, 1);

	// ��ʼ������
	cvZero(image);
	
	// �Ҷ�ֵ
	int value = 0;
	// ROI�����С
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