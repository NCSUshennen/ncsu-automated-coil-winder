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

    arduinoErrorBadCommand = "ErrorBadCommand\n"


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
                    elif inputValue == self.arduinoErrorZeroSwitchY:
                        pathFile.close()
                        return -3
                    elif inputValue == self.arduinoErrorZeroSwitchZ:
                        pathFile.close()
                        return -4
                    elif inputValue == self.arduinoErrorOutOfBounds:
                        pathFile.close()
                        return -5
                    elif inputValue == self.arduinoErrorX1OverCurrent:
                        pathFile.close()
                        return -6
                    elif inputValue == self.arduinoErrorX1VoltageRef:
                        pathFile.close()
                        return -7
                    elif inputValue == self.arduinoErrorX1Param:
                        pathFile.close()
                        return -8
                    elif inputValue == self.arduinoErrorX1OverVolt:
                        pathFile.close()
                        return -9
                    elif inputValue == self.arduinoErrorX1OverPosition:
                        pathFile.close()
                        return -10
                    elif inputValue == self.arduinoErrorX2OverCurrent:
                        pathFile.close()
                        return -11
                    elif inputValue == self.arduinoErrorX2VoltageRef:
                        pathFile.close()
                        return -12
                    elif inputValue == self.arduinoErrorX2Param:
                        pathFile.close()
                        return -13
                    elif inputValue == self.arduinoErrorX2OverVolt:
                        pathFile.close()
                        return -14
                    elif inputValue == self.arduinoErrorX2OverPosition:
                        pathFile.close()
                        return -15
                    elif inputValue == self.arduinoErrorYOverCurrent:
                        pathFile.close()
                        return -16
                    elif inputValue == self.arduinoErrorYVoltageRef:
                        pathFile.close()
                        return -17
                    elif inputValue == self.arduinoErrorYParam:
                        pathFile.close()
                        return -18
                    elif inputValue == self.arduinoErrorYOverVolt:
                        pathFile.close()
                        return -19
                    elif inputValue == self.arduinoErrorYOverPosition:
                        pathFile.close()
                        return -20
                    elif inputValue == self.arduinoErrorZOverCurrent:
                        pathFile.close()
                        return -21
                    elif inputValue == self.arduinoErrorZVoltageRef:
                        pathFile.close()
                        return -22
                    elif inputValue == self.arduinoErrorZParam:
                        pathFile.close()
                        return -23
                    elif inputValue == self.arduinoErrorZOverVolt:
                        pathFile.close()
                        return -24
                    elif inputValue == self.arduinoErrorZOverPosition:
                        pathFile.close()
                        return -25
                    elif inputValue == self.arduinoErrorFailedZeroSwitchX:
                        pathFile.close()
                        return -26
                    elif inputValue == self.arduinoErrorFailedZeroSwitchY:
                        pathFile.close()
                        return -27
                    elif inputValue == self.arduinoErrorFailedZeroSwitchZ:
                        pathFile.close()
                        return -28
                    elif inputValue == self.arduinoErrorBadCommand:
                        pathFile.close()
                        return -29

        # Close opened path file
        pathFile.close()
        print("Done reading gcode")
        return 0

    def zeroMachine(self):
        """Sends the command to zero the machine
        """
        zeroCommand = "beginZeroing\n"
        print("Reached zeroing")
        self.serialConnection.write(zeroCommand.encode())
        readyReceived = False
        while not readyReceived:
            if self.serialConnection.inWaiting() > 0:
                inputValue = ""
                inputValue = self.serialConnection.readline().decode()
                print("in: " + inputValue)
                #TODO: Error checking here
                if inputValue == self.arduinoReadyForCommand:
                    # Give Gcode command
                    # print("ReadyReceived\n")
                    self.serialConnection.write(zeroCommand.encode())
                    readyReceived = True
                elif inputValue == self.arduinoErrorOverPosition:
                    print("-1")
                    return -1
                elif inputValue == self.arduinoErrorZeroSwitchX:
                    print("-1")
                    return -2
                elif inputValue == self.arduinoErrorZeroSwitchY:
                    print("-1")
                    return -3
                elif inputValue == self.arduinoErrorZeroSwitchZ:
                    print("-1")
                    return -4
                elif inputValue == self.arduinoErrorOutOfBounds:
                    print("-1")
                    return -5
                elif inputValue == self.arduinoErrorX1OverCurrent:
                    print("-1")
                    return -6
                elif inputValue == self.arduinoErrorX1VoltageRef:
                    print("-1")
                    return -7
                elif inputValue == self.arduinoErrorX1Param:
                    print("-1")
                    return -8
                elif inputValue == self.arduinoErrorX1OverVolt:
                    print("-1")
                    return -9
                elif inputValue == self.arduinoErrorX1OverPosition:
                    print("-1")
                    return -10
                elif inputValue == self.arduinoErrorX2OverCurrent:
                    print("-1")
                    return -11
                elif inputValue == self.arduinoErrorX2VoltageRef:
                    print("-1")
                    return -12
                elif inputValue == self.arduinoErrorX2Param:
                    print("-1")
                    return -13
                elif inputValue == self.arduinoErrorX2OverVolt:
                    print("-1")
                    return -14
                elif inputValue == self.arduinoErrorX2OverPosition:
                    print("-1")
                    return -15
                elif inputValue == self.arduinoErrorYOverCurrent:
                    print("-1")
                    return -16
                elif inputValue == self.arduinoErrorYVoltageRef:
                    print("-1")
                    return -17
                elif inputValue == self.arduinoErrorYParam:
                    print("-1")
                    return -18
                elif inputValue == self.arduinoErrorYOverVolt:
                    print("-1")
                    return -19
                elif inputValue == self.arduinoErrorYOverPosition:
                    print("-1")
                    return -20
                elif inputValue == self.arduinoErrorZOverCurrent:
                    return -21
                elif inputValue == self.arduinoErrorZVoltageRef:
                    print("-1")
                    return -22
                elif inputValue == self.arduinoErrorZParam:
                    print("-1")
                    return -23
                elif inputValue == self.arduinoErrorZOverVolt:
                    print("-1")
                    return -24
                elif inputValue == self.arduinoErrorZOverPosition:
                    print("-1")
                    return -25
                elif inputValue == self.arduinoErrorFailedZeroSwitchX:
                    print("-1")
                    return -26
                elif inputValue == self.arduinoErrorFailedZeroSwitchY:
                    print("-1")
                    return -27
                elif inputValue == self.arduinoErrorFailedZeroSwitchZ:
                    print("-1")
                    return -28
                elif inputValue == self.arduinoErrorBadCommand:
                    print("-1")
                    return -29
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