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
# from UserInterface import *
from WindingWriter import *
from WindingReader import *
from WindingTester import *


class MainController:
    arduinoMegaSerial = None
    arduinoMegaPort = "/dev/ttyACM0"
    arduinoMegaRate = "9600"
    arduinoReadyForCommand = "ready\n"

    statorToothLength = None
    statorToothHeight = None
    statorWindHeight = None
    statorToothWidth = None
    statorShoeWidth = None
    numberStatorTeeth = None
    numberWinds = None
    wireGauge = None
    wireMaterial = None
    distanceBetweenTeeth = None
    statorWindWidth = None
    statorDiameter = None
    wireResistance = None

    windingWriter = None
    windingReader = None
    windingTester = None

    mmToFeet = 0.00328084

    def __init__(self):
        """Construct a new Main by setting up serial connection
                """
        # Create serial connection for arduinos
        # self.arduinoMegaSerial = serial.Serial(self.arduinoMegaPort, self.arduinoMegaRate)
        # self.arduinoMegaSerial.flushInput()
        # self.windingReader = WindingReader(self.arduinoMegaSerial)
        return

    def buildGCode(self, statorToothLength,
                   statorToothHeight,
                   statorWindHeight,
                   statorToothWidth,
                   statorShoeWidth,
                   numberStatorTeeth,
                   numberWinds,
                   wireGauge,
                   wireMaterial,
                   distanceBetweenTeeth, statorWindWidth, statorDiameter):
        """Construct a new Main

                        Keyword arguments:
                        statorToothLength --
                        statorToothHeight --
                        statorToothWidth --
                        statorShoeWidth --
                        numberStatorTeeth --
                        numberWinds --
                        wireGauge --
                        wireMaterial --
                        distanceBetweenTeeth --
                        """
        self.statorToothLength = statorToothLength
        self.statorToothHeight = statorToothHeight
        self.statorWindHeight = statorWindHeight
        self.statorToothWidth = statorToothWidth
        self.statorShoeWidth = statorShoeWidth
        self.numberStatorTeeth = numberStatorTeeth
        self.numberWinds = numberWinds
        self.wireGauge = wireGauge
        self.wireMaterial = wireMaterial
        self.distanceBetweenTeeth = distanceBetweenTeeth
        self.statorWindWidth = statorWindWidth
        self.statorDiameter = statorDiameter

        # Instantiate WindingWriter, WindingReader, WindingTester
        self.windingWriter = WindingWriter(statorToothLength,
                                           statorToothHeight,
                                           statorWindHeight,
                                           statorToothWidth,
                                           statorShoeWidth,
                                           numberStatorTeeth,
                                           numberWinds,
                                           wireGauge,
                                           wireMaterial,
                                           distanceBetweenTeeth, statorWindWidth)
        self.windingTester = WindingTester(statorToothLength,
                                           statorToothHeight,
                                           statorToothWidth,
                                           statorShoeWidth,
                                           numberStatorTeeth,
                                           numberWinds,
                                           wireGauge,
                                           wireMaterial,
                                           distanceBetweenTeeth)

        # Generate gcode
        self.windingWriter.generatePath("pathFile.txt")

    def startWinding(self):
        """Sends the path with the Arduino with the windingReader
                """
        return self.windingReader.sendPath("pathFile.txt")

    def startPostWindingTest(self):
        """Starts the post winding test
                        """
        self.windingTester.postWindingTest("postWindingTests.csv")

    def getPredictedTime(self):
        """Returns predicted time calculated by windingWriter
                                """
        return self.windingWriter.getPredictedTotalTime()

    def getPredictedFillFactor(self):
        """Returns calculated fill factor
                                """
        radius1 = (0.5 * self.statorDiameter) - (self.statorToothHeight - self.statorWindHeight)
        radius2 = radius1 - self.statorWindHeight
        rectangleArea = self.statorWindHeight * self.statorWindWidth
        windTotalArea = 3.14 * ((radius1 * radius1) - (radius2 * radius2))
        windLeftoverArea = windTotalArea - (rectangleArea * self.numberStatorTeeth)
        wireArea = 3.14 * (self.windingWriter.getWireDiameter() / 2) * (self.windingWriter.getWireDiameter() / 2) * (
                2 * self.numberWinds * self.numberStatorTeeth)
        fillFactor = 100 * (wireArea / windLeftoverArea)
        return fillFactor
        '''
        numberLayers = 2
        # Calculate max winds (number of layers times the number of winds per height)
        maxNumZWinds = self.windingWriter.getMaxNumZWinds()
        maxNumberWinds = numberLayers * maxNumZWinds
        # Fill factor is the ratio of number of winds completed over max number of winds possible
        fillFactor = 100 * (self.numberWinds / maxNumberWinds)
        return fillFactor
        '''

    def getPredictedResistance(self):
        distanceWound = self.windingWriter.getDistanceWoundPerTooth()
        print(distanceWound)
        print(self.mmToFeet)
        print(self.windingWriter.getWireResistance())
        predictedOhms = distanceWound * self.mmToFeet * self.windingWriter.getWireResistance()
        return predictedOhms

    def getPostWindingResult(self):
        # TODO: Implement to get stuff from winding Tester - Dan
        return 0

    def sendZeroCommand(self):
        # Send the command for the Arduino to zero the machine
        errorCode = self.windingReader.zeroMachine()
        return errorCode

    def getResistance(self):
        return self.windingTester.getResistance()

    def getCapacitance(self):
        return self.windingTester.getCapacitance()

    def getInductance(self):
        return self.windingTester.getInductance()

    def getPredictedElongation(self):
        return 0


'''
# Old main
# Returns the total predicted time in seconds given the distance traveled
def predictTime(totalMillimetersTraveled):
    timePerMillimeter = 0.01
    time = totalMillimetersTraveled * timePerMillimeter
    return time


def predictFillFactor(numberWinds, maxNumZWinds):
    numberLayers = 2
    # Calculate max winds (number of layers times the number of winds per height)
    maxNumberWinds = numberLayers * maxNumZWinds
    # Fill factor is the ratio of number of winds completed over max number of winds possible
    fillFactor = 100 * (numberWinds / maxNumberWinds)
    return fillFactor


def main():
    """Begin and run the coil winding program."""
    # --------------------- Variables --------------------- #
    arduinoMegaPort = "/dev/ttyACM0"
    arduinoMegaRate = "9600"

    arduinoReadyForCommand = "ready\n"

    sensorValue = None
    sensor = "getSensor1SensorValue\n"

    statorToothLength = None
    statorToothHeight = None
    statorWindHeight = None
    statorToothWidth = None
    statorShoe = None
    numberStatorTeeth = None
    numberWinds = None
    wireGauge = None
    wireMaterial = None
    distanceBetweenTeeth = None

    # ------------------- Declarations -------------------- #

    # Create serial connection for arduinos
    # arduinoMegaSerial = serial.Serial(arduinoMegaPort, arduinoMegaRate)
    # arduinoMegaSerial.flushInput()

    # ----------------------- Main ------------------------ #

    # Read and store user defined stator properties
    ui = UserInterface()
    ui.displayMessage("Starting program")
    ui.displayMessage("Please enter stator properties: ")

    statorToothLength = ui.getStatorToothLength()
    statorToothHeight = ui.getStatorToothHeight()
    statorWindHeight = ui.getStatorWindHeight()
    statorToothWidth = ui.getStatorToothWidth()
    statorShoeWidth = ui.getStatorShoeWidth()
    numberStatorTeeth = ui.getNumberStatorTeeth()
    numberWinds = ui.getNumberWinds()
    wireGauge = ui.getWireGauge()
    wireMaterial = ui.getWireMaterial()
    distanceBetweenTeeth = ui.getDistanceBetweenTeeth()

    windingWriter = WindingWriter(statorToothLength,
                                  statorToothHeight,
                                  statorWindHeight,
                                  statorToothWidth,
                                  statorShoeWidth,
                                  numberStatorTeeth,
                                  numberWinds,
                                  wireGauge,
                                  wireMaterial,
                                  distanceBetweenTeeth)
    # windingReader = WindingReader(arduinoMegaSerial)
    windingTester = WindingTester(statorToothLength,
                                  statorToothHeight,
                                  statorToothWidth,
                                  statorShoeWidth,
                                  numberStatorTeeth,
                                  numberWinds,
                                  wireGauge,
                                  wireMaterial,
                                  distanceBetweenTeeth)

    # TODO:
    #   * Generate path
    #   * Send path to winding reader
    #   * Zero machine with WindingReader -> function for zeroing w/ SensorReader
    #   * Send path with WindingReader
    #   * Post winding test with WindingTest()

    # Generate path
    ui.displayMessage("\nGenerating the path")
    windingWriter.generatePath("pathFile.txt")
    ui.displayMessage("Total mm: " + str(windingWriter.getTotalMillimetersTraveled()))
    ui.displayMessage("Predicted Time: " + str(windingWriter.getPredictedTotalTime()) + " secs")
    ui.displayMessage("Fill factor: " + str(predictFillFactor(numberWinds, windingWriter.getMaxNumZWinds())) + "%")
    # TODO: instantiate windingReader, giving it the file name of generated path

    # Zero machine with WindingReader -> function for zeroing w/ SensorReader
    ui.userAway(windingReader)

    # Send path with WindingReader
    ui.userStartWinding()
    ui.displayMessage("\nWinding")
    windingReader.sendPath("pathFile.txt")

    # Post winding test with WindingTest()
    ui.userStartPostWindingTest()
    ui.displayMessage("\nStarting post-winding test")
    windingTester.postWindingTest("postWindingTests.csv")

    ui.displayMessage("Program completed.")


# do stuff in main
main()
'''
