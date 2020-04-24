#! /usr/bin/python3
# ------------------------------------------------- #
# NCSU Senior Design: Automated Coil Winder
#
# WindingTester.py
#    Constructs a new tester, goes through the post
#    winding tests, and stores results to CSV file
#
# Created: November 2019
# Last modified: November 2019
# ------------------------------------------------- #

# imports
import serial
import csv

class WindingTester:
    # --------------------- Variables --------------------- #
    statorToothLength = None
    statorToothHeight = None
    statorToothWidth = None
    statorShoeWidth = None
    numberStatorTeeth = None
    numberWinds = None
    wireGauge = None
    wireMaterial = None
    distanceBetweenTeeth = None
    serialConnection = None

    resistance = "0 Ohms"
    capacitance = "0 mH"
    inductance = "0 uF"

    arduinoReadyAsk = "isArduinoReady\n"
    arduinoReadyForCommand = "ready\n"

    # --------------------- Functions --------------------- #
    def __init__(self, statorToothLength, statorToothHeight,
                 statorToothWidth, statorShoeWidth, numberStatorTeeth,
                 numberWinds, wireGauge, wireMaterial,
                 distanceBetweenTeeth, arduinoSerial):
        """Construct a new WindingTester

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
        fileName --
        """
        self.statorToothLength = statorToothLength
        self.statorToothHeight = statorToothHeight
        self.statorToothWidth = statorToothWidth
        self.statorShoeWidth = statorShoeWidth
        self.numberWinds = numberWinds
        self.wireGauge = wireGauge
        self.wireMaterial = wireMaterial
        self.distanceBetweenTeeth = distanceBetweenTeeth
        self.serialConnection = arduinoSerial

    def postWindingTest(self, fileName):
        """Generate a path with the stored parameters
            and return True when complete
        """
        # --------------------- Variables --------------------- #
        # exampleParam = None

        postWindingTestCommand = "doPostWindingTest\n"

        # Ask if ardiuno is ready for command, then send command to Arduino
        self.serialConnection.write(self.arduinoReadyAsk.encode())
        readyReceived = False
        while not readyReceived:
            if self.serialConnection.inWaiting() > 0:
                inputValue = ""
                inputValue = self.serialConnection.readline().decode()

                if inputValue == self.arduinoReadyForCommand:
                    self.serialConnection.write(postWindingTestCommand.encode())
                    readyReceived = True

        # Check the feedback for resistance, inductance, and capacitance values
        resistanceReceived = False
        while not resistanceReceived:
            if self.serialConnection.inWaiting() > 0:
                self.resistance = self.serialConnection.readline().decode()
                resistanceReceived = True

        inductanceReceived = False
        while not inductanceReceived:
            if self.serialConnection.inWaiting() > 0:
                self.inductance = self.serialConnection.readline().decode()
                inductanceReceived = True

        capacitanceReceived = False
        while not capacitanceReceived:
            if self.serialConnection.inWaiting() > 0:
                self.capacitance = self.serialConnection.readline().decode()
                capacitanceReceived = True

        myData = [["Stator Tooth Length: ", self.statorToothLength],
          ["Stator Tooth Height: ", self.statorToothHeight],
          ["Stator Tooth Width: ", self.statorToothWidth],
          ["Stator Shoe Width: ", self.statorShoeWidth],
          ["Number of Winds: ", self.numberWinds],
          ["Wire Gauge: ", self.wireGauge],
          ["Wire Material: ", self.wireMaterial],
          ["Distance Between Teeth: ", self.distanceBetweenTeeth],
          ["Resistance: ", self.resistance],
          ["Inductance: ", self.inductance],
          ["Capacitance: ", self.capacitance]]


        testsFile = open(fileName, "w")
        with testsFile:
            writer = csv.writer(testsFile)
            writer.writerows(myData)

        # Close opened test file
        testsFile.close()

        print("Post winding writing complete")

    def getResistance(self):
        return self.resistance

    def getCapacitance(self):
        return self.capacitance

    def getInductance(self):
        return self.inductance