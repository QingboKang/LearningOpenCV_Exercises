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
 * �������������Ƶ���ݣ��������ű任�����任�����ʾ�ڴ����С�
 * ���ʱ�䣺22:18 3/31 ������ 2013
 */

IplImage * doPryDown(IplImage * in, int filter = IPL_GAUSSIAN_5x5)
{
	IplImage * out; 
	if(in->width % 2 == 0 && in->height % 2 == 0)
	{
		out = cvCreateImage( cvSize( in->width/2, in->height/2 ),
			in->depth, in->nChannels);
		cvPyrDown( in, out );

		return out;
	}
	else
	{
		out = cvCreateImage( cvSize( (in->width+1) / 2, (in->height+1)/2 ),
			in->depth, in->nChannels);
		cvPyrDown( in, out );

		return out;
	}

}

int main()
{
	CvCapture* capture;

	// ѡ��ڶ�������ͷ
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

	while(1)
	{     
		// ������
		frame = cvQueryFrame(capture);
				
		if (!frame)
		{
			break;
		}

		// ԭʼ�ߴ�
		printf("original size: {%d %d}\n", frame->width, frame->height);
		// ���ű任
		frame_pyrdown = doPryDown( frame, 7 );
		// �任��ĳߴ�
		printf("after pyrdown size: {%d %d}\n", frame_pyrdown->width, frame_pyrdown->height);
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