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
 *��ѧϰOpenCV�������µڶ���
 * ���ʱ�䣺0:38 3/11 ����һ 2013
 */

CvFont font; 

/*
 * ����¼��ص�����
 */
void my_mouse_callback(int event, int x, int y, int flags, void* param)
{
	// �õ�ͼ��
	IplImage* image = (IplImage*) param;

	// ��ȡͼ���Ӧ���ص���ɫֵ
	CvScalar pixel_value;
	pixel_value = cvGet2D( image, x, y );

	// ����Ҫ��ʾ���ı�
	char display_text[14];
	sprintf( display_text, "(%d %d %d)", (int) pixel_value.val[0],
		(int)pixel_value.val[1], (int)pixel_value.val[2] );


	switch(event)
	{
		// ����������¼�
	case CV_EVENT_LBUTTONDOWN:
		printf("(%d, %d) --- (%f\t%f\t%f)\n", x, y, pixel_value.val[0], pixel_value.val[1], pixel_value.val[2]);
		cvPutText( image, display_text, cvPoint(x, y), &font, pixel_value);
		break;
	}
}

int main()
{
	IplImage* image = cvLoadImage("1.jpg", 1);
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0f, 1.0f, 0, 1, 8);
	
	cvNamedWindow("image");
	cvSetMouseCallback( "image", my_mouse_callback, (void*)image );

	while(1)
	{	
		cvShowImage("image", image);

		if(cvWaitKey(15) == 27) break;
	}

	cvReleaseImage( &image );
	cvDestroyAllWindows();
	return 0;
}