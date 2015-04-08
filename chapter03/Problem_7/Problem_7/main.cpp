#include "opencv_libs.h"
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
 * ���ʱ�䣺0:41 3/3 ������  2013
 */

int main()
{
	// ����һ����ͨ���Ĳ�ɫͼ��
	IplImage * srcImage = cvLoadImage("lena.bmp", 1);

	// �ָ��R, G, B������ͨ��ͼ��
	IplImage* red_plane = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);
	IplImage* green_plane = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);
	IplImage* blue_plane = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);

	// ����RGBͼ���˳��Ϊ B, G, R
	cvSplit(srcImage, blue_plane, green_plane, red_plane,  NULL);

	// ��¡��ͼ����
	IplImage* clone1 = cvCloneImage(green_plane);
	IplImage* clone2 = cvCloneImage(green_plane);

	// �ҵ���ɫƽ������ֵ����Сֵ
	double dMaxValue, dMinValue;
	cvMinMaxLoc(green_plane, &dMinValue, &dMaxValue, NULL, NULL, NULL);
	printf("Max: %f\tMin: %f\n", dMaxValue, dMinValue);

	// ��clone1������Ԫ�ظ�ֵΪthresh = (unsigned char)((max - min)/2))
	unsigned char thresh = (dMaxValue - dMinValue)/2;
	cvSet(clone1, cvScalarAll(thresh), 0);
	// ��clone2������Ԫ�ظ���
	cvSet(clone2, cvScalarAll(0), 0);

	// ��������ͼ��
	cvCmp(green_plane, clone1, clone2, CV_CMP_GE);

	// ��ʾ���
	cvSubS(green_plane, cvScalarAll(thresh/2), green_plane, clone2 );

	cvShowImage("clone1", clone1);
	cvShowImage("clone2", clone2);

	cvShowImage("��", red_plane);
	cvShowImage("��", green_plane);
	cvShowImage("��", blue_plane);
	cvShowImage("ԭʼͼ��", srcImage);
	
	cvvWaitKey(0);

	cvReleaseImage(&srcImage);
	cvReleaseImage(&red_plane);
	cvReleaseImage(&green_plane);
	cvReleaseImage(&blue_plane);
	cvReleaseImage(&clone1);
	cvReleaseImage(&clone2);
	cvDestroyAllWindows();
	return 0;
}