
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include "camkit.h"

//#define WIDTH 352
//#define HEIGHT 288
#define WIDTH 352
#define HEIGHT 288
#define FRAMERATE 15

#define CAPTURE_FILE "frame_yuyv_new.jpg"
#define CAPTURE_RGB_FILE "frame_rgb_new.bmp"
#define CAPTURE_show_FILE "a.bmp"



/*
void *calloc(unsigned n,unsigned size) 功 能: 在内存的动态存储区中分配n个长度为size的连续空间，函数返回一个指向分配起始地址的指针；如果分配不成功，返回NULL。跟malloc的区别：calloc在动态分配完内存后，自动初始化该内存空间为零，而malloc不初始化，里边数据是随机的垃圾数据
*/
//位图文件头数据结构含有位图文件的类型，大小和打印格式等信息
//进行数据字节的对齐
#pragma pack(1)
typedef struct BITMAPFILEHEADER
{
  unsigned short bfType;//位图文件的类型,
  unsigned long bfSize;//位图文件的大小，以字节为单位
  unsigned short bfReserved1;//位图文件保留字，必须为0
  unsigned short bfReserved2;//同上
  unsigned long bfOffBits;//位图阵列的起始位置，以相对于位图文件   或者说是头的偏移量表示，以字节为单位
} BITMAPFILEHEADER;
#pragma pack()


typedef struct BITMAPINFOHEADER//位图信息头类型的数据结构，用于说明位图的尺寸
{
  unsigned long biSize;//位图信息头的长度，以字节为单位
  unsigned long biWidth;//位图的宽度，以像素为单位
  unsigned long biHeight;//位图的高度，以像素为单位
  unsigned short biPlanes;//目标设备的级别,必须为1
  unsigned short biBitCount;//每个像素所需的位数，必须是1(单色),4(16色),8(256色)或24(2^24色)之一
  unsigned long biCompression;//位图的压缩类型，必须是0-不压缩，1-BI_RLE8压缩类型或2-BI_RLE4压缩类型之一
  unsigned long biSizeImage;//位图大小，以字节为单位
  unsigned long biXPelsPerMeter;//位图目标设备水平分辨率，以每米像素数为单位
  unsigned long biYPelsPerMeter;//位图目标设备垂直分辨率，以每米像素数为单位
  unsigned long biClrUsed;//位图实际使用的颜色表中的颜色变址数
  unsigned long biClrImportant;//位图显示过程中被认为重要颜色的变址数
} BITMAPINFOHEADER;




// set paraments
U32 vfmt = V4L2_PIX_FMT_YUYV;
U32 ofmt = V4L2_PIX_FMT_YUV420;

char *deviceOne = "/dev/video0";
char *deviceTwo = "/dev/video1";


/////////////////////////////////////////////////////////////////////////////


struct BITMAPFILEHEADER bfh;
struct BITMAPINFOHEADER bih;

void create_bmp_header()
{
  bfh.bfType = (unsigned short)0x4D42;
  bfh.bfSize = (unsigned long)(14 + 40 + WIDTH * HEIGHT*3);
  bfh.bfReserved1 = 0;
  bfh.bfReserved2 = 0;
  bfh.bfOffBits = (unsigned long)(14 + 40);


  bih.biBitCount = 24;
  bih.biWidth = WIDTH;
  bih.biHeight = HEIGHT;
  bih.biSizeImage = WIDTH * HEIGHT * 3;
  bih.biClrImportant = 0;
  bih.biClrUsed = 0;
  bih.biCompression = 0;
  bih.biPlanes = 1;
  bih.biSize = 40;//sizeof(bih);
  bih.biXPelsPerMeter = 0x00000ec4;
  bih.biYPelsPerMeter = 0x00000ec4;
}



void store_yuyv(void **buf, int len)
{
    FILE *fp = fopen(CAPTURE_FILE, "wb");
    if (fp < 0) {
        printf("open frame data file failed\n");
        return;
    }
    fwrite(*buf, 1, len, fp);
    fclose(fp);
    printf("Capture one frame saved in %s\n", CAPTURE_FILE);
    return;
}


void yuyv2rgb(unsigned char *newBuf,unsigned char *starter, int len)
{
    unsigned char YUYV[4],RGB[6];
    int j,k,i;
    unsigned int location=0;
    j=0;
    for(i=0;i < len ;i+=4)
    {
        YUYV[0]=starter[i];//Y0
        YUYV[1]=starter[i+1];//U
        YUYV[2]=starter[i+2];//Y1
        YUYV[3]=starter[i+3];//V
        if(YUYV[0]<1)
        {
            RGB[0]=0;
            RGB[1]=0;
            RGB[2]=0;
        }
        else
        {
            RGB[0]=YUYV[0]+1.772*(YUYV[1]-128);//b
            RGB[1]=YUYV[0]-0.34413*(YUYV[1]-128)-0.71414*(YUYV[3]-128);//g
            RGB[2]=YUYV[0]+1.402*(YUYV[3]-128);//r
        }
        if(YUYV[2]<0)
        {
            RGB[3]=0;
            RGB[4]=0;
            RGB[5]=0;
        }
        else
        {
            RGB[3]=YUYV[2]+1.772*(YUYV[1]-128);//b
            RGB[4]=YUYV[2]-0.34413*(YUYV[1]-128)-0.71414*(YUYV[3]-128);//g
            RGB[5]=YUYV[2]+1.402*(YUYV[3]-128);//r


        }


        for(k=0;k<6;k++)
        {
            if(RGB[k]<0)
                RGB[k]=0;
            if(RGB[k]>255)
                RGB[k]=255;
        }


        //请记住：扫描行在位图文件中是反向存储的！
        if(j%(WIDTH*3)==0)//定位存储位置
        {
            location=(HEIGHT-j/(WIDTH*3))*(WIDTH*3);
        }
        bcopy(RGB,newBuf+location+(j%(WIDTH*3)),sizeof(RGB));


        j+=6;
    }
    return;
}


void move_noise(unsigned char *newBuf)
{//双滤波器
    int i,j,k,temp[3],temp1[3];
    unsigned char BGR[13*3];
    unsigned int sq,sq1,loc,loc1;
    int h=HEIGHT,w=WIDTH;
    for(i=2;i<h-2;i++)
    {
        for(j=2;j<w-2;j++)
        {
            memcpy(BGR,newBuf+(i-1)*w*3+3*(j-1),9);
            memcpy(BGR+9,newBuf+i*w*3+3*(j-1),9);
            memcpy(BGR+18,newBuf+(i+1)*w*3+3*(j-1),9);
            memcpy(BGR+27,newBuf+(i-2)*w*3+3*j,3);
            memcpy(BGR+30,newBuf+(i+2)*w*3+3*j,3);
            memcpy(BGR+33,newBuf+i*w*3+3*(j-2),3);
            memcpy(BGR+36,newBuf+i*w*3+3*(j+2),3);


            memset(temp,0,4*3);

            for(k=0;k<9;k++)
            {
                temp[0]+=BGR[k*3];
                temp[1]+=BGR[k*3+1];
                temp[2]+=BGR[k*3+2];
            }
            temp1[0]=temp[0];
            temp1[1]=temp[1];
            temp1[2]=temp[2];
            for(k=9;k<13;k++)
            {
                temp1[0]+=BGR[k*3];
                temp1[1]+=BGR[k*3+1];
                temp1[2]+=BGR[k*3+2];
            }
            for(k=0;k<3;k++)
            {
                temp[k]/=9;
                temp1[k]/=13;
            }
            sq=0xffffffff;loc=0;
            sq1=0xffffffff;loc1=0;
            unsigned int a;
            for(k=0;k<9;k++)
            {
                a=abs(temp[0]-BGR[k*3])+abs(temp[1]-BGR[k*3+1])+abs(temp[2]-BGR[k*3+2]);
                if(a<sq)
                {
                    sq=a;
                    loc=k;
                }
            }
            for(k=0;k<13;k++)
            {
                a=abs(temp1[0]-BGR[k*3])+abs(temp1[1]-BGR[k*3+1])+abs(temp1[2]-BGR[k*3+2]);
                if(a<sq1)
                {
                    sq1=a;
                    loc1=k;
                }
            }

            newBuf[i*w*3+3*j]=(unsigned char)((BGR[3*loc]+BGR[3*loc1])/2);
            newBuf[i*w*3+3*j+1]=(unsigned char)((BGR[3*loc+1]+BGR[3*loc1+1])/2);
            newBuf[i*w*3+3*j+2]=(unsigned char)((BGR[3*loc+2]+BGR[3*loc1+2])/2);
        }
    }
    return;
}




void store_bmp(unsigned char *newBuf,int n_len,char *filename)
{
    FILE *fp1 = fopen(filename, "wb");
    if (fp1 < 0) {
        printf("open frame data file failed\n");
        return;
    }
    fwrite(&bfh,sizeof(bfh),1,fp1);
    fwrite(&bih,sizeof(bih),1,fp1);
    fwrite(newBuf, 1, n_len, fp1);
    fclose(fp1);
    printf("Change one frame saved in %s\n", filename);
    return;
}

void capture_bmp(void *cap_buf,int cap_len,char *filename){
    int n_len;
    unsigned char *newBuf;
    n_len = cap_len*3/2;
    newBuf=calloc((unsigned int)n_len,sizeof(unsigned char));
    yuyv2rgb(newBuf,cap_buf,cap_len);//还是这个采集的图片的效果比较好
    move_noise(newBuf);
    create_bmp_header();
    store_bmp(newBuf,n_len,filename);
}

int openOne()
{

	/* using capture handle*/
	struct cap_handle *caphandle = NULL;

	struct cap_handle *caphandleOne = NULL; // first captrue


	struct cap_param cappOne;
    cappOne.dev_name = deviceOne;
	cappOne.width = WIDTH;
	cappOne.height = HEIGHT;
	cappOne.pixfmt = vfmt;
	cappOne.rate = FRAMERATE;

	caphandleOne = capture_open(cappOne);


	void *cap_buf;
	int cap_len;

	capture_start(caphandleOne);		// !!! need to start capture stream!

	caphandle = caphandleOne;

	cap_buf=NULL;
	cap_len=0;

	capture_get_data(caphandle, &cap_buf, &cap_len);
 	store_yuyv(&cap_buf, cap_len);
    capture_bmp(cap_buf,cap_len,CAPTURE_RGB_FILE);
	capture_stop(caphandleOne);

	return 0;
}




int main()
{
	openOne();
	return 0;
}
