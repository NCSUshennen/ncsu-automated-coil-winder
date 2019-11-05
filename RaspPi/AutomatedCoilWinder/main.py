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
    arduinoMegaPort = "/dev/ttyACM0"
    arduinoMegaRate = "9600"

    arduinoReadyForCommand = "ready\n"

    # ------------------- Declarations -------------------- #

    # Create serial connection for arduinos
    arduinoMegaSerial = serial.Serial(arduinoMegaPort, arduinoMegaRate)
    arduinoMegaSerial.flushInput()

    # ----------------------- Main ------------------------ #
    # TODO: instantiate other classes that will help run through post winding
    sensorReader = SensorReader(arduinoMegaSerial)
    # sensorValue = sensorReader.readSensor(sensor)
    # print(sensorValue)

# do stuff in main
main()