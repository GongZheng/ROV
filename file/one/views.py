# -*- coding: utf-8 -*-
from django.views.decorators.csrf import csrf_exempt
from django.shortcuts import render
from django.http import HttpResponse
import os
import uuid
import json
import models
import numpy
import socket
import fcntl
import struct
@csrf_exempt
 
def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    ip = socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])
    s.close()
    return ip

def upload(request):
  #  post 方法为文件流传入 断点续传大小为10MB，即大于10MB，上传进行文件切割
    if request.method == 'POST':
        token = request.GET.get('token','')
        fileName = request.GET.get('name','')
        '''get range  获取文件上传的范围'''
        ContentRange = request.META['HTTP_CONTENT_RANGE']
        ContentRange = ContentRange[5:]
        tempStr = ContentRange.split('/')
        rangeFrom = tempStr[0].split('-')[0]
        rangeSize = tempStr[1]
        rangeTo = tempStr[0].split('-')[1]
       
       # 验证有效的token，如果token有效，说明允许上传文件 
        uploadFile = handle_file(token,fileName)
        if uploadFile != None:
            uploadFile.seek(0,2)
            fileSize = uploadFile.tell()
           
            if int(fileSize) != int(rangeFrom):
               	return HttpResponse(json.dumps({'error post': 'filesize != range    '+ str( fileSize)}))
            for r in request.read():
                uploadFile.write(r)
            
            ''' 上传文件后，返回文件大小，通知客户端是否继续上传，或完成'''
            fileSize = uploadFile.tell()
            uploadFile.close()
            
            data = {'start':fileSize, 'success':True, 'message':''}
            
            response = HttpResponse(json.dumps(data))
            response['Access-Control-Allow-Headers'] = 'Content-Range,Content-Type'
            response['Access-Control-Allow-Origin'] = '*'
            response['Access-Control-Allow-Methods'] = 'POST, GET, OPTIONS'
            return response 
        return HttpResponse(json.dumps({'error post': 'error'}))
    else :
       #用于验证文件断开之后，文件在服务端的大小，一般情况下，第一次上传，都会先进行get,得到确切的文件大小后，客户端才继续上传

        token = request.GET.get('token','')
        size = request.GET.get('size',0)
        fileName = request.GET.get('name','')
        if token == '111':
	        return HttpResponse(json.dumps({'error get': 'file has existed'}))
        if has_token(token):
            fileTemp = handle_file(token,fileName)
            fileTemp.seek(0,2)
            start = fileTemp.tell()
            fileTemp.close()
            data ={'start':start, 'success':True, 'message':''}
            return HttpResponse(json.dumps(data))
        else :
	        return HttpResponse(json.dumps({'error get ': 'error'}))

''' 创建文件'''
def handle_file(token, filename):
    if not os.path.exists('upload/'):
        os.mkdir('upload/')
    if has_token(token):
        return open('upload/' + filename, 'a+')


''' 生成token '''
def get_token(request):
    if request.method == 'GET':
        name = request.GET.get('name', '')
        size = request.GET.get('size', 0)
        mytoken = hash(name + size)
    if not has_token(mytoken):
            obj = models.filetoken(token = mytoken )
            obj.save()
            data = { 'name':name, 'size':size, 'token':mytoken, 'success':True, 'message':''}
            return HttpResponse(json.dumps(data))
    else:
            data = { 'name':name, 'size':size, 'token':'111', 'success':False, 'message':'file has existed'}
            return HttpResponse(json.dumps(data))


''' 校验token '''
def has_token(token):
    tokens = models.filetoken.objects.all().values('token') 
    tokenStr  =  str(token)
    for temp in tokens:
        if temp['token'] == tokenStr:
            return True 
    return False

'''开启摄像头，控制是否录制'''
def startpicam(requst):
    address = requst.META['REMOTE_ADDR']
    if requst.method == 'GET':
        record = requst.GET.get('record','0')
    print record
    s = os.system('killall simple_test')
    p = os.system('/home/pi/ROV/Vcamkit/build/src/simple_test '+address+' '+record +' '+'/home/pi/ROV/file/upload')
    return HttpResponse("摄像头已开启！"+address)

'''关闭摄像头'''
def closepicam(requst):
    p = os.system('killall simple_test')
    return HttpResponse("摄像头已关闭！")

    
def video(requst):
    address =  requst.META['REMOTE_ADDR']
    strone = '''m=video 8000 RTP/AVP 96  
a=rtpmap:96 H264  
a=framerate:15  
c=IN IP4 '''+address+''' 
'''
    handle_video_file(strone,'0')   
    
    strtwo = '''m=video 8888 RTP/AVP 96  
a=rtpmap:96 H264  
a=framerate:15  
c=IN IP4 '''+address+''' 
'''
    handle_video_file(strtwo,'1')
    caraddress = requst.META['HTTP_HOST'].split(':')[0]
    with open('/home/pi/ROV/file/one/static/js/ip.js','w') as de:
        de.write("globalip =\'"+caraddress + ":8088\'\n ")
        de.write("videoip = \'"+caraddress + ":8888\' ")
    return render(requst,'car.html')

def handle_video_file(str,index):
    with open('/home/pi/ROV/file/one/static/js/' + 'video'+index+'.sdp', 'w') as destination:
        destination.write(str)


'''WIFI 设置'''
def wifi(request):
    return render(request, 'wificonfig.html')

def settingwifi(request):
    if request.method == 'POST':
        account = request.POST.get('account','')
        pwd = request.POST.get('password','')
        str = '''
network={
ssid="'''+account+'''"
scan_ssid=1
psk="'''+pwd+'''"
}
'''
	print  str
	with open('/etc/wpa_supplicant/wpa_supplicant.conf', 'a') as destination:
            destination.write(str)
            destination.close()
        p = os.system('sudo ifdown wlan1')
        q = os.system('sudo ifup wlan1')
        return HttpResponse("successful")
    else :
        return HttpResponse("error to set wifi configuration")


def getip(request):
	ip = get_ip_address("wlan0")
	return HttpResponse(ip)

''' ----------------------分割线-------------------------'''
def index(request):
    return render(request, 'test.html',{'what':'test file!'})

def file_index(request):
    return render(request, 'index.html')

def upload2(request):
    if request.method == 'POST':
        handle_uploaded_file(request.FILES['file'], str(request.FILES['file']))
        return HttpResponse("Successful")

    return HttpResponse("Failed")

def handle_uploaded_file(file, filename):
    if not os.path.exists('upload/'):
        os.mkdir('upload/')

    with open('upload/' + filename, 'wb+') as destination:
        for chunk in file.chunks():
            destination.write(chunk)
