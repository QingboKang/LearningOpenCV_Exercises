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
 *��ѧϰOpenCV�������µ�һ��b����
 * ���ʱ�䣺19:24 3/10 ������ 2013
 */

int main()
{
	char * file_path = "H:/TDDOWNLOAD/AV/1111.avi";
	CvCapture * capture = 0;
	capture = cvCreateFileCapture(file_path);

	// ������Ƶ�ļ�ʧ��
	if(capture == NULL)
	{
		printf("Can't load the video file, quit...\n");
		return 0;
	}
	
	// ÿһ֡
	IplImage * frame = cvQueryFrame(capture);;
	// �ҶȺ��ÿһ֡
	IplImage * gray_frame = cvCreateImage( cvGetSize(frame), frame->depth, 1);
	IplImage * gray_frame_3 = cvCreateImage(cvGetSize(frame), frame->depth, 3);
	// ��Ե�����ÿһ֡
	IplImage * canny_frame = cvCreateImage( cvGetSize(frame), frame->depth, 1);
	IplImage * canny_frame_3 = cvCreateImage(cvGetSize(frame), frame->depth, 3);
	// �ۺ�����ÿһ֡
	IplImage * total_frame = cvCreateImage( cvSize(frame->width * 3, frame->height),
		frame->depth, frame->nChannels);

	while(1)
	{
		// ����Ƶ�ļ���������
		frame = cvQueryFrame(capture);
		if(!frame)
			break;

		// ����������ת��Ϊ�Ҷ�ͼ
		cvCvtColor( frame, gray_frame, CV_RGB2GRAY );
		cvCvtColor( gray_frame, gray_frame_3, CV_GRAY2BGR );
		
		// ��ͼ����Canny��Ե���
		cvCanny( gray_frame, canny_frame, 30, 100, 3);
		cvCvtColor( canny_frame, canny_frame_3, CV_GRAY2BGR );

		// ������ͼ��ֱ��Ƶ��µ�ͼ����
		cvSetImageROI( total_frame, cvRect(0, 0, frame->width, frame->height));
		cvCopy( frame, total_frame, 0);
		cvResetImageROI(total_frame);
				
		cvSetImageROI( total_frame, cvRect(frame->width, 0, gray_frame->width, gray_frame->height));
		cvCopy( gray_frame_3, total_frame, 0);
		cvResetImageROI(total_frame);
				
		cvSetImageROI( total_frame, cvRect(frame->width*2, 0, canny_frame->width, canny_frame->height));
		cvCopy( canny_frame_3, total_frame, 0);
		cvResetImageROI(total_frame);

		// ��ͼ���������ͬ�Ĳ���д�Ϻ��ʵ����ֱ�ǩ
		CvFont textFont = cvFont( 10, 1);
		cvInitFont( &textFont, CV_FONT_HERSHEY_SIMPLEX, 0.5f, 0.5f, 0, 1 );

		cvPutText( total_frame, "Frame", cvPoint( 10, 20 ), &textFont, cvScalar( 0, 0, 255 ) );
		cvPutText( total_frame, "Frame_Gray", cvPoint( frame->width + 10, 20 ), &textFont, cvScalar( 0, 0, 255 ) );
		cvPutText( total_frame, "Frame_Canny", cvPoint( frame->width * 2 + 10, 20 ), &textFont, cvScalar( 0, 0, 255 ) );



		// ��ʾͼ��
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