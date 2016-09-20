# -*- coding: UTF-8 -*-

import multiprocessing
import socket
import cv2
import numpy
import base64
#import sys
#reload(sys)
#sys.setdefaultencoding('utf8')
def camera(address,camID): #提前摄像头并传输给服务器
	sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	sock.connect(address)   #建立连接address为ip和端口
	capture = cv2.VideoCapture(camID)  #打开摄像头
	ret, frame = capture.read()        #抓取摄像头一帧
	encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90] #参数
	print ('cam%d on'%camID)
	while ret:
		result, imgencode = cv2.imencode('.jpg', frame, encode_param) #编码
		data = numpy.array(imgencode) #转换为矩阵
		stringData = data.tostring() #转换为字符串流
		c = str(camID);
		#sock.send( (str(len(stringData)).encode("utf-8")).ljust(16)); #发送字符串流的大小(转换为16字节大小)
		sock.send( base64.b64encode(stringData)+b';'+c.encode("utf-8"))                                      #发送字符串流
		print ("read")
		ret, frame = capture.read()
		print ("read end")                                  #抓取下一帧
		#decimg=cv2.imdecode(data,1)
		#cv2.imshow('CLIENT',decimg)
		if cv2.waitKey(100) == 27:                   #等待
			break;
	sock.close()                #关闭套接字
	cv2.destroyAllWindows()     #关闭cv窗口,可无

def main():
	address = ('192.168.1.176', 8001) #服务器接收端地址
	# 创建新进程
	process0 = multiprocessing.Process(target = camera, args = (address,0,))
	process1 = multiprocessing.Process(target = camera, args = (address,1,))


	# 开启进程
	process0.start()
	process1.start()


if __name__ == '__main__':
	main()
