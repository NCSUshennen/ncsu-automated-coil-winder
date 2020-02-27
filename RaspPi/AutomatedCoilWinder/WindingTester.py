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

    resistance = 0
    capacitance = 0
    inductance = 0

    # --------------------- Functions --------------------- #
    def __init__(self, statorToothLength, statorToothHeight,
                 statorToothWidth, statorShoeWidth, numberStatorTeeth,
                 numberWinds, wireGauge, wireMaterial,
                 distanceBetweenTeeth):
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

    def postWindingTest(self, fileName):
        """Generate a path with the stored parameters
            and return True when complete
        """
        # --------------------- Variables --------------------- #
        # exampleParam = None

        # TODO: ask if ardiuno is ready for command, then send ppst Winding Command
        myData = [["Stator Tooth Length: ", self.statorToothLength],
          ["Stator Tooth Height: ", self.statorToothHeight],
          ["Stator Tooth Width: ", self.statorToothWidth],
          ["Stator Shoe Width: ", self.statorShoeWidth],
          ["Number of Winds: ", self.numberWinds],
          ["Wire Gauge: ", self.wireGauge],
          ["Wire Material: ", self.wireMaterial],
          ["Distance Between Teeth: ", self.distanceBetweenTeeth]]


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
