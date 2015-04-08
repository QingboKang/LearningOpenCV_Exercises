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
 *��ѧϰOpenCV���ڶ��µ�����
 * �������������Ƶ���ݣ���������������Զ�̬�������ű�����
 * ���ʱ�䣺23:05 3/31 ������ 2013
 */

int g_slider_position = 0;   // ��¼������λ�õ�ȫ�ֱ���
int g_scaling = 2;    // ��¼���ű�����ȫ�ֱ���

void onTrackbarSlide(int pos)
{
	// �ı����ű���
	g_scaling = pos + 2;
}

IplImage * doPryDown(IplImage * in, int filter = IPL_GAUSSIAN_5x5)
{
	IplImage * out; 
	
	CvSize newSize = cvSize( (int)( in->width / 2 ),(int) (in->height / 2 ) );
	out = cvCreateImage( newSize, in->depth, in->nChannels);
	cvPyrDown( in, out );
	return out;
}

int main()
{
	CvCapture* capture;

	// ѡ���һ������ͷ
	capture = cvCreateCameraCapture(0);
	if(capture == NULL)
	{
		printf("Camera error.\texit.\n");
		exit(-1);
	}

	// ��������
	cvNamedWindow("Camera", 0);
	
	IplImage *frame = cvQueryFrame(capture);
	IplImage *frame_pyrdown;
	
	if (!frame)
	{
		return 0;
	}
	// ����������
	cvCreateTrackbar( "scaling", "Camera", &g_slider_position, 
		6, onTrackbarSlide );

	while(1)
	{     
		// ������
		frame = cvQueryFrame(capture);
				
		if (!frame)
		{
			break;
		}
		// ���ű任(2-8֮��)
		frame_pyrdown = doPryDown( frame, 7);
		for(int i = 2; i < g_scaling; i++)
		{
			frame_pyrdown = doPryDown( frame_pyrdown, 7);
		}
	
		printf("Ŀǰ���ű���: %d\t(%d, %d)\n", g_scaling,
			frame_pyrdown->width, frame_pyrdown->height);

		// ��ʾ����
		cvShowImage("Camera", frame_pyrdown);

		char c = cvWaitKey(50);
		if (c == 27)
		{
			break;
		}
	}

	// �ͷ���Դ
	cvReleaseCapture(&capture);
	cvReleaseImage( &frame);
	cvReleaseImage( &frame_pyrdown);
	cvDestroyWindow("Camera");

	return 0;
}