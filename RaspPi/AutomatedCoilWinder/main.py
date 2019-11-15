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
from UserInterface import *
from WindingWriter import *
from WindingReader import *
from SensorReader import *

def main():
    """Begin and run the coil winding program."""
    # --------------------- Variables --------------------- #
    #arduinoMegaPort = "/dev/ttyACM0"
    #arduinoMegaRate = "9600"

    arduinoReadyForCommand = "ready\n"

    sensorValue = None
    sensor = "getSensor1SensorValue\n"

    # ------------------- Declarations -------------------- #

    # Create serial connection for arduinos
    #arduinoMegaSerial = serial.Serial(arduinoMegaPort, arduinoMegaRate)
    #arduinoMegaSerial.flushInput()

    # ----------------------- Main ------------------------ #

    # Read and store user defined stator properties
    ui = UserInterface()
    ui.displayMessage("Starting program")
    ui.displayMessage("Please enter stator properties: ")
    windingWriter = WindingWriter(ui.getStatorToothLength(),
                                  ui.getStatorToothHeight(),
                                  ui.getStatorToothWidth(),
                                  ui.getStatorLipRadius(),
                                  ui.getNumberStatorTeeth(),
                                  ui.getNumberWireLayers(),
                                  ui.getWireGauge(),
                                  ui.getWireMaterial(),
                                  ui.getDistanceBetweenTeeth())
    windingReader = WindingReader();

    # TODO:
    #   * Generate path
    #   * Send path to winding reader
    #   * Zero machine with WindingReader -> function for zeroing w/ SensorReader
    #   * Send path with WindingReader
    #   * Post winding test with WindingTest()

    # Generate path
    ui.displayMessage("\nGenerating the path")
    windingWriter.generatePath("pathFile.txt")
    # TODO: instantiate windingReader, giving it the file name of generated path

    # Zero machine with WindingReader -> function for zeroing w/ SensorReader
    ui.userZero()
    ui.displayMessage("\nZeroing")
    # windingReader.zeroMachine()

    # Send path with WindingReader
    ui.userStartWinding()
    ui.displayMessage("\nWinding")
    windingReader.sendPath("pathFile.txt")

    # Post winding test with WindingTest()
    ui.userStartPostWindingTest()
    ui.displayMessage("\nStarting post-winding test")

    ui.displayMessage("Program completed.")

# do stuff in main
main()