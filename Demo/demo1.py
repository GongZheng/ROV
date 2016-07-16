'''

这段演示代码实现了打开一个http服务接收来自手机的指令，处理和记录IMU传感器模块通过串口
传来的数据，以及简单的调整电机速度的功能。

我们可以以此为基础来增加一个通过http接收的新的指令，比如说setdirection，然后把调整电
机速度那个线程改成根据上述指令提供的目标角度来自动控制电机速度调整自己姿态的代码。

'''

# 基于全新安装的RASPBIAN JESSIE LITE，硬件是树莓派1 B+ v1.2
# Version: May 2016
# Release date: 2016-05-27

# > sudo apt-get update
# > sudo apt-get install python3 python3-dev python3-setuptools python3-serial
# > sudo apt-get install pigpio

# > sudo easy_install3 -U RPIO
# 如果在树莓派3使用RPIO这个库，它可能会提示“只能在树莓派上运行”，然后退出
# 因为他检测处理器型号，其中不包括树莓派3的型号，找到文件把型号判断注释掉就好了
# 另：RPIO不是必需的，用GPIO也可以操作电机，RPIO其实就是GPIO的一个子类，提供了一些方便

# > sudo raspi-config
#     - 9 Advanced Options
#     - A7 Serial
#     选No，关闭系统默认的把命令行的内容重定向到串口的功能，重启

# 注：最新版的Raspbian在执行上述操作后串口设备可能会消失，这是一个Bug，修复方法如下：
#     将/boot/config.txt最后的enable_uart=0改为enable_uart=1
# 树莓派3的串口设备号可能是ttyS0而不是下面的ttyAMA0

# 又：发现前面安装的RPIO和最新发行版的内核不兼容，所以又安装了最新的测试版RPIO
# 从https://github.com/metachris/RPIO/tree/v2下载并上传到树莓派，并运行其中的setup.py
# > sudo python3 setup.py install

from http.server import BaseHTTPRequestHandler, HTTPServer
from RPIO import PWM
import time, signal, urllib.parse, threading, sys, shutil, copy, io, socket, struct, serial, math

# Global variables for attitude
pitch = 0
roll = 0
yaw = 0

# Global variables for motors
speeds = [0, 0, 0, 0];
motorpins = [17, 18, 22, 27];

# 处理控制端发来的HTTP GET请求，现在只有一个指令setspeed
class GetHandler(BaseHTTPRequestHandler):
	def do_GET(self):
		uri = urllib.parse.urlparse(self.path)
		command = uri.path.rsplit('/', 1)[-1]
		if command == 'setspeed':
			global speeds
			params = urllib.parse.parse_qsl(uri.query)
			for key, val in params:
				if key == 'v':
					v = int(val)
				elif key == 'n':
					n = int(val)
			speeds[n] = v
			self.send_response(200)
			self.end_headers()
			self.wfile.write(bytes(command, 'UTF-8'))
		else:
			self.send_response(404)
			self.end_headers()
			self.wfile.write(bytes('File Not Found', 'UTF-8'))
		return

# 处理IMU（集成AHRS算法）从串口发来的数据，也包括给IMU发送校正指令等等
class AhrsProcessor(threading.Thread):
	def run(self):
		global pitch, roll, yaw
		uart = serial.Serial(port='/dev/ttyAMA0', baudrate = 115200, timeout=1)
		uart.write(b'\xA5\x57\xFC')
		n = 0
		while True:
			if n % 200 == 0:
				uart.write(b'\xA5\x58\xFD')
				n = 0
			# 感觉对字符串.decode("utf-8")会有问题，另外接下来要用2进制格式接收数据了
			data = uart.readline().decode("utf-8")
			if len(data) == 0:
				uart.write(b'\xA5\x55\xFA')
				print('Sent euler angles command')
			elif data.startswith('#RPY'):
				try:
					roll = float(data[4:11]) / 180 * math.pi
					pitch = float(data[12:19]) / 180 * math.pi
					yaw = float(data[20:27]) / 180 * math.pi
				except:
					pass
			else:
				pass # print(data)
			n += 1

# 这个线程简单的调整电机的速度
class MotorThread(threading.Thread):
	def run(self):
		global speeds, motorpins
		servo = PWM.Servo() # 主要代码
		currentspeeds = [0, 0, 0, 0]
		while True:
			for i in range(4):
				if currentspeeds[i] != speeds[i]:
					servo.set_servo(motorpins[i], speeds[i]) # 主要代码
					currentspeeds[i] = speeds[i]
			time.sleep(0.1)

def main():
	print('Starting Attitude and Heading Reference System...')
	ahrsthread = AhrsProcessor()
	ahrsthread.daemon = True
	ahrsthread.start()
	print('Starting Motor Processor...')
	mthread = MotorThread()
	mthread.daemon = True
	mthread.start()
	print('Starting Operational Interface...')
	server = HTTPServer(('0.0.0.0', 80), GetHandler)
	server.serve_forever()

def exit(signum, frame):
	print('Exiting from exit()...')
	sys.exit(1)

if __name__ == '__main__':
	signal.signal(signal.SIGINT, exit)
	main()
