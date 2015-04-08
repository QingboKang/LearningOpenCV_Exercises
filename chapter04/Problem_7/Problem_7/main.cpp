#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv_libs.h>

/*
 *《学习OpenCV》第四章第七题  
 * 完成时间：0:23 10/4 星期五 2013  
 */    

#define   WARPSTEPSIZE     0.01      // 透视变换矩阵每次的变化率

#define   RESIZESTEPSIZE   0.1       // 放大、缩小每次的变化率

#define   ROTATESTEPSIZE   10        // 旋转图像时每次旋转的角度（逆时针）

// 透视变换目标变换矩阵
CvPoint2D32f g_dstQuad[4];

// image width & height
int g_width = 0;
int g_height = 0;

// rotate degree
int g_RotateDegree = 0;  

/*
 * function: Zoom in or zoom out an image.
 * param: inorout -- indicate in or out(0 means zoom in; 1 means zoom out)
 * param: the destination size.
 */
CvSize getZoomDstSize(int inorout)
{
    if(inorout != 0 && inorout != 1)
    {
        return cvSize(-1, -1);
    }
    if(g_height == 0 || g_width == 0)
    {
        return cvSize(-1, -1);
    }

    // Zoom in
    if(inorout == 0)
    {
        g_width += g_width * RESIZESTEPSIZE;
        g_height += g_height * RESIZESTEPSIZE;
    }
    else if(inorout == 1)
    {
        g_width -= g_width * RESIZESTEPSIZE;
        g_height -= g_height * RESIZESTEPSIZE;

        if(g_height < 1)
        {
            g_height = 1;
        }
        else if(g_width < 1)
        {
            g_width = 1;
        }
    }

    return cvSize(g_width, g_height);
}

void rotateImage(IplImage* img, IplImage *img_rotate,float degree)  
{  
    // 旋转中心
    CvPoint2D32f center;    
    center.x=float (img->width/2.0+0.5);  
    center.y=float (img->height/2.0+0.5);  
    // 计算二维旋转的仿射变换矩阵  
    float m[6];              
    CvMat M = cvMat( 2, 3, CV_32F, m );  
    cv2DRotationMatrix( center, degree,1, &M);  
    // 变换图像，并用黑色填充其余值  
    cvWarpAffine(img,img_rotate, &M,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,cvScalarAll(0) );  
} 

/*
 * function: change the g_dstQuad(minus or plus one of the element)
 * param: index -- which element(0 -- 4)
 * param: xory -- indicate x or y to change(0 means x; 1 means y)
 * param: operation -- indicate which operation(0 means plus; 1 means minus) 
 * return: 0 means success.  -1 means failed.
 */
int changeg_dstQuad(int index, int xory, int operation)
{
    // param check
    if( index > 3 || index < 0 || 
        (xory != 0 && xory != 1) ||
        (operation != 0 && operation != 1) ||
        g_width == 0 || g_height == 0)
    {
        return -1;
    }

    // plus
    if(operation == 0)
    {
        if(xory == 0)    // x
        {
            g_dstQuad[index].x += WARPSTEPSIZE * g_width;
            if(g_dstQuad[index].x > g_width - 1)
            {
                g_dstQuad[index].x = g_width - 1;
            }
        }
        else if(xory == 1)   // y
        {
            g_dstQuad[index].y += WARPSTEPSIZE * g_height;
            if(g_dstQuad[index].y > g_height - 1)
            {
                g_dstQuad[index].y = g_height -1;
            }
        }
    }
    
    // minus
    else if (operation == 1)
    {
        if(xory == 0)        // x
        {
            g_dstQuad[index].x -= WARPSTEPSIZE * g_width;
            if(g_dstQuad[index].x < 0)
            {
                g_dstQuad[index].x = 0;
            }
        }
        else if(xory == 1 )   // y
        {
            g_dstQuad[index].y -= WARPSTEPSIZE * g_height;
            if(g_dstQuad[index].y < 0)
            {
                g_dstQuad[index].y = 0;
            }
        }
    }

    return 0;
}

int main()
{
    const char * FILEPATH = "lena.bmp";
    IplImage * src = cvLoadImage(FILEPATH, CV_LOAD_IMAGE_UNCHANGED);

    if(!src)
    {
        printf("load image error.\texit\n");
        return -1;
    }

    CvPoint2D32f srcQuad[4];
    CvMat* warp_matrix = cvCreateMat(3, 3, CV_32FC1);
    IplImage *dst;

    cvNamedWindow("Source_Image", 1);
    cvNamedWindow("Perspective_Warp", 1);

    dst = cvCloneImage(src);
    dst->origin = dst->origin;
    cvZero(dst);

    // image width & height
    g_width = src->width;
    g_height = src->height;

    srcQuad[0].x = 0;               // src Top left
    srcQuad[0].y = 0;
    srcQuad[1].x = g_width - 1;   // src Top right
    srcQuad[1].y = 0;
    srcQuad[2].x = 0;                // src Bottom left
    srcQuad[2].y = g_height - 1;  
    srcQuad[3].x = g_width - 1;     // src Bottom right
    srcQuad[3].y = g_height - 1;

    g_dstQuad[0].x = 0;           // dst Top left
    g_dstQuad[0].y = 0;
    g_dstQuad[1].x = g_width - 1;   // dst Top right
    g_dstQuad[1].y = 0;
    g_dstQuad[2].x = 0;           // dst Bottom left
    g_dstQuad[2].y = g_height - 1;
    g_dstQuad[3].x = g_width - 1;   // dst Bottom right
    g_dstQuad[3].y = g_height - 1;

    while(1)
    {
        cvShowImage("Source_Image", src);
        char c = cvWaitKey(10);
       
        int ret = 0;
        switch(c)
        {
        // ESC
        case 27:
            goto end;
            break;
        // 0 -- 放大图像
        case 48:
            {
                cvNamedWindow("Resize", 1);
                CvSize dstSize = getZoomDstSize(0);
                if(dstSize.height == -1 || dstSize.width == -1)
                {
                    goto end;
                }

                IplImage* imageresize = cvCreateImage(dstSize, src->depth, src->nChannels);
                if(!imageresize)
                {
                    goto end;
                }
                cvResize(src, imageresize, CV_INTER_LINEAR);
                cvShowImage("Resize", imageresize);
                cvReleaseImage(&imageresize);
            }
            break;
        // SHIFT + 0 ')' -- 缩小图像
        case 41:
            {
                cvNamedWindow("Resize", 1);
                CvSize dstSize = getZoomDstSize(1);
                if(dstSize.height == -1 || dstSize.width == -1)
                {
                    goto end;
                }

                IplImage* imageresize = cvCreateImage(dstSize, src->depth, src->nChannels);
                if(!imageresize)
                {
                    goto end;
                }
                cvResize(src, imageresize, CV_INTER_LINEAR);
                cvShowImage("Resize", imageresize);
                cvReleaseImage(&imageresize);
            }
            break;
        // 9 -- 旋转图像（逆时针）
        case 57:
            {
                cvNamedWindow("Rotate", 1);
                
                IplImage* imagerotate = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
                if(!imagerotate)
                {
                    goto end;
                }
                g_RotateDegree += ROTATESTEPSIZE;
                rotateImage(src, imagerotate, g_RotateDegree);
                cvShowImage("Rotate", imagerotate);
                cvReleaseImage(&imagerotate);
            }
            break;
        // SHIFT + 9 '(' -- 旋转图像（顺时针）
        case 40:
            {
                cvNamedWindow("Rotate", 1);

                IplImage* imagerotate = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
                if(!imagerotate)
                {
                    goto end;
                }
                g_RotateDegree -= ROTATESTEPSIZE;
                rotateImage(src, imagerotate, g_RotateDegree);
                cvShowImage("Rotate", imagerotate);
                cvReleaseImage(&imagerotate);
            }
            break;
        // 1
        case 49:
            ret = changeg_dstQuad(0, 0, 0);
            break;
        // 2
        case 50:
            ret = changeg_dstQuad(0, 1, 0);
            break;
        // 3
        case 51:
            ret = changeg_dstQuad(1, 0, 0);
            break;
        // 4
        case 52:
            ret = changeg_dstQuad(1, 1, 0);
            break;
        // 5
        case 53:
            ret = changeg_dstQuad(2, 0, 0);
            break;
        // 6
        case 54:
            ret = changeg_dstQuad(2, 1, 0);
            break;
        // 7
        case 55:
            ret = changeg_dstQuad(3, 0, 0);
            break;
        // 8
        case 56:
            ret = changeg_dstQuad(3, 1, 0);
            break;
        // SHIFT + 1  '!'
        case 33:
            ret = changeg_dstQuad(0, 0, 1);
            break;
        // SHIFT + 2  '@'
        case 64:
            ret = changeg_dstQuad(0, 1, 1);
            break;
        // SHIFT + 3 '#'
        case 35:
            ret = changeg_dstQuad(1, 0, 1);
            break;
        // SHIFT + 4 '$'
        case 36:
            ret = changeg_dstQuad(1, 1, 1);
            break;
        // SHIFT + 5 '%'
        case 37:
            ret = changeg_dstQuad(2, 0, 1);
            break;
        // SHIFT + 6 '^'
        case 94:
            ret = changeg_dstQuad(2, 1, 1);
            break;
        // SHIFT + 7 '&'
        case 38:
            ret = changeg_dstQuad(3, 0, 1);
            break;
        // SHIFT + 8 '*'
        case 42:
            ret = changeg_dstQuad(3, 1, 1);
            break;

        default:
            break;
        }

        // Error
        if(ret != 0)
        {
            goto end;
        }
        cvGetPerspectiveTransform(srcQuad, g_dstQuad, warp_matrix);
        cvWarpPerspective( src, dst, warp_matrix, 
                           CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

        cvShowImage("Source_Image", src);
        cvShowImage("Perspective_Warp", dst);
    }

end:
    cvReleaseImage(&src);
    cvReleaseImage(&dst);
    cvReleaseMat(&warp_matrix);
    cvDestroyAllWindows();

    return 0;
}