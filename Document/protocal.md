# 移动监控设计文档 #

## 名词解释 ##

名词          |       说明
----------  |----------
C  |  手机端
S   |  树莓派服务端
R   | 树莓派小车
E(key, message) | 使用key作为密钥加密message
C(key, message) | 使用key作为密钥解密message


> 下划线的为后续版本功能

<!-- 
## 成员设计 ##

### 树莓派端(RMD) ###

名字          |  类型  |   作用  |   格式
------------|--------|-------|---------
ID          |   string   |  设备标识 | 
available_network   |   json    |   维护一个可用网络列表  |   {   "network_list":[...]}
~~private_key~~ |   --- |   私钥  |
user_key    |   string  | 用户对称密钥 |

### 应用端(CTN) ###

名字          |  类型  |   作用  |   格式
------------|--------|-------|---------
available_device   |   json    |   维护一个可用设备列表  |    {  "device_list":[...] }
network_device  |   json | 记录网络下拥有的设备   | {  "bssid":[...]   }
~~device_pk~~   |   json    | 设备-公钥对应列表 | { "device.id":"device.pk" }
user_key    |   string  | 用户对称密钥 | -->


## 协议设计 ##

### 树莓派端（R） ###

名字          |  协议  |   作用  
-------------|----------|-----
wifi_connet  |   10001   |  树莓派端接受手机的wifie信息并链接wifi   
~~heartcount~~  |   10002   |  检测树莓派小车是否在线   
ip_discover  |  10003   | 树莓派响应需要请求的ip地址给对应的请求者    
picture       |  20001 | 定时上传图片传输接口
~~camera~~      |   20002   |  实时图像传输接口 
action_control| 30001   |  运动控制     


### 树莓派服务器端（S） ###

名字          |  协议  |   作用  
-------------|----------|-----
wifi_connet  |   10001   |  树莓派端接受手机的wifie信息并链接wifi   
~~heartcount~~  |   10002   |  检测树莓派小车是否在线   
ip_discover  |  10003   | 树莓派响应需要请求的ip地址给对应的请求者    
picture       |  20001 | 定时上传图片传输接口
~~camera~~      |   20002   |  实时图像传输接口 
action_control| 30001   |  运动控制     


### 手机控制端（C） ###

名字          |  协议  |   作用  
-------------|----------|-----
wifi_connet  |   10001   |  树莓派端接受手机的wifie信息并链接wifi   
~~heartcount~~  |   10002   |  检测树莓派小车是否在线   
ip_discover  |  10003   | 树莓派响应需要请求的ip地址给对应的请求者    
picture       |  20001 | 定时上传图片传输接口
~~camera~~      |   20002   |  实时图像传输接口 
action_control| 30001   |  运动控制       


