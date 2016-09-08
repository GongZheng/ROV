 sudo apt-get update
 sudo apt-get install python3 python3-dev python3-setuptools python3-serial
 sudo apt-get install pigpio
 sudo easy_install3 -U RPIO
 如果在树莓派3使用RPIO这个库，它可能会提示“只能在树莓派上运行”，然后退出
 因为他检测处理器型号，其中不包括树莓派3的型号，找到文件把型号判断注释掉就好了
 另：RPIO不是必需的，用GPIO也可以操作电机，RPIO其实就是GPIO的一个子类，提供了一些方便
 sudo raspi-config
 - 9 Advanced Options
 - A7 Serial
     选No，关闭系统默认的把命令行的内容重定向到串口的功能，重启
 注：最新版的Raspbian在执行上述操作后串口设备可能会消失，这是一个Bug，修复方法如下：
     将/boot/config.txt最后的enable_uart=0改为enable_uart=1
 树莓派3的串口设备号可能是ttyS0而不是ttyAMA0
 又：发现前面安装的RPIO和最新发行版的内核不兼容，所以又安装了最新的测试版RPIO
 从https://github.com/metachris/RPIO/tree/v2下载并上传到树莓派，并运行其中的setup.py
sudo python3 setup.py install
sudo python3 car_server_pwm.py
然后发起url请求：pi的ip地址/car?v=(速度1-100)&a=1（0、1、2、3、4、5、6）进行控制
参考于这篇文章http://www.sjsjw.com/100/005531MYM025948/