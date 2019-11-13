#!/usr/bin/env python
#Tests sending a String command to the Arduino to begin a task

import serial

arduinoMegaPort = "/dev/ttyACM1"
arduinoMegaRate = "9600"

arduinoMegaSerial = serial.Serial(arduinoMegaPort,arduinoMegaRate)
arduinoMegaSerial.flushInput()

arduinoReadyForCommand ="ready\n"
doPostWindingTest = "doPostWindingTest\n"
isArduinoReady = "isArduinoReady\n"
helloThere = "Hello There!\n"

arduinoMegaSerial.write(isArduinoReady.encode())
while True:
    if arduinoMegaSerial.inWaiting() >0:
        inputValue = arduinoMegaSerial.readline().decode()
        print(inputValue)
        if inputValue == arduinoReadyForCommand:
            #if we want to do the post winding test, sendDoPostWinding
            print("Got ready command")
            n = input("Do post winding test? (y/n): ")
            if n == "y":
                arduinoMegaSerial.write(doPostWindingTest.encode())
            elif n == "n":
                arduinoMegaSerial.write(helloThere.encode())
            else:
                print("Input error, please input 'y' or 'n'")