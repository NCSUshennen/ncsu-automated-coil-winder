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
    numberTurns = None
    wireGauge = None
    wireMaterial = None
    distanceBetweenTeeth = None

    # --------------------- Functions --------------------- #
    def __init__(self, statorToothLength, statorToothHeight,
                 statorToothWidth, statorShoeWidth, numberStatorTeeth,
                 numberTurns, wireGauge, wireMaterial,
                 distanceBetweenTeeth):
        """Construct a new WindingWriter

        Keyword arguments:
        statorToothLength --
        statorToothHeight --
        statorToothWidth --
        statorShoeWidth --
        numberStatorTeeth --
        numberTurns --
        wireGauge --
        wireMaterial --
        distanceBetweenTeeth --
        """
        self.statorToothLength = statorToothLength
        self.statorToothHeight = statorToothHeight
        self.statorToothWidth = statorToothWidth
        self.statorShoeWidth = statorShoeWidth
        self.numberTurns = numberTurns
        self.wireGauge = wireGauge
        self.wireMaterial = wireMaterial
        self.distanceBetweenTeeth = distanceBetweenTeeth

    def generatePath(self, fileName):
        """Generate a path with the stored parameters
            and return True when complete
        """
        # --------------------- Variables --------------------- #
        # exampleParam = None

        # TODO (485): Generate a path from the given parameters

        # TODO: Generate a basic square path for winding post
        # Open the path file for writing
        pathFile = open(fileName, "w")

        #% for Telling arduino this is gcode
        pathFile.write("%\n")

        #Move diagonally to start of block
        pathFile.write( "G0 X100 Y100\n")

        #Move one full rotation
        pathFile.write("G0 Y225\n")
        pathFile.write("G0 X175\n")
        pathFile.write("G0 Y100\n")
        pathFile.write("G0 X100\n")

        #Move up half a cm
        pathFile.write("G0 Z5\n")

        #Move one full rotation
        pathFile.write("G0 Y225\n")
        pathFile.write("G0 X175\n")
        pathFile.write("G0 Y100\n")
        pathFile.write("G0 X100\n")

        #Move one full rotation
        pathFile.write("G0 Y225\n")
        pathFile.write("G0 X175\n")
        pathFile.write("G0 Y100\n")
        pathFile.write("G0 X100\n")

        #Move down half a cm
        pathFile.write("G0 Z0\n")

        #Move one full rotation
        pathFile.write("G0 Y225\n")
        pathFile.write("G0 X175\n")
        pathFile.write("G0 Y100\n")
        pathFile.write("G0 X100\n")

        #Move back to zero
        pathFile.write( "G0 X0 Y0 Z0\n")

        #% for Telling arduino this gode is done
        pathFile.write("%\n")

        # Close opened path file
        pathFile.close()

        return True