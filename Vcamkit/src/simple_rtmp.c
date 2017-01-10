
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include "camkit.h"
//#define WIDTH 352
//#define HEIGHT 288
#define WIDTH 640
#define HEIGHT 480
#define FRAMERATE 15


unsigned char *data;
int data_size;

FILE *fp_send1;

//读文件的回调函数
//we use this callback function to read data from buffer
int read_buffer1(unsigned char *buf, int buf_size ){
	if(!feof(fp_send1)){
		int true_size=fread(buf,1,buf_size,fp_send1);
		return true_size;
	}else{
		return -1;
	}
}

int read_buffer2(unsigned char * buf,int buf_size)
{
	buf = data;
	buf_size = data_size;
	return 1;
}

void open1()
{
	struct cap_handle *caphandle = NULL;
	struct cvt_handle *cvthandle = NULL;
	struct enc_handle *enchandle = NULL;
	struct tms_handle *tmshandle = NULL;
	
	struct cap_param capp;
	struct cvt_param cvtp;
	struct tms_param tmsp;
	struct enc_param encp;

	// set paraments
	U32 vfmt = V4L2_PIX_FMT_YUYV;
	U32 ofmt = V4L2_PIX_FMT_YUV420;

	capp.dev_name = "/dev/video0";
	capp.width = WIDTH;
	capp.height = HEIGHT;
	capp.pixfmt = vfmt;
	capp.rate = FRAMERATE;

	cvtp.inwidth = WIDTH;
	cvtp.inheight = HEIGHT;
	cvtp.inpixfmt = vfmt;
	cvtp.outwidth = WIDTH;
	cvtp.outheight = HEIGHT;
	cvtp.outpixfmt = ofmt;

	encp.src_picwidth = WIDTH;
	encp.src_picheight = HEIGHT;
	encp.enc_picwidth = WIDTH;
	encp.enc_picheight = HEIGHT;
	encp.chroma_interleave = 0;
	encp.fps = FRAMERATE;
	encp.gop = 30;
	encp.bitrate = 800; 
	
	tmsp.startx = 10;
	tmsp.starty = 10;
	tmsp.video_width = WIDTH;
	tmsp.factor = 0;	

	tmshandle = timestamp_open(tmsp);
	caphandle = capture_open(capp);  //start capture
	 cvthandle = convert_open(cvtp);
	enchandle = encode_open(encp);
	RTMP264_Connect("rtmp://118.89.26.234/myapp/mystream");
	// start stream loop
	int ret;
	void *cap_buf, *cvt_buf, *hd_buf, *enc_buf, *pac_buf;
	int cap_len, cvt_len, hd_len, enc_len, pac_len;
	enum pic_t ptype;
	unsigned long framecount = 0;
capture_start(caphandle);		// !!! need to start capture stream!

	while (1)
	{
//		printf("statr open");
		ret = capture_get_data(caphandle, &cap_buf, &cap_len);
		if (ret != 0) //正常是返回 0 ，就不走这里了
		{
			if (ret < 0)		// error
			{
				printf("--- capture_get_data failed\n");
				break;
			}
			else	// eagain
			{
				usleep(10000);
				continue;
			}
		}
		if (cap_len <= 0)
		{
			printf("!!! No capture data\n");
			continue;
		}
		// else

		//printf("cover open \n");
		ret = convert_do(cvthandle, cap_buf, cap_len, &cvt_buf, &cvt_len);
		if (ret < 0)
		{
			printf("--- convert_do failed\n");
			break;
		}
		if (cvt_len <= 0)
		{
			printf("!!! No convert data\n");
			continue;
		}
		// else

		timestamp_draw(tmshandle,cvt_buf);
		// fetch h264 headers first!
		while ((ret = encode_get_headers(enchandle, &hd_buf, &hd_len, &ptype))!= 0)
		{
            	/*	//fwrite(hd_buf, 1, hd_len, dumpfile);
			pack_put(pachandle, hd_buf, hd_len);
			while (pack_get(pachandle, &pac_buf, &pac_len) == 1)
			{
				if(flag)
               				ret = net_send(nethandleOne, pac_buf, pac_len);
				else
               				ret = net_send(nethandleTwo, pac_buf, pac_len);
				if (ret != pac_len)
				{
					printf("send head pack data failed, size: %d, err: %s\n", pac_len,
							strerror(errno));
				}
			}
		*/
		}




	//	printf("decode open \n");
		ret = encode_do(enchandle, cvt_buf, cvt_len, &enc_buf, &enc_len,
				&ptype);
		if (ret < 0)
		{
			printf("--- encode_do failed\n");
			break;
		}
		if (enc_len <= 0)
		{
			printf("!!! No encode data\n");
			continue;
		}
		printf("send: %d \n", enc_len);
		
//		fp_send1 = fopen("test.h264","ab+");	
		fwrite(enc_buf,1,enc_len,fp_send1);

		printf("send:\n");
		
		//发送
	//	RTMP264_Send(read_buffer1);

	//断开连接并释放相关资源
		fclose(fp_send1);
	}
	capture_stop(caphandle);

	encode_close(enchandle);
	convert_close(cvthandle);
	capture_close(caphandle);
	RTMP264_Close();
}




int main()
{
//	open1();
fp_send1 = fopen("cuc_ieschool.h264", "rb");

	//初始化并连接到服务器
	RTMP264_Connect("rtmp://118.89.26.234/myapp/mystream");
	
	//发送
	RTMP264_Send(read_buffer1);

	//断开连接并释放相关资源
	RTMP264_Close();
	return 0;
}

