#! /usr/bin/python3
# ------------------------------------------------- #
# NCSU Senior Design: Automated Coil Winder
#
# WindingWriter.py
#    Constructs a new writer with the given parameters
#    and generates a path with those parameters
#
# Created: November 2019
# Last modified: November 2019
# ------------------------------------------------- #

class WindingWriter:
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

    # --------------------- Winding head values --------------------- #
    headClearanceX = 8.5
    headClearanceY = 15

    # --------------------- Position values --------------------- #
    startingCornerX = 462.5
    startingCornerY = 175
    currentCornerX = None
    currentCornerY = None
    currentCornerZ = None
    currentZ = None

    # --------------------- Calculated values --------------------- #
    ylength = None
    xlength = None
    zlength = None

    # --------------------- Functions --------------------- #
    def __init__(self, statorToothLength, statorToothHeight,
                 statorToothWidth, statorShoeWidth, numberStatorTeeth,
                 numberWinds, wireGauge, wireMaterial,
                 distanceBetweenTeeth):
        """Construct a new WindingWriter

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
        self.statorToothWidth = statorToothWidth
        self.statorShoeWidth = statorShoeWidth
        self.numberWinds = numberWinds
        self.wireGauge = wireGauge
        self.wireMaterial = wireMaterial
        self.distanceBetweenTeeth = distanceBetweenTeeth

        # Starting corner
        self.currentCornerX = int(self.startingCornerX - (0.5 * self.statorToothWidth) + (
                    0.5 * self.statorShoeWidth) + self.headClearanceX)
        self.currentCornerY = self.startingCornerY - self.headClearanceY

    def calculateValues(self):
        self.ylength = self.statorToothLength + (2 * self.headClearanceY)
        self.xlength = self.statorShoeWidth + int(2 * self.headClearanceX)
        print("xlength: "+ str(self.xlength))

        # Resolution is 0.05 mm
        if (self.wireGauge == "18"):
            self.zlength = 1.0
        elif (self.wireGauge == "17"):
            self.zlength = 1.15
        elif (self.wireGauge == "16"):
            self.zlength = 1.30
        elif (self.wireGauge == "15"):
            self.zlength = 1.45
        elif (self.wireGauge == "14"):
            self.zlength = 1.6
        elif (self.wireGauge == "13"):
            self.zlength = 1.8
        else:
            self.zlength = 1
        return

    def createPosts(self):
        return

    def windRect(self, pathFile):
        # Go forward in the y direction parameter ylength
        pathFile.write("G0 Y" + str(self.currentCornerY + self.ylength) + "\n")
        # Go forward in the x direction parameter xlength
        pathFile.write("G0 X" + str(self.currentCornerX + self.xlength) + "\n")
        # Go backwards in the ydirection ylength
        pathFile.write("G0 Y" + str(self.currentCornerY) + "\n")
        # Go backwards in the xdirection xlength
        pathFile.write("G0 X" + str(self.currentCornerX) + "\n")

        return

    def generatePath(self, fileName):
        """Generate a path with the stored parameters
            and return True when complete
        """

        # TODO (485): Generate a path from the given parameters
        # Open the path file for writing
        pathFile = open(fileName, "w")
        # % for Telling arduino this is gcode
        pathFile.write("%\n")

        # --------------------- Path generation --------------------- #

        # Move diagonally to start of block
        pathFile.write("G0 X" + str(self.currentCornerX) + " Y" + str(self.currentCornerY) + "\n")

        self.calculateValues()
        self.windRect(pathFile)

        # % for Telling arduino this gcode is done
        pathFile.write("%\n")
        # Close opened path file
        pathFile.close()

        """
        # Generate a basic square path for winding post
        # Open the path file for writing
        pathFile = open(fileName, "w")
        
        
        #% for Telling arduino this is gcode
        pathFile.write("%\n")

        #Move z axis up
        pathFile.write("G0 Z35\n")
        #Move diagonally to start of block
        pathFile.write( "G0 X465 Y160\n")

        pathFile.write("G0 Z0\n")
        #Move one full rotation
        pathFile.write("G0 Y320\n")
        pathFile.write("G0 X500\n")
        pathFile.write("G0 Y160\n")
        pathFile.write("G0 X465\n")

        pathFile.write("G0 Z3\n")
        #Move one full rotation
        pathFile.write("G0 Y320\n")
        pathFile.write("G0 X500\n")
        pathFile.write("G0 Y160\n")
        pathFile.write("G0 X465\n")

        pathFile.write("G0 Z6\n")
        #Move one full rotation
        pathFile.write("G0 Y320\n")
        pathFile.write("G0 X500\n")
        pathFile.write("G0 Y160\n")
        pathFile.write("G0 X465\n")

        #Move back to zero
        pathFile.write("G0 Z35\n")
        pathFile.write("G0 X0 Y0\n")
        pathFile.write("G0 Z0\n")

        #% for Telling arduino this gcode is done
        pathFile.write("%\n")

        # Close opened path file
        pathFile.close()
        """

        return True
