Pi的视频流传输
=============
##安装与测试
###pi或ubuntu上安装opencv3.1版本
   pi或ubuntu上安装opencv的编译安装过程具体看[这里。](http://blog.csdn.net/linj_m/article/details/45048905)
   opencv-3.1.00zip的包已经放在了项目的[文件](https://coding.net/u/gongz/p/ROV/attachment)中。
###pc上安装nodejs
[windows下](http://jingyan.baidu.com/article/b0b63dbfca599a4a483070a5.html)
[ubuntu下](http://jingyan.baidu.com/article/72ee561a533c55e16138dfef.html)

###测试
pc上安装好nodejs后，修改nodejs/server.js目录下的：
>var HOST = '192.168.1.176';//本机ip
>var PORT = 8001;           //本机接收端口

为本机ip和需要接收视频的端口
终端运行
>node server.js

pi上安装好opencv（注：python3下可用）后，修改process_client.py
>address = ('192.168.1.176', 8001) #服务器接收端地址

运行
>python3 process_client.py

在浏览器上输入网址：http://192.168.1.176:3000/
即可查看视频。
#####注：本process_client.py脚本默认已经安装两个摄像头，若只有一个，请注释掉一个进程。