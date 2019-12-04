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

        #Move z axis up
        pathFile.write("G0 Z30\n")
        #Move diagonally to start of block
        pathFile.write( "G0 X475 Y185\n")

        #Move one full rotation
        pathFile.write("G0 Y325\n")
        pathFile.write("G0 X503\n")
        pathFile.write("G0 Y185\n")
        pathFile.write("G0 X475\n")

        #Move back to zero
        pathFile.write("G0 X0 Y0\n")
        pathFile.write("G0 Z0\n")

        ''' Below is for winding three times -> test with above code first
        #Move down 5mm
        pathFile.write("G0 Z19\n")

        #Move one full rotation
        pathFile.write("G0 Y325\n")
        pathFile.write("G0 X503\n")
        pathFile.write("G0 Y185\n")
        pathFile.write("G0 X475\n")

        #Move down 5mm
        pathFile.write("G0 Z14\n")

        #Move one full rotation
        pathFile.write("G0 Y325\n")
        pathFile.write("G0 X503\n")
        pathFile.write("G0 Y185\n")
        pathFile.write("G0 X475\n")

        #Move back to zero
        pathFile.write("G0 X0 Y0\n")
        pathFile.write("G0 Z0\n")
        '''

        #% for Telling arduino this gcode is done
        pathFile.write("%\n")

        # Close opened path file
        pathFile.close()

        return True