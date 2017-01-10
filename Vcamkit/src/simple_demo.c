/*
 * Copyright (c) 2014 Andy Huang <andyspider@126.com>
 *
 * This file is part of Camkit.
 *
 * Camkit is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Camkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Camkit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

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
#define FRAMERATE 10

void openOne()
{

	struct cap_handle *caphandle = NULL;
	struct cvt_handle *cvthandle = NULL;
	struct enc_handle *enchandle = NULL;
	struct pac_handle *pachandle = NULL;
        struct net_handle *nethandle = NULL;
	struct tms_handle *tmshandle = NULL;

	struct cap_param capp;
	struct cvt_param cvtp;
	struct enc_param encp;
	struct pac_param pacp;
        struct net_param netp;
	struct tms_param tmsp;	

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
	encp.gop = 12;
	encp.bitrate = 1000;

	pacp.max_pkt_len = 1400;
	pacp.ssrc = 1234;

        netp.type = UDP;
        netp.serip = "192.168.199.219";
        netp.serport = 8888;

        tmsp.startx = 10;
	tmsp.starty = 10;
	tmsp.video_width = WIDTH;
	tmsp.factor = 0;

	caphandle = capture_open(capp);
	if (!caphandle)
	{
		printf("--- Open capture failed\n");
		return -1;
	}

	cvthandle = convert_open(cvtp);
	if (!cvthandle)
	{
		printf("--- Open convert failed\n");
		return -1;
	}

	enchandle = encode_open(encp);
	if (!enchandle)
	{
		printf("--- Open encode failed\n");
		return -1;
	}

	pachandle = pack_open(pacp);
	if (!pachandle)
	{
		printf("--- Open pack failed\n");
		return -1;
	}

        nethandle = net_open(netp);
        if (!nethandle)
        {
                printf("--- Open network failed\n");
      		return -1;
        }
	tmshandle = timestamp_open(tmsp);

	// start stream loop
	int ret;
	void *cap_buf, *cvt_buf, *hd_buf, *enc_buf, *pac_buf;
	int cap_len, cvt_len, hd_len, enc_len, pac_len;
	enum pic_t ptype;
	
	struct timeval ctime , ltime;
	unsigned long fps_counter = 0;
	int sec,usec;
	double start_time = 0;
	

	unsigned long framecount = 0;

	capture_start(caphandle);		// !!! need to start capture stream!
	
	gettimeofday(&ltime,NULL);

	while (1)
	{
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
			printf("!!! No capture data\n");
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
               			ret = net_send(nethandle, pac_buf, pac_len);
				if (ret != pac_len)
				{
					printf("send  one pack data failed, size: %d, err: %s\n", pac_len,
							strerror(errno));
				}
			}
		}

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
		// else

        	//fwrite(enc_buf, 1, enc_len, dumpfile);
		// RTP pack and send

		pack_put(pachandle, enc_buf, enc_len);
		while (pack_get(pachandle, &pac_buf, &pac_len) == 1)
		{
          		ret = net_send(nethandle, pac_buf, pac_len);
			if (ret != pac_len)
			{
				printf("send one pack failed, size: %d, err: %s\n", pac_len,
						strerror(errno));
			}
		}

		framecount++;
	}

	capture_stop(caphandle);
        net_close(nethandle);
	pack_close(pachandle);
	encode_close(enchandle);
	convert_close(cvthandle);
	timestamp_close(tmshandle);

}

void openTwo()
{

	struct cap_handle *caphandle = NULL;
	struct cvt_handle *cvthandle = NULL;
	struct enc_handle *enchandle = NULL;
	struct pac_handle *pachandle = NULL;
        struct net_handle *nethandle = NULL;
	struct tms_handle *tmshandle = NULL;

	struct cap_param capp;
	struct cvt_param cvtp;
	struct enc_param encp;
	struct pac_param pacp;
        struct net_param netp;
	struct tms_param tmsp;	

	// set paraments
	U32 vfmt = V4L2_PIX_FMT_YUYV;
	U32 ofmt = V4L2_PIX_FMT_YUV420;

	capp.dev_name = "/dev/video1";
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
	encp.gop = 12;
	encp.bitrate = 1000;

	pacp.max_pkt_len = 1400;
	pacp.ssrc = 1234;

        netp.type = UDP;
        netp.serip = "192.168.199.219";
        netp.serport = 8000;

        tmsp.startx = 10;
	tmsp.starty = 10;
	tmsp.video_width = WIDTH;
	tmsp.factor = 0;

	caphandle = capture_open(capp);
	if (!caphandle)
	{
		printf("--- Open capture failed\n");
		return -1;
	}

	cvthandle = convert_open(cvtp);
	if (!cvthandle)
	{
		printf("--- Open convert failed\n");
		return -1;
	}

	enchandle = encode_open(encp);
	if (!enchandle)
	{
		printf("--- Open encode failed\n");
		return -1;
	}

	pachandle = pack_open(pacp);
	if (!pachandle)
	{
		printf("--- Open pack failed\n");
		return -1;
	}

        nethandle = net_open(netp);
        if (!nethandle)
        {
                printf("--- Open network failed\n");
      		return -1;
        }
	tmshandle = timestamp_open(tmsp);

	// start stream loop
	int ret;
	void *cap_buf, *cvt_buf, *hd_buf, *enc_buf, *pac_buf;
	int cap_len, cvt_len, hd_len, enc_len, pac_len;
	enum pic_t ptype;
	
	struct timeval ctime , ltime;
	unsigned long fps_counter = 0;
	int sec,usec;
	double start_time = 0;
	

	unsigned long framecount = 0;

	capture_start(caphandle);		// !!! need to start capture stream!
	
	gettimeofday(&ltime,NULL);

	while (1)
	{
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
			printf("!!! No capture data\n");
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
               			ret = net_send(nethandle, pac_buf, pac_len);
				if (ret != pac_len)
				{
					printf("send two pack data failed, size: %d, err: %s\n", pac_len,
							strerror(errno));
				}
			}
		}

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
		// else

        	//fwrite(enc_buf, 1, enc_len, dumpfile);
		// RTP pack and send

		pack_put(pachandle, enc_buf, enc_len);
		while (pack_get(pachandle, &pac_buf, &pac_len) == 1)
		{
          		ret = net_send(nethandle, pac_buf, pac_len);
			if (ret != pac_len)
			{
				printf("send two pack failed, size: %d, err: %s\n", pac_len,
						strerror(errno));
			}
		}

		framecount++;
	}

	capture_stop(caphandle);
        net_close(nethandle);
	pack_close(pachandle);
	encode_close(enchandle);
	convert_close(cvthandle);
	timestamp_close(tmshandle);

}

void *threadOne()
{
	printf("one created \n");
	openOne();
}

void *threadTwo()
{
	printf("two created \n");
	openTwo();
}


int main(int argc, char *argv[])
{
	//openOne();
	//openTwo();
	pthread_t one,two;
	void *thread_result;
	int ret;
	ret = pthread_create(&one,NULL,threadOne,NULL);
	ret = pthread_create(&two,NULL,threadTwo,NULL);
	if(ret !=0)
	{
		printf("error\n");				
		exit(EXIT_FAILURE);
	}
	
	
	ret = pthread_join(one, &thread_result);
   	ret = pthread_join(two, &thread_result);
	if (ret != 0)
    	{
     		printf("Thread join failed!/n");
        	exit(EXIT_FAILURE);
    	}


    	exit(EXIT_FAILURE);
	return 0;
}
