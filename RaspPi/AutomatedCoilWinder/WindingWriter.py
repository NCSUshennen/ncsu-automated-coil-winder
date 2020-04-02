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
    statorWindWidth = None

    # --------------------- Distance traveled/Time measurement values --------------------- #
    totalMillimetersTraveled = None
    totalTimeTaken = None
    xRateTime = 0.0095
    xOffsetTime = 0.185
    yRateTime = 0.008
    yOffsetTime = 0.268
    zRateTime = 0.0058
    zOffsetTime = 0.485
    prevX = None;
    prevY = None;
    prevZ = None;

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
    wireResistance = None  # in Ohms/1000ft
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
    # --------------------- Constructor --------------------- #
    def __init__(self, statorToothLength, statorToothHeight, statorWindHeight,
                 statorToothWidth, statorShoeWidth, numberStatorTeeth,
                 numberWinds, wireGauge, wireMaterial,
                 distanceBetweenTeeth, statorWindWidth):
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
        self.statorWindWidth = statorWindWidth

        # Starting corner
        self.currentCornerX = float(self.startingCornerX - (0.5 * self.statorToothWidth) + (
                0.5 * self.statorShoeWidth) + self.headClearanceX)
        self.currentCornerY = float(self.startingCornerY - self.headClearanceY)

        # Initialize all distance parameters to 0
        self.totalMillimetersTraveled = 0
        self.totalTimeTaken = 0
        self.prevX = 0
        self.prevY = 0
        self.prevZ = 0

    # --------------------- Distance counting --------------------- #
    def calculateDistanceTraveled(self, newX, newY, newZ):
        xDistance = abs(self.prevX - newX)
        yDistance = abs(self.prevY - newY)
        zDistance = abs(self.prevZ - newZ)
        self.totalMillimetersTraveled += xDistance + yDistance + zDistance
        if xDistance != 0:
            self.totalTimeTaken += (self.xRateTime * xDistance) + self.xOffsetTime
        if yDistance != 0:
            self.totalTimeTaken += (self.yRateTime * yDistance) + self.yOffsetTime
        if zDistance != 0:
            self.totalTimeTaken += (self.zRateTime * zDistance) + self.zOffsetTime

        # Set previous values to current values for next reading
        self.prevX = newX
        self.prevY = newY
        self.prevZ = newZ
        return

    def getPredictedTotalTime(self):
        return self.totalTimeTaken

    def getTotalMillimetersTraveled(self):
        return self.totalMillimetersTraveled

    def getMaxNumZWinds(self):
        return self.maxNumZWinds

    def getWireDiameter(self):
        return self.wireDiameter

    def getWireResistance(self):
        return self.wireResistance

    def getDistanceWoundPerTooth(self):
        return self.numberWinds * (2 * (self.statorToothLength + self.wireDiameter) + 2 * (self.statorWindWidth))

    # --------------------- Path generation --------------------- #

    def calculateValues(self):
        self.ylength = float(self.statorToothLength + (2 * self.headClearanceY))
        self.xlength = float(self.statorShoeWidth + float(2 * self.headClearanceX))

        # Resolution is 0.05 mm
        if self.wireGauge == 18.0:
            print("Wire gauge 18")
            self.wireDiameter = 1.0
            self.wireResistance = 6.385
        elif self.wireGauge == 17.0:
            self.wireDiameter = 1.15
            self.wireResistance = 5.064
        elif self.wireGauge == 16.0:
            self.wireDiameter = 1.30
            self.wireResistance = 4.016
        elif self.wireGauge == 15.0:
            self.wireDiameter = 1.45
            self.wireResistance = 3.184
        elif self.wireGauge == 14.0:
            self.wireDiameter = 1.6
            self.wireResistance = 2.525
        elif self.wireGauge == 13.0:
            self.wireDiameter = 1.8
            self.wireResistance = 2.003
        else:
            print("Wire gauge unknown")
            print(self.wireGauge)
            self.wireDiameter = 1
            self.wireResistance = 1

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
                            750, 766, 782, 798, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800,
                            800, 800, 800, 800, 800, 800]
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
        self.calculateDistanceTraveled(self.prevX, self.currentPostUpperLeftY, self.prevZ)
        pathFile.write("G0 X" + str(self.currentPostUpperLeftX) + "\n")
        self.calculateDistanceTraveled(self.currentPostUpperLeftX, self.prevY, self.prevZ)

        # Go down to correct z
        pathFile.write("G0 Z" + str(self.currentCornerZ) + "\n")
        self.calculateDistanceTraveled(self.prevX, self.prevY, self.currentCornerZ)

        # Wind once
        pathFile.write("G0 X" + str(self.currentPostUpperRightX) + "\n")
        self.calculateDistanceTraveled(self.currentPostUpperRightX, self.prevY, self.prevZ)
        pathFile.write("G0 Y" + str(self.currentPostLowerRightY) + "\n")
        self.calculateDistanceTraveled(self.prevX, self.currentPostLowerRightY, self.prevZ)
        pathFile.write("G0 X" + str(self.currentPostLowerLeftX) + "\n")
        self.calculateDistanceTraveled(self.currentPostLowerLeftX, self.prevY, self.prevZ)
        pathFile.write("G0 Y" + str(self.currentPostUpperLeftY) + "\n")
        self.calculateDistanceTraveled(self.prevX, self.currentPostUpperLeftY, self.prevZ)

        # Go up a wireGauge length
        pathFile.write("G0 Z" + str(self.currentCornerZ + self.wireDiameter) + "\n")
        self.calculateDistanceTraveled(self.prevX, self.prevY, self.currentCornerZ + self.wireDiameter)

        # Wind once again
        pathFile.write("G0 X" + str(self.currentPostUpperRightX) + "\n")
        self.calculateDistanceTraveled(self.currentPostUpperRightX, self.prevY, self.prevZ)
        pathFile.write("G0 Y" + str(self.currentPostLowerRightY) + "\n")
        self.calculateDistanceTraveled(self.prevX, self.currentPostLowerRightY, self.prevZ)
        pathFile.write("G0 X" + str(self.currentPostLowerLeftX) + "\n")
        self.calculateDistanceTraveled(self.currentPostLowerLeftX, self.prevY, self.prevZ)
        pathFile.write("G0 Y" + str(self.currentPostUpperLeftY) + "\n")
        self.calculateDistanceTraveled(self.prevX, self.currentPostUpperLeftY, self.prevZ)

        # Increase the post we are currently on
        self.currentPost += 1
        return

    def windRect(self, pathFile):
        # Go forward in the y direction parameter ylength
        pathFile.write("(StartEncode)\n")
        pathFile.write("G0 Y" + str(self.currentCornerY + self.ylength) + "\n")
        self.calculateDistanceTraveled(self.prevX, self.currentCornerY + self.ylength, self.prevZ)
        # Go forward in the x direction parameter xlength
        pathFile.write("G0 X" + str(self.currentCornerX + self.xlength) + "\n")
        self.calculateDistanceTraveled(self.currentCornerX + self.xlength, self.prevY, self.prevZ)
        # Go backwards in the ydirection ylength
        pathFile.write("G0 Y" + str(self.currentCornerY) + "\n")
        self.calculateDistanceTraveled(self.prevX, self.currentCornerY, self.prevZ)
        # Go backwards in the xdirection xlength
        pathFile.write("G0 X" + str(self.currentCornerX) + "\n")
        self.calculateDistanceTraveled(self.currentCornerX, self.prevY, self.prevZ)
        pathFile.write("(StopEncode)\n")
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
        self.calculateDistanceTraveled(self.currentCornerX, self.currentCornerY, self.prevZ)

        # Keep winding until you've wound the last tooth
        while self.numTeethWound < self.numberStatorTeeth:
            # Initial/reset values
            self.numTimesWound = 0
            self.numZWinds = 0
            self.zDirection = "up"

            while self.numTimesWound < self.numberWinds:
                # Go to next current Z
                pathFile.write("G0 Z" + str(self.currentZ) + "\n")
                self.calculateDistanceTraveled(self.prevX, self.prevY, self.currentZ)

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
                self.calculateDistanceTraveled(self.currentCornerX, self.currentCornerY, self.prevZ)
                self.currentZ = self.currentCornerZ
                # pathFile.write("G0 Z" + str(self.currentCornerZ) + "\n")

        # TODO: Wind the last post and zero without hitting posts
        pathFile.write("G0 X15.0\n")
        self.calculateDistanceTraveled(15, self.prevY, self.prevZ)
        pathFile.write("G0 Y0.0\n")
        self.calculateDistanceTraveled(self.prevX, 0, self.prevZ)
        pathFile.write("G0 X0.0\n")
        self.calculateDistanceTraveled(0, self.prevY, self.prevZ)
        pathFile.write("G0 Z0.0\n")
        self.calculateDistanceTraveled(self.prevX, self.prevY, 0)

        # % for Telling arduino this gcode is done
        pathFile.write("%\n")
        # Close opened path file
        pathFile.close()

        return True
