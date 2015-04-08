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
 *��ѧϰOpenCV�������µ�һ��a����
 * ���ʱ�䣺1:09 3/10 ������ 2013
 */
int main()
{
	char * file_path = "H:/TDDOWNLOAD/AV/����ϣ AV DEBUT STAR362.avi";
	CvCapture * capture = 0;
	capture = cvCreateFileCapture(file_path);

	// ������Ƶ�ļ�ʧ��
	if(capture == NULL)
	{
		printf("Can't load the video file, quit...\n");
		return 0;
	}
	
	// ÿһ֡
	IplImage * frame;
	// �ҶȺ��ÿһ֡
	IplImage * gray_frame;
	// ��Ե�����ÿһ֡
	IplImage * canny_frame;

	while(1)
	{
		// ����Ƶ�ļ���������
		frame = cvQueryFrame(capture);
		if(!frame)
			break;

		// ����������ת��Ϊ�Ҷ�ͼ
		gray_frame = cvCreateImage( cvGetSize(frame), frame->depth, 1);
		cvCvtColor( frame, gray_frame, CV_RGB2GRAY );
		
		// ��ͼ����Canny��Ե���
		canny_frame = cvCreateImage( cvGetSize(frame), frame->depth, 1);
		cvCanny( gray_frame, canny_frame, 30, 100, 3);

		// ��ʾͼ��
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
