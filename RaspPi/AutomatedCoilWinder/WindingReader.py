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
    def __init__(self):
        """Construct a new WindingReader

        Keyword arguments:
        serialConnection -- the serial connection to read from

        """
        # self.serialConnection = arduinoSerial

    def sendPath(self, fileName):
        """Sends a gcode path from a text file to the given serial conenction
        """
        # --------------------- Variables --------------------- #
        # exampleParam = None

        # TODO: Send the path stored in the given file name
        # Open the path file for reading and sending
        pathFile = open(fileName, "r")

        # TODO: Store each line in a string and send that string to the arduino
        #       with a newline char at end (readline)
        #       When reach end of file, close out

        while True:
            # read next line
            line = pathFile.readline()

            # If the next line is the end of the file, break
            if line == "":
                break

            print(line)
            '''
            # send next line to arduino if ready
            readyReceived = False
            while not readyReceived:
                if self.serialConnection.inWaiting() > 0:
                    inputValue = self.serialConnection.readline().decode()
                    print("in: " + inputValue)
                    if inputValue == self.arduinoReadyForCommand:
                            # Give Gcode command
                            print("ReadyReceived\n")
                            readyReceived = True
                            self.serialConnection.write(line.encode())
            '''

        # Close opened path file
        pathFile.close()
        print("Done reading gcode")

    def zeroMachine(self):
        """Zeros the machine
        """
        # --------------------- Variables --------------------- #
        # exampleParam = None

        # TODO: Send the path to zero the machine