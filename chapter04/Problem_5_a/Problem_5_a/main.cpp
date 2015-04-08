#include "opencv_libs.h"
#include <highgui.h>
#include <cv.h>
#include <math.h>


/*
 *《学习OpenCV》第四章第五题a
 * 完成时间：8:08 4/5 星期五 2013
 */

#define IMG_WIDTH 512    // 图像宽
#define IMG_HEIGHT 512   // 图像高

#define RAND_INT_MAX 256

// 不同模式的定义
#define MODE_LINE     1     // 画线
#define MODE_CIRCLE   2     // 画圆
#define MODE_ELLIPSE  3     // 画椭圆
#define MODE_POLYGON  4     // 画多边形

// 当前模式
int g_draw_mode = 0;

typedef struct MyPoints
{
	CvPoint point;
	struct MyPoints* next;
} myPoints;

IplImage * image;
IplImage * temp;

// 随机数产生器
CvRNG rng = cvRNG(cvGetTickCount()); 

// 是否在画线
bool drawing_line = false;
// 画线--起点、终点
CvPoint g_Line_startpoint = cvPoint(-1, -1);
CvPoint g_Line_endpoint = cvPoint(-1, -1);

// 是否在画圆
bool drawing_circle = false;
// 画圆--圆心、半径
CvPoint g_Circle_center = cvPoint(-1, -1);
int g_Circle_radius = -1;

// 是否在画椭圆
bool drawing_ellipse = false;
// 画椭圆--圆心、长短半轴
CvPoint g_ellipse_center = cvPoint(-1, -1);
CvSize g_ellipse_axes = cvSize( -1, -1 );

// 是否在画多边形
bool drawing_polygon = false;
// 画多边形--所有点的数据
CvPoint** g_polygon_pts = (CvPoint**)malloc(sizeof(CvPoint*));
int* g_polygon_npts;
myPoints* pHead, *pTail, *pCurr;
int iNumofPoints = 0;

// 生成随机颜色
CvScalar getRandColor()
{
	return cvScalar( (cvRandInt(&rng) % RAND_INT_MAX),
		(cvRandInt(&rng) % RAND_INT_MAX ),
		(cvRandInt(&rng) % RAND_INT_MAX ));
}

// 画直线函数
void draw_line(IplImage * img)
{
	if(g_Line_endpoint.x == -1)
		return;
	CvScalar value = getRandColor();
	cvLine( img, g_Line_startpoint, g_Line_endpoint, value, 2);
}

// 画圆函数
void draw_circle(IplImage * img)
{
	if(g_Circle_radius == -1)
		return;
	CvScalar value = getRandColor();
	cvCircle( img, g_Circle_center, g_Circle_radius, value, 2 );
}

// 画椭圆函数
void draw_ellipse(IplImage* img)
{
	if(g_ellipse_center.x == -1)
		return;
	CvScalar value = getRandColor();
	cvEllipse( img, g_ellipse_center, g_ellipse_axes, 0, 0, 360, value, 2 );
}

// 画多边形函数
void draw_polygon(IplImage * img)
{
	CvScalar value = getRandColor();
	if(iNumofPoints == 0)
		return;

	// 遍历链表，取出point
	myPoints* p;
	int count = 0;
	
	printf("%d\t", iNumofPoints);
	g_polygon_pts[0] = (CvPoint*)malloc( sizeof(CvPoint) * iNumofPoints );
	for(p = pHead; p != NULL; p = p->next, count++)
	{
		g_polygon_pts[0][count] = p->point;
		printf("(%d, %d)  ",  p->point.x, p->point.y);
	}
	printf("\n");
	if(g_polygon_npts == NULL)
	{
		g_polygon_npts = (int*)malloc(sizeof(int));
	}
	g_polygon_npts[0] = iNumofPoints;
	cvPolyLine( img, g_polygon_pts, g_polygon_npts, 1, 0, value, 2 );
}

// 得到两点之间的距离
double getDistanceof2Points(CvPoint pt1, CvPoint pt2)
{
	double d = sqrt( pow( pt1.x - pt2.x, 2 ) + pow( pt1.y - pt2.y, 2 ) );
	return d;
}

// 鼠标事件回调函数
void my_mouse_callback(int event, int x, int y, int flags, void* param )
{
	IplImage * image = (IplImage * )param;

	switch(event)
	{
		// 鼠标左键按下
	case CV_EVENT_LBUTTONDOWN:
		// 不同模式的选择
		switch(g_draw_mode)
		{
			// 画线模式
		case MODE_LINE:
			drawing_line = true;
			g_Line_startpoint = cvPoint( x, y );
			break;
			// 画圆模式
		case MODE_CIRCLE:
			drawing_circle = true;
			g_Circle_center = cvPoint(x, y);
			break;
			// 画椭圆模式
		case MODE_ELLIPSE:
			drawing_ellipse = true;
			g_ellipse_center = cvPoint(x, y);
			break;
			// 画多边形模式
		case MODE_POLYGON:
			drawing_polygon = true;

			// 构造当前点
			myPoints* pNew;
			pNew = (myPoints*)malloc(sizeof(myPoints));
			pNew->point = cvPoint(x, y);
			pNew->next = NULL;
			// 链表为空
			if( pHead == NULL )
			{
				pHead = pNew;
				pTail = pNew;
			}
			// 链表不空
			else
			{
				pTail->next = pNew;
				pTail = pNew;
			}
			iNumofPoints++;
			break;
		}

		// 鼠标移动
	case CV_EVENT_MOUSEMOVE:
		switch(g_draw_mode)
		{
			// 画线模式
		case MODE_LINE:
			if(drawing_line)
			{
				g_Line_endpoint = cvPoint(x, y);
			}
			break;
			// 画圆模式
		case  MODE_CIRCLE:
			if(drawing_circle)
			{
				g_Circle_radius = getDistanceof2Points( 
					g_Circle_center, cvPoint( x, y ) );
			}
			break;			
			// 画椭圆模式
		case MODE_ELLIPSE:
			if(drawing_ellipse)
			{
				g_ellipse_axes = cvSize( 
					abs( x - g_ellipse_center.x ), 
					abs( y - g_ellipse_center.y ) );
			}
			break;
		}
		break;
	case CV_EVENT_LBUTTONUP:
		switch(g_draw_mode)
		{
			// 画线模式
		case MODE_LINE:
			drawing_line = false;
			draw_line( image );
			g_Line_endpoint = cvPoint(-1, -1);
			break;
			// 画圆模式
		case MODE_CIRCLE:
			drawing_circle = false;
			draw_circle( image );
			g_Circle_radius = -1;
			break;
			// 画椭圆模式
		case MODE_ELLIPSE:
			drawing_ellipse = false;
			draw_ellipse( image );
			g_ellipse_center = cvPoint(-1, -1);
			break;
		}
		break;
		// 右键按下，清除所有图像
	case CV_EVENT_RBUTTONDOWN:
		{
			printf("清除所有图像.\n");
			cvZero( image );

			// 画多边形模式，额外的处理
			if(g_draw_mode == MODE_POLYGON)
			{
				g_polygon_pts[0] = NULL;
				iNumofPoints = 0;
				pHead = pTail = NULL;
				g_polygon_npts = NULL;
			}
		}
		break;
	}
}

int main()
{
	// 8位3通道图像
	image = cvCreateImage( cvSize( IMG_WIDTH, IMG_HEIGHT ), 
		IPL_DEPTH_8U, 3 );
	// 置零
	cvZero(image);
	
	temp = cvCloneImage( image );

	// 创建窗口并设置鼠标事件回调函数
	cvNamedWindow("Drawing");
	cvSetMouseCallback("Drawing", my_mouse_callback, (void*) image );

	while(1)
	{
		cvCopyImage( image, temp );

		switch(g_draw_mode)
		{
			// 画线模式
		case MODE_LINE:
			if(drawing_line)
				draw_line( temp );
			break;
			// 画圆模式
		case MODE_CIRCLE:
			if(drawing_circle)
				draw_circle( temp );
			break;
			// 画椭圆模式
		case MODE_ELLIPSE:
			if(drawing_ellipse)
				draw_ellipse( temp );
			break;
			// 画多边形模式
		case MODE_POLYGON:
			if(drawing_polygon)
				draw_polygon( temp );
			break;
		}
		cvShowImage("Drawing", temp);

		// Esc键退出
		// 对于不同的按键，画不同的图形(线、圆、椭圆、多边形)
		int choose = cvWaitKey(15);
		switch(choose)
		{
			// l-108 L-76
		case 76:
		case 108:
			printf("当前处于画线模式.\n");
			g_draw_mode = MODE_LINE;
			break;
			// c-99 C-67
		case 67:
		case 99:
			printf("当前处于画圆模式.\n");
			g_draw_mode = MODE_CIRCLE;
			break;
			// e-101 E-69
		case 69:
		case 101:
			printf("当前处于画椭圆模式.\n");
			g_draw_mode = MODE_ELLIPSE;
			break;
			// p-112 P-80
		case 80:
		case 112:
			printf("当前处于画多边形模式.\n");
			g_draw_mode = MODE_POLYGON;
			break;
			// Esc 退出
		case 27:
			goto end;
		}
	}

	end:
	// 释放资源
	cvReleaseImage( &image );
	cvReleaseImage( &temp );
	cvDestroyAllWindows();

	return 0;
}