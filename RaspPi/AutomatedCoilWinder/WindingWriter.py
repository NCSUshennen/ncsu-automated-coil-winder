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
    statorLipRadius = None
    numberStatorTeeth = None
    numberWireLayers = None
    wireGauge = None
    wireMaterial = None
    distanceBetweenTeeth = None

    # --------------------- Functions --------------------- #
    def __init__(self, statorToothLength, statorToothHeight,
                 statorToothWidth, statorLipRadius, numberWireLayers,
                 wireGauge, wireMaterial, distanceBetweenTeeth):
        """Construct a new WindingWriter

        Keyword arguments:
        statorToothLength --
        statorToothHeight --
        statorToothWidth --
        statorLipRadius --
        numberStatorTeeth --
        numberWireLayers --
        wireGauge --
        wireMaterial --
        distanceBetweenTeeth --
        """
        self.statorToothLength = statorToothLength
        self.statorToothHeight = statorToothHeight
        self.statorToothWidth = statorToothWidth
        self.statorLipRadius = statorLipRadius
        self.numberWireLayers = numberWireLayers
        self.wireGauge = wireGauge
        self.wireMaterial = wireMaterial
        self.distanceBetweenTeeth = distanceBetweenTeeth

    def generatePath(self):
        """Generate a path with the stored parameters
            and return True when complete
        """
        # --------------------- Variables --------------------- #
        # exampleParam = None

        # TODO: Generate the path

        return True