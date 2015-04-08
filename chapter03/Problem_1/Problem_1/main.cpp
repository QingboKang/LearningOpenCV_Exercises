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
 * 《学习OpenCV》第三章第一题
 * 完成时间：16:31 3/1 星期五 2013
 */
int main()
{
	/*------------------------------------*/
	// 选取一个负的浮点数
	float f = -3.589;

	// 取绝对值
	float abs_f = CV_IABS(f);

	// 四舍五入，取其极值
	int round_f = cvRound(abs_f);

	printf("absolute value: %f\n", abs_f);
	printf("round value: %d\n", round_f);
	/*------------------------------------*/
	CvRNG rng;
	rng = cvRNG(cvGetTickCount());

	// 产生10个随机整数和小数
	for(int i = 0; i < 10; i++)
	{
		printf("%d\trand integer: %d\t", i, (cvRandInt(&rng) % RAND_INT_MAX ));
		printf("rand floating: %.4f\n", cvRandReal(&rng));
	}
	/*------------------------------------*/
	// 将一个浮点型的CvPoint2D32f转换成一个整数型的CvPoint
	CvPoint2D32f point_2D32f = cvPoint2D32f(2.5, 3.5);
	CvPoint point = cvPointFrom32f(point_2D32f);

	printf("---------------------------------------\n");
	printf("CvPoint2D32f: %f\t%f\n", point_2D32f.x, point_2D32f.y);
	printf("Convert to CvPoint: %d\t%d\n", point.x, point.y);
	// 将一个整数型的CvPoint转换为浮点型的CvPoint2D32f
	CvPoint2D32f point_1 = cvPointTo32f(point);
	printf("Convert to CvPoint2D32f: %f\t%f\n", point_1.x, point_1.y);

	return 0;
}