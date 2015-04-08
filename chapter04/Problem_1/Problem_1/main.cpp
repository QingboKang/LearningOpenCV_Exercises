#include <highgui.h>
#include <cv.h>

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
 *《学习OpenCV》第四章第一题a部分
 * 完成时间：1:09 3/10 星期日 2013
 */
int main()
{
	char * file_path = "H:/TDDOWNLOAD/AV/麻生希 AV DEBUT STAR362.avi";
	CvCapture * capture = 0;
	capture = cvCreateFileCapture(file_path);

	// 载入视频文件失败
	if(capture == NULL)
	{
		printf("Can't load the video file, quit...\n");
		return 0;
	}
	
	// 每一帧
	IplImage * frame;
	// 灰度后的每一帧
	IplImage * gray_frame;
	// 边缘检测后的每一帧
	IplImage * canny_frame;

	while(1)
	{
		// 从视频文件读入数据
		frame = cvQueryFrame(capture);
		if(!frame)
			break;

		// 将读入数据转换为灰度图
		gray_frame = cvCreateImage( cvGetSize(frame), frame->depth, 1);
		cvCvtColor( frame, gray_frame, CV_RGB2GRAY );
		
		// 对图像做Canny边缘检测
		canny_frame = cvCreateImage( cvGetSize(frame), frame->depth, 1);
		cvCanny( gray_frame, canny_frame, 30, 100, 3);

		// 显示图像
		cvShowImage("color", frame);
		cvShowImage("gray", gray_frame);
		cvShowImage("canny", canny_frame);
	
		char c = cvWaitKey(4);
		if(c == 27)
			break;
	}
	cvReleaseCapture(&capture);
	cvDestroyAllWindows();

	return 0;
}
