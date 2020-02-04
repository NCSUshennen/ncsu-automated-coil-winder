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
    # TODO: update with actual head clearances later
    headClearanceX = 8.5
    headClearanceY = 15
    postHeadClearance = 5

    # --------------------- Position values --------------------- #
    # TODO: update with actual starting values later
    startingCornerX = 462.5
    startingCornerY = 175
    currentCornerX = None
    currentCornerY = None
    currentCornerZ = None
    currentZ = None
    postYValue = 20
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
    postWindDistance = None

    # --------------------- Misc. Post values --------------------- #
    postDiameter = 10
    currentPostCenterX = None

    currentPostUpperLeftX = None
    currentPostUpperLeftY = None

    currentPostUpperRightX = None
    currentPostUpperRightY = None

    currentPostLowerRightX = None
    currentPostLowerRightY = None

    currentPostLowerLeftX = None
    currentPostLowerLeftY = None

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

        # post values
        self.postWindDistance = (0.5 * self.postDiameter) + self.postHeadClearance

        return

    def createPosts(self):
        # TODO: update with actual post values later
        self.postXValues = [462, 478, 494, 510, 526, 542, 558, 574, 590, 606, 622, 638, 654, 670, 686, 702, 718, 734,
                            750, 766, 782, 798]
        self.currentPost = 0
        return

    def switchZDirection(self):
        if (self.zDirection == "up"):
            self.zDirection = "down"
        else:
            self.zDirection = "up"
        return

    def windNextPost(self, pathFile):
        self.currentPostCenterX = self.postXValues[self.currentPost]

        # Post winding rectangle corner values
        self.currentPostUpperLeftX = self.postWindDistance + self.currentPostCenterX
        self.currentPostUpperLeftY = self.postWindDistance + self.postYValue

        self.currentPostUpperRightX = self.currentPostCenterX - self.postWindDistance
        self.currentPostUpperRightY = self.postWindDistance + self.postYValue

        self.currentPostLowerRightX = self.currentPostCenterX - self.postWindDistance
        self.currentPostLowerRightY = self.postYValue - self.postWindDistance

        self.currentPostLowerLeftX = self.postWindDistance + self.currentPostCenterX
        self.currentPostLowerLeftY = self.postYValue - self.postWindDistance

        # Go to upper left corner of next post
        pathFile.write("G0 Y" + str(self.currentPostUpperLeftY) + "\n")
        pathFile.write("G0 X" + str(self.currentPostUpperLeftX) + "\n")

        # Go down to correct z
        pathFile.write("G0 Z" + str(self.currentCornerZ) + "\n")

        # Wind once
        pathFile.write("G0 X" + str(self.currentPostUpperRightX) + "\n")
        pathFile.write("G0 Y" + str(self.currentPostLowerRightY) + "\n")
        pathFile.write("G0 X" + str(self.currentPostLowerLeftX) + "\n")
        pathFile.write("G0 Y" + str(self.currentPostUpperLeftY) + "\n")

        # Go up a wireGauge length
        pathFile.write("G0 Z" + str(self.currentCornerZ + self.wireDiameter) + "\n")

        # Wind once again
        pathFile.write("G0 X" + str(self.currentPostUpperRightX) + "\n")
        pathFile.write("G0 Y" + str(self.currentPostLowerRightY) + "\n")
        pathFile.write("G0 X" + str(self.currentPostLowerLeftX) + "\n")
        pathFile.write("G0 Y" + str(self.currentPostUpperLeftY) + "\n")

        # Increase the post we are currently on
        self.currentPost += 1
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

        # Calculate necessary values
        self.calculateValues()
        self.createPosts()

        # Wind first post
        self.windNextPost(pathFile)

        # Move diagonally to start of block
        pathFile.write("G0 X" + str(self.currentCornerX) + " Y" + str(self.currentCornerY) + "\n")

        # Keep winding until you've wound the last tooth
        while self.numTeethWound < self.numberStatorTeeth:
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

            # Wind next post
            self.windNextPost(pathFile)

            # Set/go to new current corner
            if self.numTeethWound <= self.numberStatorTeeth:
                self.currentCornerX += self.distanceBetweenTeeth + self.statorToothWidth
                pathFile.write("G0 X" + str(self.currentCornerX) + " Y" + str(self.currentCornerY) + "\n")
                self.currentZ = self.currentCornerZ
                # pathFile.write("G0 Z" + str(self.currentCornerZ) + "\n")

        # TODO: Wind the last post and zero without hitting posts
        pathFile.write("G0 X15.0\n")
        pathFile.write("G0 Y0.0\n")
        pathFile.write("G0 X0.0\n")
        pathFile.write("G0 Z0.0\n")

        # % for Telling arduino this gcode is done
        pathFile.write("%\n")
        # Close opened path file
        pathFile.close()

        return True
