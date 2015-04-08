#include <highgui.h>
#include <cv.h>
#include <stdio.h>

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
 *《学习OpenCV》第三章第八题a
 * 完成时间：19:12 4/4 星期四  2013
 */

typedef struct my_struct
{
	int i;
	CvPoint point;
	CvRect rect;
} MyStruct;

void write_my_struct(CvFileStorage * fs, const char* name, my_struct*  ms)
{
	//开始写数据
	cvStartWriteStruct(fs, name, 6);

	//写入一个 整数
	cvStartWriteStruct(fs,"integer",CV_NODE_SEQ);
	cvWriteInt(fs,NULL,ms->i);
	cvEndWriteStruct(fs);

	//写入cvpoint结构
	cvStartWriteStruct(fs,"CvPoint",CV_NODE_SEQ);
	cvWriteInt(fs,NULL,ms->point.x);
	cvWriteInt(fs,NULL,ms->point.y);
	cvEndWriteStruct(fs);
    
	//写入rect结构体
	cvStartWriteStruct(fs,"CvRect",CV_NODE_SEQ);
	cvWriteInt(fs,NULL,ms->rect.x);
	cvWriteInt(fs,NULL,ms->rect.y);
	cvWriteInt(fs,NULL,ms->rect.height);
	cvWriteInt(fs,NULL,ms->rect.width);
	cvEndWriteStruct(fs);
     
	//结束写数据
	cvEndWriteStruct(fs);
}

void read_my_struct(CvFileStorage* fs, CvFileNode* ms_node, my_struct* ms)
{
	// 读第一个整数
	// 注意：这里应使用node->data.i的value来读取Integer
	int i = cvGetFileNodeByName(fs, ms_node, "integer")->data.i;
	ms->i = i;

	// 读CvPoint结构
	CvSeq *s1 = cvGetFileNodeByName(fs, ms_node, "CvPoint")->data.seq;
	CvPoint point;
	point.x= cvReadInt((CvFileNode*)cvGetSeqElem(s1,0));
	point.y= cvReadInt((CvFileNode*)cvGetSeqElem(s1,1));
	ms->point = point;

	// 读取CvRect结构
	CvSeq *s2 = cvGetFileNodeByName(fs, ms_node, "CvRect")->data.seq;
	CvRect rect;
	rect.x=cvReadInt((CvFileNode*)cvGetSeqElem(s2, 0));
	rect.y=cvReadInt((CvFileNode*)cvGetSeqElem(s2, 1));
	rect.width=cvReadInt((CvFileNode*)cvGetSeqElem(s2, 3));
	rect.height=cvReadInt((CvFileNode*)cvGetSeqElem(s2, 2));
	ms->rect = rect;
}

// 将MyStruct的值显示出来
void ShowStructValue(MyStruct* pvalue)
{
	printf("integer:%d\n", pvalue->i);
	printf("CvPoint: (%d, %d)\n", pvalue->point.x, pvalue->point.y );
	printf("CvRect: h-->%d\tw-->%d\t(%d, %d)\n", pvalue->rect.height, 
		pvalue->rect.width, pvalue->rect.x, pvalue->rect.y);
}


int main()
{
	/* 写数据部分 */
	MyStruct struct_1; 
	struct_1.i = 10;
	struct_1.point = cvPoint(50, 145);
	struct_1.rect = cvRect(2, 2, 400, 400);

	CvFileStorage* fs = cvOpenFileStorage("My_struct.xml", 0, CV_STORAGE_WRITE);

	write_my_struct(fs, "my_struct", &struct_1);
	cvReleaseFileStorage(&fs);

	/* 读数据部分 */
	fs = cvOpenFileStorage("My_struct.xml", NULL, CV_STORAGE_READ );
	MyStruct struct_2;
 	CvFileNode *pnode = cvGetFileNodeByName(fs, NULL, "my_struct");

	read_my_struct( fs, pnode, &struct_2 );

	// 显示
	printf("---------------- Write -----------------\n");
	ShowStructValue( & struct_1 );
	printf("---------------- Read -------------------\n");
	ShowStructValue( & struct_2);

	cvReleaseFileStorage(&fs);

	return 0;
}