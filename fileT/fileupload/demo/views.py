# -*- coding: utf-8 -*-
from django.views.decorators.csrf import csrf_exempt
from django.shortcuts import render
from django.http import HttpResponse
import os
import uuid
import json
import models
import numpy
@csrf_exempt
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
def token(request):
    if request.method == 'GET':
        name = request.GET.get('name', '')
        size = request.GET.get('size', 0)
        mytoken = hash(name + size)
        obj = models.filetoken(token = mytoken )
        obj.save()
        data = { 'name':name, 'size':size, 'token':mytoken, 'success':True, 'message':''}
        return HttpResponse(json.dumps(data))


''' 校验token '''
def has_token(token):
    tokens = models.filetoken.objects.all().values('token') 
    tokenStr  =  str(token)
    for temp in tokens:
        if temp['token'] == tokenStr:
            return True 
    return False

def index(request):
    return render(request, 'index.html')



