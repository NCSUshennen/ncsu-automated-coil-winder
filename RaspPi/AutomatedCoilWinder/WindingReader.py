#! /usr/bin/python3
# ------------------------------------------------- #
# NCSU Senior Design: Automated Coil Winder
#
# WindingReader.py
#    Constructs a new winding reader and functions for
#    zeroing the machine and sending a path from a
#    text file
#
# Created: November 2019
# Last modified: November 2019
# ------------------------------------------------- #

class WindingReader:
    # --------------------- Variables --------------------- #
    serialConnection = None
    arduinoReadyForCommand = "ready\n"

    # --------------------- Functions --------------------- #
    def __init__(self, arduinoSerial):
        """Construct a new WindingReader

        Keyword arguments:
        serialConnection -- the serial connection to read from

        """
        self.serialConnection = arduinoSerial


    def sendPath(self):
        """Sends a gcode path from a text file to the given serial conenction
        """
        # --------------------- Variables --------------------- #
        # exampleParam = None

        # TODO: Send the path stored in the given file name

    def zeroMachine(self):
        """Zeros the machine
        """
        # --------------------- Variables --------------------- #
        # exampleParam = None

        # TODO: Send the path to zero the machine