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
 *《学习OpenCV》第四章第二题
 * 完成时间：0:38 3/11 星期一 2013
 */

CvFont font; 

/*
 * 鼠标事件回调函数
 */
void my_mouse_callback(int event, int x, int y, int flags, void* param)
{
	// 得到图像
	IplImage* image = (IplImage*) param;

	// 获取图像对应像素的颜色值
	CvScalar pixel_value;
	pixel_value = cvGet2D( image, x, y );

	// 构造要显示的文本
	char display_text[14];
	sprintf( display_text, "(%d %d %d)", (int) pixel_value.val[0],
		(int)pixel_value.val[1], (int)pixel_value.val[2] );


	switch(event)
	{
		// 鼠标左键点击事件
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