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
 *《学习OpenCV》第四章第一题b部分
 * 完成时间：19:24 3/10 星期日 2013
 */

int main()
{
	char * file_path = "H:/TDDOWNLOAD/AV/1111.avi";
	CvCapture * capture = 0;
	capture = cvCreateFileCapture(file_path);

	// 载入视频文件失败
	if(capture == NULL)
	{
		printf("Can't load the video file, quit...\n");
		return 0;
	}
	
	// 每一帧
	IplImage * frame = cvQueryFrame(capture);;
	// 灰度后的每一帧
	IplImage * gray_frame = cvCreateImage( cvGetSize(frame), frame->depth, 1);
	IplImage * gray_frame_3 = cvCreateImage(cvGetSize(frame), frame->depth, 3);
	// 边缘检测后的每一帧
	IplImage * canny_frame = cvCreateImage( cvGetSize(frame), frame->depth, 1);
	IplImage * canny_frame_3 = cvCreateImage(cvGetSize(frame), frame->depth, 3);
	// 聚合来的每一帧
	IplImage * total_frame = cvCreateImage( cvSize(frame->width * 3, frame->height),
		frame->depth, frame->nChannels);

	while(1)
	{
		// 从视频文件读入数据
		frame = cvQueryFrame(capture);
		if(!frame)
			break;

		// 将读入数据转换为灰度图
		cvCvtColor( frame, gray_frame, CV_RGB2GRAY );
		cvCvtColor( gray_frame, gray_frame_3, CV_GRAY2BGR );
		
		// 对图像做Canny边缘检测
		cvCanny( gray_frame, canny_frame, 30, 100, 3);
		cvCvtColor( canny_frame, canny_frame_3, CV_GRAY2BGR );

		// 将三幅图像分别复制到新的图像中
		cvSetImageROI( total_frame, cvRect(0, 0, frame->width, frame->height));
		cvCopy( frame, total_frame, 0);
		cvResetImageROI(total_frame);
				
		cvSetImageROI( total_frame, cvRect(frame->width, 0, gray_frame->width, gray_frame->height));
		cvCopy( gray_frame_3, total_frame, 0);
		cvResetImageROI(total_frame);
				
		cvSetImageROI( total_frame, cvRect(frame->width*2, 0, canny_frame->width, canny_frame->height));
		cvCopy( canny_frame_3, total_frame, 0);
		cvResetImageROI(total_frame);

		// 在图像的三个不同的部分写上合适的文字标签
		CvFont textFont = cvFont( 10, 1);
		cvInitFont( &textFont, CV_FONT_HERSHEY_SIMPLEX, 0.5f, 0.5f, 0, 1 );

		cvPutText( total_frame, "Frame", cvPoint( 10, 20 ), &textFont, cvScalar( 0, 0, 255 ) );
		cvPutText( total_frame, "Frame_Gray", cvPoint( frame->width + 10, 20 ), &textFont, cvScalar( 0, 0, 255 ) );
		cvPutText( total_frame, "Frame_Canny", cvPoint( frame->width * 2 + 10, 20 ), &textFont, cvScalar( 0, 0, 255 ) );



		// 显示图像
		cvShowImage("total", total_frame);
	
		char c = cvWaitKey(33);
		if(c == 27)
			break;
	}
	cvReleaseImage( & gray_frame );
	cvReleaseCapture(&capture);
	cvDestroyAllWindows();

	return 0; 
}