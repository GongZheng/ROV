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


class Car:
    """两轮小车"""
    """ m1a，m1b为第一个马达的A，B输入，m1e为第一个马达的使能端；
        m2a，m2b为第二个马达的A，B输入，m2e为第二个马达的使能端。"""
    def __init__(self, m1a, m1b, m1e, m2a, m2b, m2e):
        GPIO.setmode(GPIO.BOARD) #GPIO.BCM

        GPIO.setup(m1a, GPIO.OUT)
        GPIO.setup(m1b, GPIO.OUT)
        GPIO.setup(m1e, GPIO.OUT)
        GPIO.setup(m2a, GPIO.OUT)
        GPIO.setup(m2b, GPIO.OUT)
        GPIO.setup(m2e, GPIO.OUT)

        self.Motor1A = m1a
        self.Motor1B = m1b
        self.Motor1E = m1e
        self.Motor2A = m2a
        self.Motor2B = m2b
        self.Motor2E = m2e
        self.speed1 = 0
        self.speed2 = 0
        self.frequency = 100
        self.m1 = GPIO.PWM(self.Motor1E, self.frequency)
        self.m1.start(0)
        self.m2 = GPIO.PWM(self.Motor2E, self.frequency)
        self.m2.start(0)

    def forward(self):
        print("forward")
        GPIO.output(self.Motor1A, GPIO.HIGH)
        GPIO.output(self.Motor1B, GPIO.LOW)
        GPIO.output(self.Motor2A, GPIO.HIGH)
        GPIO.output(self.Motor2B, GPIO.LOW)

    def backward(self):
        print("backward")
        GPIO.output(self.Motor1A, GPIO.LOW)
        GPIO.output(self.Motor1B, GPIO.HIGH)
        GPIO.output(self.Motor2A, GPIO.LOW)
        GPIO.output(self.Motor2B, GPIO.HIGH)

    def left(self):
        print("left")
        GPIO.output(self.Motor1A, GPIO.LOW)
        GPIO.output(self.Motor1B, GPIO.HIGH)
        GPIO.output(self.Motor2A, GPIO.HIGH)
        GPIO.output(self.Motor2B, GPIO.LOW)

    def right(self):
        print("right")
        GPIO.output(self.Motor1A, GPIO.HIGH)
        GPIO.output(self.Motor1B, GPIO.LOW)
        GPIO.output(self.Motor2A, GPIO.LOW)
        GPIO.output(self.Motor2B, GPIO.HIGH)

    def speed_up(self):
        self.speed1 = self.speed1 + 10
        self.speed2 = self.speed2 + 10
        if self.speed1 > 100 :
            self.speed1 = 100
        if self.speed2 > 100 :
            self.speed2 = 100
        self.__change_speed(self.speed1, self.speed2)

    def speed_down(self):
        self.speed1 = self.speed1 - 10
        self.speed2 = self.speed2 - 10
        if self.speed1 < 0 :
            self.speed1 = 0
        if self.speed2 < 0 :
            self.speed2 = 0
        self.__change_speed(self.speed1, self.speed2)

    def __change_speed(self, s1, s2):
        self.m1.ChangeDutyCycle(s1)
        self.m2.ChangeDutyCycle(s2)
        print("change speed:", s1, s2)

    def start(self):
        self.speed1 = 30
        self.speed2 = 30
        print("car start.")
        self.forward()
        self.__change_speed(self.speed1, self.speed2)

    def stop(self):
        self.__change_speed(0, 0)

    def __del__(self):
        print("stop motor")
        GPIO.output(self.Motor1E, GPIO.LOW)
        GPIO.output(self.Motor2E, GPIO.LOW)
        GPIO.cleanup()


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
