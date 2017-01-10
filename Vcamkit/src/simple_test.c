
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include "camkit.h"

#define WIDTH 352
#define HEIGHT 288
//#define WIDTH 640
//#define HEIGHT 480
#define FRAMERATE 15


// set paraments
U32 vfmt = V4L2_PIX_FMT_YUYV;
U32 ofmt = V4L2_PIX_FMT_YUV420;

char *deviceOne = "/dev/video0";
char *deviceTwo = "/dev/video1";

char *oneIp = "192.168.0.101";
char *twoIp = "192.168.0.101";
int onePort = 8888;
int twoPort = 8000;

struct cvt_handle *cvthandle = NULL;
struct tms_handle *tmshandle = NULL;




struct net_handle * getNetParam(char* ip_address,int port)
{
	struct net_handle *nethandle = NULL;
	struct net_param netp;


	netp.type = UDP;
    netp.serip = ip_address;
    netp.serport = port;

    nethandle = net_open(netp);
    if (!nethandle)
    {
        printf("--- Open network failed\n");
    	return NULL;
    }
	return nethandle;
}

struct enc_handle  * getEncodeHandle()
{
	struct enc_param encp;
	encp.src_picwidth = WIDTH;
	encp.src_picheight = HEIGHT;
	encp.enc_picwidth = WIDTH;
	encp.enc_picheight = HEIGHT;
	encp.chroma_interleave = 0;
	encp.fps = FRAMERATE;
	encp.gop = 12;
	encp.bitrate = 800;

	return   encode_open(encp);
}
struct pac_handle * getPackHandle()
{
		struct pac_param pacp;
		pacp.max_pkt_len = 1400;
		pacp.ssrc = 1234;

		return pack_open(pacp);
}


int  initParams()
{
	struct cvt_param cvtp;
	struct tms_param tmsp;


  cvtp.inwidth = WIDTH;
	cvtp.inheight = HEIGHT;
	cvtp.inpixfmt = vfmt;
	cvtp.outwidth = WIDTH;
	cvtp.outheight = HEIGHT;
	cvtp.outpixfmt = ofmt;

  tmsp.startx = 10;
	tmsp.starty = 10;
	tmsp.video_width = WIDTH;
	tmsp.factor = 0;

	cvthandle = convert_open(cvtp);
	if (!cvthandle)
	{
		printf("--- Open convert failed\n");
		return -1;
	}

	tmshandle = timestamp_open(tmsp);
	if(!tmshandle)
	{
		printf("--- Open timestamp failde \n");
		return -1;
	}
	return 0;
}

int openOne()
{
	/* using net handle*/
	struct net_handle *nethandle = NULL;
	/* using capture handle*/
	struct cap_handle *caphandle = NULL;
	/** using encode handle*/
	struct enc_handle *enchandle = NULL;
	/** using pack handle*/
	struct pac_handle *pachandle = NULL;


	struct cap_handle *caphandleOne = NULL; // first captrue
	struct cap_handle *caphandleTwo = NULL; // second captrue

	struct net_handle *nethandleOne = getNetParam(oneIp,onePort); // first ip
	struct net_handle *nethandleTwo = getNetParam(twoIp,twoPort); // second ip

	struct enc_handle *enchandleOne = getEncodeHandle();  // firts encode
	struct enc_handle *enchandleTwo = getEncodeHandle();  // second encode

	struct pac_handle *pachandleOne = getPackHandle();  // first pack
	struct pac_handle *pachandleTwo = getPackHandle();	// second pack


	/*set first captrue handle*/
  struct cap_param cappOne;
  cappOne.dev_name = deviceOne;
	cappOne.width = WIDTH;
	cappOne.height = HEIGHT;
	cappOne.pixfmt = vfmt;
	cappOne.rate = FRAMERATE;

	/*set second captrue handle*/
	struct cap_param cappTwo;
	cappTwo.dev_name = deviceTwo;
	cappTwo.width = WIDTH;
	cappTwo.height = HEIGHT;
	cappTwo.pixfmt = vfmt;
	cappTwo.rate = FRAMERATE;

	caphandleOne = capture_open(cappOne);
	caphandleTwo = capture_open(cappTwo);

	if (!caphandleOne || !caphandleTwo)
	{
		printf("--- Open capture failed\n");
		return -1;
	}


	initParams();


	// start stream loop
	int flag = 1;
	int ret;
	void *cap_buf, *cvt_buf, *hd_buf, *enc_buf, *pac_buf;
	int cap_len, cvt_len, hd_len, enc_len, pac_len;
	enum pic_t ptype;
	struct timeval  ltime;

	capture_start(caphandleOne);		// !!! need to start capture stream!
	capture_start(caphandleTwo);

  gettimeofday(&ltime,NULL);

	caphandle = caphandleOne;
	nethandle = nethandleOne;
	enchandle = enchandleOne;
	pachandle = pachandleOne;
	while (1)
	{
		cap_buf=NULL;
		cvt_buf=NULL;
		hd_buf=NULL;
		enc_buf=NULL;
		pac_buf=NULL;
		cap_len=cvt_len=hd_len=enc_len=pac_len=0;

		ret = capture_get_data(caphandle, &cap_buf, &cap_len);
		if (ret != 0)
		{
			if (ret < 0)		// error
			{
				printf("--- capture_get_data failed\n");
				break;
			}
			else	// again
			{
				usleep(10000);
				continue;
			}
		}
		if (cap_len <= 0)
		{
			printf("!!! No capture data  : %d \n", flag);
			continue;
		}
		// else
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
            		//fwrite(hd_buf, 1, hd_len, dumpfile);
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
		}

		ret = encode_do(enchandle, cvt_buf, cvt_len, &enc_buf, &enc_len,
				&ptype);
		if (ret < 0)
		{
			printf(" --- encode_do failed  %d \n", flag);
			break;
		}
		if (enc_len <= 0)
		{
			printf("!!! No encode data  %d \n", flag);
			continue;
		}
		// else

        //fwrite(enc_buf, 1, enc_len, dumpfile);
		// RTP pack and send

		pack_put(pachandle, enc_buf, enc_len);
		while (pack_get(pachandle, &pac_buf, &pac_len) == 1)
		{
			if(flag)
          			ret = net_send(nethandleOne, pac_buf, pac_len);
			else
          			ret = net_send(nethandleTwo, pac_buf, pac_len);
			if (ret != pac_len)
			{
				printf("send body pack failed %d , size: %d, err: %s\n", flag, pac_len,
						strerror(errno));
			}
		}


		if (flag)
		{
			caphandle = caphandleTwo;
			nethandle = nethandleTwo;
			enchandle = enchandleTwo;
			pachandle = pachandleTwo;
			flag = 0;
		}
		else
		{
			caphandle = caphandleOne;
			nethandle = nethandleOne;
			enchandle = enchandleOne;
			pachandle = pachandleOne;
			flag = 1;
		}
	}

	capture_stop(caphandleOne);
	capture_stop(caphandleTwo);

	pack_close(pachandle);
	encode_close(enchandle);
	convert_close(cvthandle);
	timestamp_close(tmshandle);

	net_close(nethandleOne);
	net_close(nethandleTwo);
	return 0;
}


int main()
{
	openOne();
	return 0;
}

