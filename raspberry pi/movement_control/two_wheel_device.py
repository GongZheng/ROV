#!/usr/bin/python3
# coding=utf-8

class TwoWheelDevice:
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
        print("device start.")
        self.forward()
        self.__change_speed(self.speed1, self.speed2)

    def stop(self):
        self.__change_speed(0, 0)

    def __del__(self):
        print("stop motor")
        GPIO.output(self.Motor1E, GPIO.LOW)
        GPIO.output(self.Motor2E, GPIO.LOW)
        GPIO.cleanup()
