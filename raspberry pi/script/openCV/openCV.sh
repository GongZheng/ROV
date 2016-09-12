#!/bin/bash
# openCV安装
sudo apt-get update
sudo apt-get upgrade

# 搭建C/C++编译环境：
sudo apt-get install build-essential

# 安装关联库：
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev


cd ../lib/opencv-3.1.0
cmake .
sudo make
sudo make install
sudo /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
sudo ldconfig

# 测试
# cd opencv/opencv-3.0.0-beta/samples/
# sudo cmake .
# sudo make -j $(nproc)
# cd cpp/
# ./cpp-example-facedetect girls.jpg
