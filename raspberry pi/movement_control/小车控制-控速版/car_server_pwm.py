#!/usr/bin/python3
#coding=utf-8
from http.server import BaseHTTPRequestHandler, HTTPServer
import RPi.GPIO as GPIO
import L298N_car2_pwm as car
import time, signal, urllib.parse, threading, sys, shutil, copy, io, socket, struct, serial, math


# Global variables for motors
v = 50
a = 0
# 处理控制端发来的HTTP GET请求，现在只有一个指令setspeed
class GetHandler(BaseHTTPRequestHandler):
	def do_GET(self):
		uri = urllib.parse.urlparse(self.path)
		command = uri.path.rsplit('/', 1)[-1]
		if command == 'car':
			global v,a,b
			params = urllib.parse.parse_qsl(uri.query)
			for key, val in params:
				if key == 'v':
					v = int(val)
				elif key == 'a':
					a = int(val)
			self.send_response(200)
			self.end_headers()
			self.wfile.write(bytes(command, 'UTF-8'))
		else:
			self.send_response(404)
			self.end_headers()
			self.wfile.write(bytes('File Not Found', 'UTF-8'))
		return


# 这个线程简单的调整电机的速度
class MotorThread(threading.Thread):
	def run(self):
		global v,a,b
		car.init()
		car.reset()
		while True:
			car.setDC(v)
			if (a == 0):
				car.stop()
			elif (a==1):
				car.forward()
			elif (a==2):
				car.back()
			elif (a==3):
				car.front_left_turn()
			elif (a==4):
				car.front_right_turn()
			elif (a==5):
				car.rear_left_turn()
			elif (a==6):
				car.rear_right_turn()
			else:
				car.stop()
			time.sleep(0.1)
def main():
	print('Starting Motor Processor...')
	mthread = MotorThread()
	mthread.daemon = True
	mthread.start()
	print('Starting Operational Interface...')
	server = HTTPServer(('0.0.0.0', 80), GetHandler)
	server.serve_forever()

def exit(signum, frame):
	print('\nExiting from exit()...')
	GPIO.cleanup()
	sys.exit(1)

if __name__ == '__main__':
	signal.signal(signal.SIGINT, exit)
	main()
