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
    # --------------------- Entered values --------------------- #
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
    postYValue = 10
    postXValues = None
    currentPost = None

    # --------------------- Tracker values --------------------- #
    numTeethWound = None
    numTimesWound = None
    numZWinds = None
    zDirection = None  # "up" or "down"

    # --------------------- Calculated values --------------------- #
    ylength = None
    xlength = None
    wireDiameter = None
    maxNumZWinds = None

    # --------------------- Functions --------------------- #
    def __init__(self, statorToothLength, statorToothHeight, statorWindHeight,
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
        self.statorWindHeight = statorWindHeight
        self.statorToothWidth = statorToothWidth
        self.statorShoeWidth = statorShoeWidth
        self.numberStatorTeeth = numberStatorTeeth
        self.numberWinds = numberWinds
        self.wireGauge = wireGauge
        self.wireMaterial = wireMaterial
        self.distanceBetweenTeeth = distanceBetweenTeeth

        # Starting corner
        self.currentCornerX = float(self.startingCornerX - (0.5 * self.statorToothWidth) + (
                0.5 * self.statorShoeWidth) + self.headClearanceX)
        self.currentCornerY = float(self.startingCornerY - self.headClearanceY)

    def calculateValues(self):
        self.ylength = float(self.statorToothLength + (2 * self.headClearanceY))
        self.xlength = float(self.statorShoeWidth + float(2 * self.headClearanceX))

        # Resolution is 0.05 mm
        if self.wireGauge == "18":
            self.wireDiameter = 1.0
        elif self.wireGauge == "17":
            self.wireDiameter = 1.15
        elif self.wireGauge == "16":
            self.wireDiameter = 1.30
        elif self.wireGauge == "15":
            self.wireDiameter = 1.45
        elif self.wireGauge == "14":
            self.wireDiameter = 1.6
        elif self.wireGauge == "13":
            self.wireDiameter = 1.8
        else:
            self.wireDiameter = 1

        # Number of times to wind in the z
        self.currentZ = float(self.statorWindHeight + (self.wireDiameter * 0.5))
        self.currentCornerZ = self.currentZ
        self.maxNumZWinds = float((self.statorToothHeight - self.statorWindHeight) / self.wireDiameter)
        return

    def createPosts(self):
        self.postXValues = [175, 275]
        self.currentPost = 0
        return

    def switchZDirection(self):
        if (self.zDirection == "up"):
            self.zDirection = "down"
        else:
            self.zDirection = "up"
        return

    def windNextPost(self):
        # self.currentPost += 1
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

        # Open the path file for writing
        pathFile = open(fileName, "w")
        # % for Telling arduino this is gcode
        pathFile.write("%\n")

        # --------------------- Path generation --------------------- #

        # Initial values
        self.numTimesWound = 0
        self.numZWinds = 0
        self.numTeethWound = 0
        self.zDirection = "up"

        # TODO: Wind first post
        self.windNextPost()

        # Move diagonally to start of block
        pathFile.write("G0 X" + str(self.currentCornerX) + " Y" + str(self.currentCornerY) + "\n")

        # Calculate necessary values
        self.calculateValues()

        # Keep winding until you've wound the last tooth
        while self.numTeethWound <= self.numberStatorTeeth:
            # Initial/reset values
            self.numTimesWound = 0
            self.numZWinds = 0
            self.zDirection = "up"

            while self.numTimesWound < self.numberWinds:
                # Go to next current Z
                pathFile.write("G0 Z" + str(self.currentZ) + "\n")

                # Wind the next rectangle
                self.windRect(pathFile)

                if self.numZWinds >= self.maxNumZWinds - 1:
                    self.switchZDirection()
                    self.numZWinds = 0
                else:
                    # Move current Z in the right direction one wire length
                    if self.zDirection == "up":
                        self.currentZ += self.wireDiameter
                    else:
                        self.currentZ -= self.wireDiameter
                    self.numZWinds += 1
                # Increase number of times we've wound this tooth
                self.numTimesWound += 1

            # Increase the number of teeth wound
            self.numTeethWound += 1

            # TODO: Wind next post and set new current corner
            if self.numTeethWound <= self.numberStatorTeeth:
                self.currentCornerX += self.distanceBetweenTeeth + self.statorToothWidth
                pathFile.write("G0 X" + str(self.currentCornerX) + " Y" + str(self.currentCornerY) + "\n")
                self.currentZ = self.currentCornerZ
                # pathFile.write("G0 Z" + str(self.currentCornerZ) + "\n")

        # TODO: Wind the last post and zero without hitting posts
        pathFile.write("G0 X15\n")
        pathFile.write("G0 Y0\n")
        pathFile.write("G0 X0\n")
        pathFile.write("G0 Z0\n")

        # % for Telling arduino this gcode is done
        pathFile.write("%\n")
        # Close opened path file
        pathFile.close()

        return True
