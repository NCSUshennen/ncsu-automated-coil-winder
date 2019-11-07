#! /usr/bin/python3
# ------------------------------------------------- #
# NCSU Senior Design: Automated Coil Winder
#
# main.py
#    Master command: runs through winding program
#
# Created: November 2019
# Last modified: November 2019
# ------------------------------------------------- #

# imports
import serial
from SensorReader import *

def main():
    """Begin and run the coil winding program."""
    # --------------------- Variables --------------------- #
    arduinoMegaPort = "/dev/ttyACM7"
    arduinoMegaRate = "9600"

    arduinoReadyForCommand = "ready\n"

    sensorValue = None
    sensor = "getSensor1SensorValue\n"

    # ------------------- Declarations -------------------- #

    # Create serial connection for arduinos
    arduinoMegaSerial = serial.Serial(arduinoMegaPort, arduinoMegaRate)
    arduinoMegaSerial.flushInput()

    # ----------------------- Main ------------------------ #
    # TODO: instantiate other classes that will help run through post winding
    arduinoMegaSensorReader = SensorReader(arduinoMegaSerial)
    sensorValue = arduinoMegaSensorReader.readSensor(sensor)
    print("Sensor value: " + sensorValue)

# do stuff in main
main()