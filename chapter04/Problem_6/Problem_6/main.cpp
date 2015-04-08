#include <highgui.h>
#include "opencv_libs.h"
#include <cv.h>  

#define IMG_WIDTH  512
#define IMG_HEIGHT 512

/* 
 *��ѧϰOpenCV�������µ����� 
 * ���ʱ�䣺9:32 4/5 ������ 2013 
 */  

CvFont font;   
char *display_text = "|";
  
// �Ƿ��ڱ༭��ǩ
bool isEditLabel = false;
// ��ǰ��ǩ����ʼ��
CvPoint point; 

IplImage* image;

/* 
 * ����¼��ص����� 
 */  
void my_mouse_callback(int event, int x, int y, int flags, void* param)  
{  
    // �õ�ͼ��  
    IplImage* image = (IplImage*) param;  
 
    switch(event)  
    {  
        // ����������¼�  
    case CV_EVENT_LBUTTONDOWN: 
		isEditLabel = true;
		point = cvPoint(x, y);
		cvPutText( image, display_text, point, &font, cvScalarAll(255));  
        break;  
		// ����һ��¼�
	case CV_EVENT_RBUTTONDOWN:
		printf("ͼ������.\n");
		// ������������
	    cvZero( image );
		break;
    }  
}  
  
int main()  
{  
	// ����һ��ͼ��
	image = cvCreateImage( cvSize(IMG_WIDTH, IMG_HEIGHT), 
		IPL_DEPTH_8U, 3 );
	// ������������
	cvZero( image );

	IplImage* temp = cvCloneImage( image );

    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.5f, 1.5f, 0, 1, 8);  
      
    cvNamedWindow("image");  
    cvSetMouseCallback( "image", my_mouse_callback, (void*)temp );  
  
    while(1)  
    {   
		cvCopyImage( image, temp);
		if(isEditLabel == true)
		{
			puts(display_text);
			cvPutText( temp, display_text, point, &font, cvScalarAll(255));  
		}
        cvShowImage("image", temp);  

		char c = cvWaitKey(15);
		
		// 32 -- 126 ����ʾ�ַ�
		if(isEditLabel == true && c < 126 && c > 32)
		{
			char tempstr[2] = {c};
			if(display_text == "|")
			{
				display_text = (char*)malloc(sizeof(char)*2);
				strcpy( display_text, tempstr );
			}
			else
			{
				strcat( display_text, tempstr );
			}
		}
		// 13 -- Enter��
		else if(c == 13)
		{
			isEditLabel = false;
			cvPutText( image, display_text, point, &font, cvScalarAll(255)); 
			display_text = "|";
		}
		// 8 -- �˸��
		else if(c == 8)
		{
			int len = strlen(display_text);
			printf("<--\t%d\n", len);
			display_text[len - 1] = '\0';
		}
        else if(c == 27) break;  
    }  
  
    cvReleaseImage( &image );
	cvReleaseImage( &temp );
    cvDestroyAllWindows();  
    return 0;  
}  