#include <highgui.h>
#include <cv.h>
#include "opencv_libs.h"

/*
 *《学习OpenCV》第四章第三题b
 * 完成时间：1:36 3/31 星期日 2013
 */

/* 矩形框 */
CvRect rect;

bool draw = false;   // 标记是否在画

IplImage* img;
IplImage * temp;
IplImage * original;

bool draw_hist = false;

IplImage* getHistImage(IplImage* image, CvHistogram* image_hist, 
					   CvSize image_size, CvScalar value)
{
	// 计算直方图
	cvCalcHist( &image, image_hist, 0, NULL );
	// 新建一幅3通道的图像
	IplImage* dst = cvCreateImage(image_size, IPL_DEPTH_8U, 3 );
	
	cvSet( dst, cvScalarAll(255) );
	
	float max_value = 0;
	cvGetMinMaxHistValue( image_hist, NULL, &max_value, NULL, NULL );
	double bin_width = (double)dst->width/256;
	double bin_unith = (double)dst->height/max_value;   // 高度比例

	for(int i = 0; i < 256; i++)
	{
		// 获得矩形左上角和右下角坐标
		CvPoint p0 = cvPoint( i + bin_width, dst->height );
		CvPoint p1 = cvPoint( (i+1) * bin_width, 
			dst->height - cvGetReal1D(image_hist->bins, i) *  bin_unith );
		// 画实心矩形
		cvRectangle( dst, p0, p1, value, -1, 8, 0 );
	}

	return dst;
}

void draw_rect(IplImage* img, CvRect rect)
{
	cvRectangle( img, 
		cvPoint( rect.x, rect.y ),
		cvPoint( rect.x + rect.width, rect.y + rect.height),
		cvScalar( 0x00, 0x00, 0xff) );
	printf("draw\n");

	// 在这里处理直方图
	// 设置感兴趣区域
	cvSetImageROI( img, rect);
	IplImage* src_rect = cvCreateImage ( 
		cvSize( rect.width, rect.height ), 
		img->depth, img->nChannels );
	cvCopy(img, src_rect );
	cvResetImageROI( img );

	IplImage* r_img = cvCreateImage( cvGetSize( src_rect),
		src_rect->depth, 1 );
	IplImage* g_img = cvCreateImage( cvGetSize( src_rect),
		src_rect->depth, 1 );
	IplImage* b_img = cvCreateImage( cvGetSize( src_rect),
		src_rect->depth, 1 );
	IplImage* gray_img = cvCreateImage( cvGetSize( src_rect),
		src_rect->depth, 1 );
	
	// 分离RGB分量
	cvSplit( src_rect, r_img, g_img, b_img, NULL);
	cvShowImage( "red", r_img);
	cvShowImage( "green", g_img);
	cvShowImage( "blue", b_img);
	
	// 灰度转换
	cvCvtColor( src_rect, gray_img, CV_BGR2GRAY);
	int size = 256;
	float range[] = {0, 255};
	float* ranges[] = {range};
	// 创建直方图
	CvHistogram * r_hist = cvCreateHist( 1, &size, CV_HIST_ARRAY, ranges, 1);
	CvHistogram * g_hist = cvCreateHist( 1, &size, CV_HIST_ARRAY, ranges, 1);
	CvHistogram * b_hist = cvCreateHist( 1, &size, CV_HIST_ARRAY, ranges, 1);
	CvHistogram * gray_hist = cvCreateHist( 1, &size, CV_HIST_ARRAY, ranges, 1);

	// 直方图尺寸
	CvSize image_size = cvSize( 400, 300);
	
	IplImage* r_dst = getHistImage(r_img, r_hist, image_size, cvScalar(0x00, 0x00, 0xff));
	IplImage* g_dst = getHistImage(g_img, g_hist, image_size, cvScalar(0x00, 0xff, 0x00));
	IplImage* b_dst = getHistImage(b_img, b_hist, image_size, cvScalar(0xff, 0x00, 0x00));
	IplImage* gray_dst = getHistImage( gray_img, gray_hist, image_size, cvScalar(0) );

	// 把四个直方图在一幅图片上显示出来
	IplImage* dst = cvCreateImage( cvSize( image_size.width * 2, image_size.height * 2), 8, 3 );
	cvSetZero( dst );
	// 拷贝红色分量直方图
	CvRect r_rect = cvRect( 0, 0, image_size.width, image_size.height);
	cvSetImageROI(dst, r_rect);
	cvCopy( r_dst, dst);
	// 拷贝绿色分量直方图
	CvRect g_rect = cvRect(image_size.width, 0, image_size.width, image_size.height );
	cvSetImageROI( dst, g_rect);
	cvCopy( g_dst, dst);
	// 蓝色分量
	CvRect b_rect = cvRect(0, image_size.height, image_size.width, image_size.height );
	cvSetImageROI(dst, b_rect);
	cvCopy( b_dst, dst );
	// 灰度分量
	CvRect gray_rect = cvRect( image_size.width, image_size.height, 
		image_size.width, image_size.height );
	cvSetImageROI( dst, gray_rect);
	cvCopy( gray_dst, dst);

	cvResetImageROI( dst );

	cvShowImage( "src", src_rect);
	cvShowImage( "dst", dst );

	cvWaitKey(0);

	cvDestroyAllWindows();
	cvReleaseImage( &r_img );
	cvReleaseImage(&g_img);
	cvReleaseImage(&b_img);
	cvReleaseImage(&gray_img);
	cvReleaseImage(&r_dst);
	cvReleaseImage(&g_dst);
	cvReleaseImage(&b_dst);
	cvReleaseImage(&gray_dst);
	cvReleaseImage(&src_rect);
	cvReleaseImage(&dst);
}

// 鼠标回调函数
void my_mouse_callback( int event, int x, int y, int flags, void* param)
{
	IplImage* image = (IplImage*) param;

	switch( event )
	{
	case CV_EVENT_MOUSEMOVE:
		{
			if(draw)
			{
				rect.width = x - rect.x;
				rect.height = y - rect.y;
			}

			draw_hist = false;
		}
		break;
	case CV_EVENT_LBUTTONDOWN:
		{
			draw = true;
			rect = cvRect( x, y, 0, 0 );
			draw_hist = false;
		}
		break;
	case CV_EVENT_LBUTTONUP:
		{
			draw = false;
			draw_hist = true;
			if(rect.width < 0)
			{
				rect.x += rect.width;
				rect.width *= -1;
			}
			if(rect.height < 0)
			{
				rect.y += rect.height;
				rect.height *= -1;
			}
			// draw
			draw_rect(image, rect);
		}
		break;
		// 在右键按下时清除
	case CV_EVENT_RBUTTONDOWN:
		cvCopyImage(original, img);
		printf("clear.\n");
		break;
	}
}

int main()
{
	img = cvLoadImage( "lena.bmp", 1 );

	rect = cvRect( -1, -1, 0, 0);
	
	// 副本
    temp = cvCloneImage( img );
	original = cvCloneImage(img);
	
	cvNamedWindow("draw rect");
	cvSetMouseCallback("draw rect", my_mouse_callback, (void*)img);

	while(1)
	{
		cvCopyImage(img, temp);

		if(draw_hist)
		{
			draw_rect( temp , rect );
		}

		cvShowImage( "draw rect", temp);

		if(cvWaitKey(15) == 27)
			break;
	}
	cvReleaseImage(&img);
	cvReleaseImage(&temp);
	cvDestroyAllWindows();

	return 0;
}