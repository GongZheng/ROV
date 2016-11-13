# 移动监控设计文档 #

## 名词解释 ##

名词          |       说明
----------  |----------
RMD  |  远程可操作移动监控设备
CTN   |  控制端


## API设计 ##

### 树莓派端 ###

名字          | 端口号   |   协议  |   作用  |   格式
-------------|----------|-------|---------|
probe       |   10045   |  udp  |应用端主动广播探测响应端口 |  
heartcount  |   10046   |  udp  |应用端测试是否在线端口   |
auth        |   10047   |  udp  |配置会话参数端口    |
action_control| 10048   |  tcp  |运动控制       |
camera      |   10049   |  udp/sctp | 图像传输接口 |

### 应用端 ###

名字          | 端口号   |   协议  |   作用  |   格式
-------------|----------|-------|---------|
alert       |   10045   |  tcp  | 树莓派遭遇紧急情况需要报告 | 

## 成员设计 ##

### 树莓派端(RMD) ###

名字          |  类型  |   作用  |   格式
------------|--------|-------|---------
ID          |   string   |  设备标识 | 
available_network   |   json    |   维护一个可用网络列表  |   {   "network_list":[...]}
private_key |   --- |   私钥  |

### 应用端(CTN) ###

名字          |  类型  |   作用  |   格式
------------|--------|-------|---------
available_device   |   json    |   维护一个可用设备列表  |    {  "device_list":[...] }
network_device  |   json | 记录网络下拥有的设备   | {  "bssid":[...]   }
*device_pk*   |   json    | 设备-公钥对应列表 | { "device.id":"device.pk" }

## 信息传递机制 ##
### 探测 ###
#### probe ####
```sequence
CTN->RMD: (broadcast)probe
RMD-->CTN: {  "device_id":RMD.ID }
```

### 认证 ###
#### auth ####
```sequence
CTN->RMD: auth apply
RMD-->CTN: {  "device_id":RMD.ID }
```


## 参考资料 ##

### 可用IBE算法 ###

名字          |  资料
------------| ---------
SM9          |   ---
Waters      |   ---
BF          | ---
