
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include <fcntl.h> // open close
#include <sys/shm.h>
#include <netinet/in.h>
#include <sys/socket.h> // socket
#include <sys/types.h>  // 基本数据类型
#include <unistd.h> // read write
#include <signal.h>
#include <time.h>
#include "camkit.h"

#define WIDTH 352
#define HEIGHT 288
//#define WIDTH 640
//#define HEIGHT 480
#define FRAMERATE 15
#define recordMaxtime  999999999//视频录制最长时间

//http监听部分全局变量和函数开始
#define PORT 8888
#define SERV "0.0.0.0"
#define QUEUE 20
#define BUFF_SIZE 1024

pthread_t thread[2];
pthread_mutex_t mut;

FILE *dumpfileOne,*dumpfileTwo;
time_t startTime;
int snapShot;
int record;
char fileaim[1024];
typedef struct doc_type{
        char *key;
        char *value;
}HTTP_CONTENT_TYPE;

HTTP_CONTENT_TYPE http_content_type[] = {
        { "html","text/html" },
        { "gif" ,"image/gif" },
        { "jpeg","image/jpeg" }
};

int sockfd;
char *http_res_tmpl = "HTTP/1.1 200 OK\r\n"
        "Server: ROV V1.0\r\n"
        "Accept-Ranges: bytes\r\n"
        "Access-Control-Allow-Origin:*\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "Content-Type: %s\r\n\r\n";

// http 发送相应报文
void http_send(int sock_client,char *content){ 
        char HTTP_HEADER[BUFF_SIZE],HTTP_INFO[BUFF_SIZE];
        int len = strlen(content);
        sprintf(HTTP_HEADER,http_res_tmpl,len,"text/html");
        len = sprintf(HTTP_INFO,"%s%s",HTTP_HEADER,content);
        send(sock_client,HTTP_INFO,len,0);
}

// 退出信号处理
void handle_signal(int sign){
        fputs("\nSIGNAL INTERRUPT \nBye\nSAFE EXIT\n",stdout);
        close(sockfd);
        exit(0);
}

void *httplisten(){
		signal(SIGINT,handle_signal);

        // 定义 socket
        sockfd = socket(AF_INET,SOCK_STREAM,0);
        // 定义 sockaddr_in
        struct sockaddr_in skaddr;
        skaddr.sin_family = AF_INET; // ipv4
        skaddr.sin_port   = htons(PORT);
        skaddr.sin_addr.s_addr = inet_addr(SERV);
        // bind，绑定 socket 和 sockaddr_in
        if( bind(sockfd,(struct sockaddr *)&skaddr,sizeof(skaddr)) == -1 ){
                perror("bind error");
                exit(1);
        }

        // listen，开始添加端口
        if( listen(sockfd,QUEUE) == -1 ){
                perror("listen error");
                exit(1);
        }

        // 客户端信息
        char buff[BUFF_SIZE];
        struct sockaddr_in claddr;
        socklen_t length = sizeof(claddr);

        while (1)
        {
            int sock_client = accept(sockfd,(struct sockaddr *)&claddr, &length);
            //printf("%d\n",sock_client);
            if( sock_client <0 ){
                    perror("accept error");
                    exit(1);
            }
            memset(buff,0,sizeof(buff));
            int len = recv(sock_client,buff,sizeof(buff),0);
            //printf("che:%s\n",buff);
            char *token = strtok(buff," ");
            token = strtok(NULL," ");
            if (strcmp("/snapShot",token)==0&&snapShot!=1){
            	pthread_mutex_lock(&mut);
            	snapShot = 1;
                pthread_mutex_unlock(&mut);
            	while(snapShot!=0){ //等待截图成功
            		printf("\r%s","snapShoting......");
            	}
            	sprintf(buff,"%s\n","截图成功" );
            	printf("\r%s\n","snapShot had finished!");
            }else{
            	sprintf(buff,"%s\n",token);
            }
            http_send(sock_client,buff);
            close(sock_client);
        }
        //fputs("Bye ROV",stdout);
        close(sockfd);
}
//http监听部分结束

// set paraments
U32 vfmt = V4L2_PIX_FMT_YUYV;
U32 ofmt = V4L2_PIX_FMT_YUV420;

char *deviceOne = "/dev/video0";
char *deviceTwo = "/dev/video1";

char oneIp[24];
char twoIp[24];
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

int *openOne()
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

	int both = 0;
	static char str_time[100]; 
	struct tm *local_time = NULL; 
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

		if (snapShot==1) //截图
		{ 	
			time_t shotTime = time((time_t*)NULL);;
			local_time = localtime(&shotTime); 
			strftime(str_time, sizeof(str_time), "%Y_%m_%d_%H_%M_%S", local_time);
			char picname[108];
			if (flag==1&&both==0)
			{
				sprintf(picname,"%s/%s_pic0.bmp",fileaim,str_time);
				capture_bmp(cap_buf,cap_len,picname);
				both++;
			}
			if (flag==0&&both==1)
			{
				sprintf(picname,"%s/%s_pic1.bmp",fileaim,str_time);
				capture_bmp(cap_buf,cap_len,picname);
				pthread_mutex_lock(&mut);
	        	snapShot=0;
	        	pthread_mutex_unlock(&mut);
	        	both=0;
			}
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

		//时间戳显示
		//timestamp_draw(tmshandle,cvt_buf);

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

		
		if(record==1&&(int)startTime==0){
			printf("%d\n",(int)startTime );
			startTime = time((time_t*)NULL);
			local_time = localtime(&startTime); 
			strftime(str_time, sizeof(str_time), "%Y_%m_%d_%H_%M_%S", local_time); 
			//printf("%s\n", str_time);
			char fileOnename[108];
			char fileTwoname[108];
			sprintf(fileOnename,"%s/%s_cam0.h264",fileaim,str_time);
			sprintf(fileTwoname,"%s/%s_cam1.h264",fileaim,str_time);
			dumpfileOne = fopen(fileOnename,"wb");
			dumpfileTwo = fopen(fileTwoname,"wb");
			printf("%s recording...\n",fileOnename);
			printf("%s recording...\n",fileTwoname);
		}
		if(record==1){
			if(((int)time((time_t*)NULL)-(int)startTime)<=recordMaxtime){
	        	if(flag){
	        		fwrite(enc_buf, 1, enc_len, dumpfileOne);
	        	}
	        	else{
	        		fwrite(enc_buf, 1, enc_len, dumpfileTwo);
	        	}
			}else if (dumpfileOne!=NULL&&dumpfileTwo!=NULL)
			{
				fclose(dumpfileOne);
				fclose(dumpfileTwo);
				pthread_mutex_lock(&mut);
	        	record=0;
	        	pthread_mutex_unlock(&mut);
	        	startTime = 0;
	        	dumpfileOne = NULL;
	        	dumpfileTwo = NULL;
				//printf("record finished!\n");
				printf("record finished...\n");
			}
		}
		

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

int strcopy(char str1[],char str2[])
{
   int length = strlen(str2);
   int i = 0;
   for( i=0;i<=length;i++)str1[i]=str2[i];
   return 0;
}

void thread_create(void)
{
        int temp;
        memset(&thread, 0, sizeof(thread));          //comment1
        /*创建线程*/
        if((temp = pthread_create(&thread[0], NULL, httplisten, NULL)) != 0)       //comment2
                printf("httplisten create failed...\n");
        else
                printf("httplisten create successed...\n");
        if((temp = pthread_create(&thread[1], NULL, openOne, NULL)) != 0)  //comment3
                printf("openOne create failed...");
        else
                printf("openOne create successed...\n");
}

void thread_wait(void)
{
        /*等待线程结束*/
        if(thread[0] !=0) {                   //comment4
                pthread_join(thread[0],NULL);
                printf("httplisten finished...\n");
        }
        if(thread[1] !=0) {                //comment5
                pthread_join(thread[1],NULL);
                printf("openOne finished...\n");
        }
}

int main(int argc,char *argv[])
{
        
    strcopy(oneIp,argv[1]);
    //twoIp = argv[0];
    printf("     ------   %s\n",oneIp);

    strcopy(twoIp,argv[1]);

    if(strcmp("1",argv[2])==0)
    	record = 1;
    else
    	record = 0;

    if(strlen(argv[3])!=0)
    	strcpy(fileaim,argv[3]);
	pthread_mutex_init(&mut,NULL);
    printf("PiCam start....\n");
    thread_create();
    printf("PiCam hold...\n");
    thread_wait();

	return 0;
}

