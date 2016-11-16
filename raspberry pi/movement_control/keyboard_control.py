#!/usr/bin/python
# -*- coding: UTF-8 -*-#
#
# 通过l298n驱动两轮小车
#
# sudo apt-get install python3-dev
# sudo easy_install3 RPi.GPIO
#
import RPi.GPIO as GPIO
from time import sleep
import threading
import sys
import sys,tty,termios


from two_wheel_device import TwoWheelDevice as Car

class KeyEventThread(threading.Thread):
    def __init__(self, car):
        threading.Thread.__init__(self)
        self.car = car
    def getch(self):
        fd = sys.stdin.fileno()
        old =   termios.tcgetattr(fd)
        try:
            tty.setraw(fd)
            return sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old)
    def run(self):
        print("thread start.")
        print("Press 'q' to exit")
        print("'w' = forward, 's' = backward, 'a' = left, 'd' = right, 'c' = stop")
        print("'z' = speed down, 'x' = speed up")
        is_first = True
        while True:
            key = self.getch()

            if (key in ['w','s','a','d']) and is_first:
                self.car.start()
                is_first = False
            if  key == 'q':
                del self.car
                exit()
                return
            elif key == 'w':
                self.car.forward()
            elif key == 's':
                self.car.backward()
            elif key == 'a':
                self.car.left()
            elif key == 'd':
                self.car.right()
            elif key == 'c':
                self.car.stop()
                is_first = True
            elif key == 'z':
                self.car.speed_down()
            elif key == 'x':
                self.car.speed_up()
            else:
                print("key=" + key)
        return


if __name__ == "__main__":
    car = Car(11, 12, 13, 15, 16, 18)

    keythread = KeyEventThread(car)
    keythread.start()
