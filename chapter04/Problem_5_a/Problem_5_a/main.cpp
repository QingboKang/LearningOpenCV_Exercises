#include "opencv_libs.h"
#include <highgui.h>
#include <cv.h>
#include <math.h>


/*
 *��ѧϰOpenCV�������µ�����a
 * ���ʱ�䣺8:08 4/5 ������ 2013
 */

#define IMG_WIDTH 512    // ͼ���
#define IMG_HEIGHT 512   // ͼ���

#define RAND_INT_MAX 256

// ��ͬģʽ�Ķ���
#define MODE_LINE     1     // ����
#define MODE_CIRCLE   2     // ��Բ
#define MODE_ELLIPSE  3     // ����Բ
#define MODE_POLYGON  4     // �������

// ��ǰģʽ
int g_draw_mode = 0;

typedef struct MyPoints
{
	CvPoint point;
	struct MyPoints* next;
} myPoints;

IplImage * image;
IplImage * temp;

// �����������
CvRNG rng = cvRNG(cvGetTickCount()); 

// �Ƿ��ڻ���
bool drawing_line = false;
// ����--��㡢�յ�
CvPoint g_Line_startpoint = cvPoint(-1, -1);
CvPoint g_Line_endpoint = cvPoint(-1, -1);

// �Ƿ��ڻ�Բ
bool drawing_circle = false;
// ��Բ--Բ�ġ��뾶
CvPoint g_Circle_center = cvPoint(-1, -1);
int g_Circle_radius = -1;

// �Ƿ��ڻ���Բ
bool drawing_ellipse = false;
// ����Բ--Բ�ġ����̰���
CvPoint g_ellipse_center = cvPoint(-1, -1);
CvSize g_ellipse_axes = cvSize( -1, -1 );

// �Ƿ��ڻ������
bool drawing_polygon = false;
// �������--���е������
CvPoint** g_polygon_pts = (CvPoint**)malloc(sizeof(CvPoint*));
int* g_polygon_npts;
myPoints* pHead, *pTail, *pCurr;
int iNumofPoints = 0;

// ���������ɫ
CvScalar getRandColor()
{
	return cvScalar( (cvRandInt(&rng) % RAND_INT_MAX),
		(cvRandInt(&rng) % RAND_INT_MAX ),
		(cvRandInt(&rng) % RAND_INT_MAX ));
}

// ��ֱ�ߺ���
void draw_line(IplImage * img)
{
	if(g_Line_endpoint.x == -1)
		return;
	CvScalar value = getRandColor();
	cvLine( img, g_Line_startpoint, g_Line_endpoint, value, 2);
}

// ��Բ����
void draw_circle(IplImage * img)
{
	if(g_Circle_radius == -1)
		return;
	CvScalar value = getRandColor();
	cvCircle( img, g_Circle_center, g_Circle_radius, value, 2 );
}

// ����Բ����
void draw_ellipse(IplImage* img)
{
	if(g_ellipse_center.x == -1)
		return;
	CvScalar value = getRandColor();
	cvEllipse( img, g_ellipse_center, g_ellipse_axes, 0, 0, 360, value, 2 );
}

// ������κ���
void draw_polygon(IplImage * img)
{
	CvScalar value = getRandColor();
	if(iNumofPoints == 0)
		return;

	// ��������ȡ��point
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

// �õ�����֮��ľ���
double getDistanceof2Points(CvPoint pt1, CvPoint pt2)
{
	double d = sqrt( pow( pt1.x - pt2.x, 2 ) + pow( pt1.y - pt2.y, 2 ) );
	return d;
}

// ����¼��ص�����
void my_mouse_callback(int event, int x, int y, int flags, void* param )
{
	IplImage * image = (IplImage * )param;

	switch(event)
	{
		// ����������
	case CV_EVENT_LBUTTONDOWN:
		// ��ͬģʽ��ѡ��
		switch(g_draw_mode)
		{
			// ����ģʽ
		case MODE_LINE:
			drawing_line = true;
			g_Line_startpoint = cvPoint( x, y );
			break;
			// ��Բģʽ
		case MODE_CIRCLE:
			drawing_circle = true;
			g_Circle_center = cvPoint(x, y);
			break;
			// ����Բģʽ
		case MODE_ELLIPSE:
			drawing_ellipse = true;
			g_ellipse_center = cvPoint(x, y);
			break;
			// �������ģʽ
		case MODE_POLYGON:
			drawing_polygon = true;

			// ���쵱ǰ��
			myPoints* pNew;
			pNew = (myPoints*)malloc(sizeof(myPoints));
			pNew->point = cvPoint(x, y);
			pNew->next = NULL;
			// ����Ϊ��
			if( pHead == NULL )
			{
				pHead = pNew;
				pTail = pNew;
			}
			// ������
			else
			{
				pTail->next = pNew;
				pTail = pNew;
			}
			iNumofPoints++;
			break;
		}

		// ����ƶ�
	case CV_EVENT_MOUSEMOVE:
		switch(g_draw_mode)
		{
			// ����ģʽ
		case MODE_LINE:
			if(drawing_line)
			{
				g_Line_endpoint = cvPoint(x, y);
			}
			break;
			// ��Բģʽ
		case  MODE_CIRCLE:
			if(drawing_circle)
			{
				g_Circle_radius = getDistanceof2Points( 
					g_Circle_center, cvPoint( x, y ) );
			}
			break;			
			// ����Բģʽ
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
			// ����ģʽ
		case MODE_LINE:
			drawing_line = false;
			draw_line( image );
			g_Line_endpoint = cvPoint(-1, -1);
			break;
			// ��Բģʽ
		case MODE_CIRCLE:
			drawing_circle = false;
			draw_circle( image );
			g_Circle_radius = -1;
			break;
			// ����Բģʽ
		case MODE_ELLIPSE:
			drawing_ellipse = false;
			draw_ellipse( image );
			g_ellipse_center = cvPoint(-1, -1);
			break;
		}
		break;
		// �Ҽ����£��������ͼ��
	case CV_EVENT_RBUTTONDOWN:
		{
			printf("�������ͼ��.\n");
			cvZero( image );

			// �������ģʽ������Ĵ���
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
	// 8λ3ͨ��ͼ��
	image = cvCreateImage( cvSize( IMG_WIDTH, IMG_HEIGHT ), 
		IPL_DEPTH_8U, 3 );
	// ����
	cvZero(image);
	
	temp = cvCloneImage( image );

	// �������ڲ���������¼��ص�����
	cvNamedWindow("Drawing");
	cvSetMouseCallback("Drawing", my_mouse_callback, (void*) image );

	while(1)
	{
		cvCopyImage( image, temp );

		switch(g_draw_mode)
		{
			// ����ģʽ
		case MODE_LINE:
			if(drawing_line)
				draw_line( temp );
			break;
			// ��Բģʽ
		case MODE_CIRCLE:
			if(drawing_circle)
				draw_circle( temp );
			break;
			// ����Բģʽ
		case MODE_ELLIPSE:
			if(drawing_ellipse)
				draw_ellipse( temp );
			break;
			// �������ģʽ
		case MODE_POLYGON:
			if(drawing_polygon)
				draw_polygon( temp );
			break;
		}
		cvShowImage("Drawing", temp);

		// Esc���˳�
		// ���ڲ�ͬ�İ���������ͬ��ͼ��(�ߡ�Բ����Բ�������)
		int choose = cvWaitKey(15);
		switch(choose)
		{
			// l-108 L-76
		case 76:
		case 108:
			printf("��ǰ���ڻ���ģʽ.\n");
			g_draw_mode = MODE_LINE;
			break;
			// c-99 C-67
		case 67:
		case 99:
			printf("��ǰ���ڻ�Բģʽ.\n");
			g_draw_mode = MODE_CIRCLE;
			break;
			// e-101 E-69
		case 69:
		case 101:
			printf("��ǰ���ڻ���Բģʽ.\n");
			g_draw_mode = MODE_ELLIPSE;
			break;
			// p-112 P-80
		case 80:
		case 112:
			printf("��ǰ���ڻ������ģʽ.\n");
			g_draw_mode = MODE_POLYGON;
			break;
			// Esc �˳�
		case 27:
			goto end;
		}
	}

	end:
	// �ͷ���Դ
	cvReleaseImage( &image );
	cvReleaseImage( &temp );
	cvDestroyAllWindows();

	return 0;
}