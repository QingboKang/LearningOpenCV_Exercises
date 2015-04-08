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
 *《学习OpenCV》第三章第五题
 * 完成时间：0:41 3/3 星期日  2013
 */

int main()
{
	// 加载一个三通道的彩色图像
	IplImage * srcImage = cvLoadImage("lena.bmp", 1);

	// 分割成R, G, B三个单通道图像
	IplImage* red_plane = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);
	IplImage* green_plane = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);
	IplImage* blue_plane = cvCreateImage(cvGetSize(srcImage), srcImage->depth, 1);

	// 分离RGB图像的顺序为 B, G, R
	cvSplit(srcImage, blue_plane, green_plane, red_plane,  NULL);

	// 克隆绿图两次
	IplImage* clone1 = cvCloneImage(green_plane);
	IplImage* clone2 = cvCloneImage(green_plane);

	// 找到绿色平面的最大值和最小值
	double dMaxValue, dMinValue;
	cvMinMaxLoc(green_plane, &dMinValue, &dMaxValue, NULL, NULL, NULL);
	printf("Max: %f\tMin: %f\n", dMaxValue, dMinValue);

	// 将clone1的所有元素赋值为thresh = (unsigned char)((max - min)/2))
	unsigned char thresh = (dMaxValue - dMinValue)/2;
	cvSet(clone1, cvScalarAll(thresh), 0);
	// 将clone2的所有元素赋零
	cvSet(clone2, cvScalarAll(0), 0);

	// 生成掩码图像
	cvCmp(green_plane, clone1, clone2, CV_CMP_GE);

	// 显示结果
	cvSubS(green_plane, cvScalarAll(thresh/2), green_plane, clone2 );

	cvShowImage("clone1", clone1);
	cvShowImage("clone2", clone2);

	cvShowImage("红", red_plane);
	cvShowImage("绿", green_plane);
	cvShowImage("蓝", blue_plane);
	cvShowImage("原始图像", srcImage);
	
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