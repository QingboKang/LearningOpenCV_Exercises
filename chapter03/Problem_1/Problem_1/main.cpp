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

#define RAND_INT_MAX 256

/*
 * ��ѧϰOpenCV�������µ�һ��
 * ���ʱ�䣺16:31 3/1 ������ 2013
 */
int main()
{
	/*------------------------------------*/
	// ѡȡһ�����ĸ�����
	float f = -3.589;

	// ȡ����ֵ
	float abs_f = CV_IABS(f);

	// �������룬ȡ�伫ֵ
	int round_f = cvRound(abs_f);

	printf("absolute value: %f\n", abs_f);
	printf("round value: %d\n", round_f);
	/*------------------------------------*/
	CvRNG rng;
	rng = cvRNG(cvGetTickCount());

	// ����10�����������С��
	for(int i = 0; i < 10; i++)
	{
		printf("%d\trand integer: %d\t", i, (cvRandInt(&rng) % RAND_INT_MAX ));
		printf("rand floating: %.4f\n", cvRandReal(&rng));
	}
	/*------------------------------------*/
	// ��һ�������͵�CvPoint2D32fת����һ�������͵�CvPoint
	CvPoint2D32f point_2D32f = cvPoint2D32f(2.5, 3.5);
	CvPoint point = cvPointFrom32f(point_2D32f);

	printf("---------------------------------------\n");
	printf("CvPoint2D32f: %f\t%f\n", point_2D32f.x, point_2D32f.y);
	printf("Convert to CvPoint: %d\t%d\n", point.x, point.y);
	// ��һ�������͵�CvPointת��Ϊ�����͵�CvPoint2D32f
	CvPoint2D32f point_1 = cvPointTo32f(point);
	printf("Convert to CvPoint2D32f: %f\t%f\n", point_1.x, point_1.y);

	return 0;
}