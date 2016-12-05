#!/bin/bash
# 添加sudo运行

# 树莓派配置脚本
# 用sudo来运行此脚本

cp ./my_supplicant.conf /etc/wpa_supplicant/my_supplicant.conf
cat ./interfaces > /etc/network/interfaces

systemctl daemon-reload
service networking restart
sudo systemctl restart networking.service

# 树莓派源列表 http://www.raspbian.org/RaspbianMirrors
cp ./sources.list /etc/apt/

apt-get update
apt-get upgrade
apt-get install vim
apt-get install python3
apt-get install g++
apt-get install cmake

apt-get install libswscale-dev

cp /etc/systemd/system/default.target /lib/systemd/system/multi-user.target
echo "Created symlink from /etc/systemd/system/default.target to /lib/systemd/system/multi-user.target."

# firmware 安装
# openmaxil， camkit依赖

# camkit
# http://shumeipai.nxez.com/2015/01/28/raspberry-pi-supports-hard-coded-video-suite.html
# https://git.oschina.net/andyspider/Camkit


# ffmpeg
# http://tieba.baidu.com/p/3418475592


# # libavcodec 安装
# apt-get install libavcodec-dev

# # 在树莓派中编译camkit  ,其中-DPLATFORM=RPI是说明该环境是树莓派环境
# cp camkit
# mkdir build
# cd build
# cmake ../ -DPLATFORM=RPI
# make
# make install

# ＃测试是否安装成功，安装成功后，进入build/src
# cd src
# ./cktool -?
