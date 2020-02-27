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

# imports
import serial


class WindingReader:
    # --------------------- Variables --------------------- #
    serialConnection = None
    arduinoReadyForCommand = "ready\n"

    # Error codes here
    arduinoErrorOverPosition = "ErrorHitOverPositionSwitch\n"

    arduinoErrorZeroSwitchX = "ErrorHitXZeroingSwitch\n"
    arduinoErrorZeroSwitchY = "ErrorHitYZeroingSwitch\n"
    arduinoErrorZeroSwitchZ = "ErrorHitZZeroingSwitch\n"

    arduinoErrorOutOfBounds = "ErrorDestinationOutOfBounds\n"

    arduinoErrorX1OverCurrent = "ErrorAlarmX1OverCurrent\n"
    arduinoErrorX1VoltageRef = "ErrorAlarmX1VoltageReference\n"
    arduinoErrorX1Param = "ErrorAlarmX1Parameters\n"
    arduinoErrorX1OverVolt = "ErrorAlarmX1OverVoltage\n"
    arduinoErrorX1OverPosition = "ErrorAlarmX1OverPosition\n"

    arduinoErrorX2OverCurrent = "ErrorAlarmX2OverCurrent\n"
    arduinoErrorX2VoltageRef = "ErrorAlarmX2VoltageReference\n"
    arduinoErrorX2Param = "ErrorAlarmX2Parameters\n"
    arduinoErrorX2OverVolt = "ErrorAlarmX2OverVoltage\n"
    arduinoErrorX2OverPosition = "ErrorAlarmX2OverPosition\n"

    arduinoErrorYOverCurrent = "ErrorAlarmYOverCurrent\n"
    arduinoErrorYVoltageRef = "ErrorAlarmYVoltageReference\n"
    arduinoErrorYParam = "ErrorAlarmYParameters\n"
    arduinoErrorYOverVolt = "ErrorAlarmYOverVoltage\n"
    arduinoErrorYOverPosition = "ErrorAlarmYOverPosition\n"

    arduinoErrorZOverCurrent = "ErrorAlarmZOverCurrent\n"
    arduinoErrorZVoltageRef = "ErrorAlarmZVoltageReference\n"
    arduinoErrorZParam = "ErrorAlarmZParameters\n"
    arduinoErrorZOverVolt = "ErrorAlarmZOverVoltage\n"
    arduinoErrorZOverPosition = "ErrorAlarmZOverPosition\n"

    arduinoErrorFailedZeroSwitchX = "ErrorFailedToHitXZeroingSwitch\n" # Specifically for zeroing circuit
    arduinoErrorFailedZeroSwitchY = "ErrorFailedToHitYZeroingSwitch\n"
    arduinoErrorFailedZeroSwitchZ = "ErrorFailedToHitZZeroingSwitch\n"


    # --------------------- Functions --------------------- #
    def __init__(self, arduinoSerial):
        """Construct a new WindingReader

        Keyword arguments:
        serialConnection -- the serial connection to read from

        """
        self.serialConnection = arduinoSerial

    def sendPath(self, fileName):
        """Sends a gcode path from a text file to the given serial conenction
        """
        # --------------------- Variables --------------------- #
        windingPathCmd = "beginWindingPath\n"

        # Send the path stored in the given file name
        # Open the path file for reading and sending
        pathFile = open(fileName, "r")

        # Store each line in a string and send that string to the arduino
        #       with a newline char at end (readline)
        #       When reach end of file, close out

        # Ask if arduino is ready for command, then send winding command
        self.serialConnection.write(windingPathCmd.encode())
        while True:
            # read next line
            line = pathFile.readline()

            # If the next line is the end of the file, break
            if line == "":
                break

            # send next line to arduino if ready
            readyReceived = False
            while not readyReceived:
                if self.serialConnection.inWaiting() > 0:
                    inputValue = ""
                    inputValue = self.serialConnection.readline().decode()
                    print("in: " + inputValue)

                    if inputValue == self.arduinoReadyForCommand:
                        # Give Gcode command
                        print("ReadyReceived\n")
                        print(line)
                        self.serialConnection.write(line.encode())
                        readyReceived = True
                    elif inputValue == self.arduinoErrorOverPosition:
                        pathFile.close()
                        return -1
                    elif inputValue == self.arduinoErrorZeroSwitchX:
                        pathFile.close()
                        return -2
                    elif inputValue == self.arduinoErrorOutOfBounds:
                        pathFile.close()
                        return -3
                    elif inputValue == self.arduinoErrorOutOfBounds:
                        pathFile.close()
                        return -3

        # Close opened path file
        pathFile.close()
        print("Done reading gcode")
        return 0

    def zeroMachine(self, ui):
        """Sends the command to zero the machine
        """
        zeroCommand = "beginZeroing\n"
        readyReceived = False
        while not readyReceived:
            if self.serialConnection.inWaiting() > 0:
                inputValue = ""
                inputValue = self.serialConnection.readline().decode()
                # print("in: " + inputValue)
                #TODO: Error checking here
                if inputValue == self.arduinoReadyForCommand:
                    # Give Gcode command
                    # print("ReadyReceived\n")
                    self.serialConnection.write(zeroCommand.encode())
                    readyReceived = True
                elif inputValue == self.arduinoErrorFailedZeroSwitch:
                    return -4
        return 0

        '''
        # Old code for making the machine go away for Fall 2019 Demo day
        windingPathCmd = "beginWindingPath\n"
        self.serialConnection.write(windingPathCmd.encode())

        # TODO: Send the path to zero the machine
        line = "%\n"
        self.serialConnection.write(line.encode())

        line = "G0 X600\n"
        readyReceived = False
        while not readyReceived:
            if self.serialConnection.inWaiting() > 0:
                inputValue = ""
                inputValue = self.serialConnection.readline().decode()
                print("in: " + inputValue)
                if inputValue == self.arduinoReadyForCommand:
                    # Give Gcode command
                    print("ReadyReceived\n")
                    print(line)
                    self.serialConnection.write(line.encode())
                    readyReceived = True

        ui.userZero()
        ui.displayMessage("\nZeroing")

        line = "G0 X0\n"
        readyReceived = False
        while not readyReceived:
            if self.serialConnection.inWaiting() > 0:
                inputValue = ""
                inputValue = self.serialConnection.readline().decode()
                print("in: " + inputValue)
                if inputValue == self.arduinoReadyForCommand:
                    # Give Gcode command
                    print("ReadyReceived\n")
                    print(line)
                    self.serialConnection.write(line.encode())
                    readyReceived = True

        line = "%\n"
        self.serialConnection.write(line.encode())
        '''
