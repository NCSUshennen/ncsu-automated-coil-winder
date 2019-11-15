#! /usr/bin/python3
# ------------------------------------------------- #
# NCSU Senior Design: Automated Coil Winder
#
# SensorReader.py
#    Retrieves the value from the Arduino for a given
#    sensor
#
# Created: November 2019
# Last modified: November 2019
# ------------------------------------------------- #

class SensorReader:
    # --------------------- Variables --------------------- #
    serialConnection = None
    arduinoReadyForCommand = "ready\n"

    # --------------------- Functions --------------------- #
    def __init__(self, arduinoSerial):
        """Construct a new sensor reader

        Keyword arguments:
        arduinoSerial -- the serial connection to read from (default 0.0)
        """
        self.serialConnection = arduinoSerial

    def readSensor(self, sensor):
        """Read the given sensor and return the sensor value.

        Keyword arguments:
        sensor -- the sensor to read (default 0.0)
        """
        # --------------------- Variables --------------------- #
        arduinoReadyForCommand = "ready\n"

        # Wait for Arduino to be ready, then read sensor

        # self.serialConnection.write("doPostWindingTest\n".encode())
        self.serialConnection.write(self.arduinoReadyForCommand.encode())
        print("Asked if ready")

        readyReceived = False
        while not readyReceived:
            if self.serialConnection.inWaiting() > 0:
                inputValue = self.serialConnection.readline().decode()
                print("in: " + inputValue)
                if inputValue == arduinoReadyForCommand:
                        # Give command to get sensor
                        print("ReadyReceived\n")
                        readyReceived = True
                        self.serialConnection.write(sensor.encode())

        print("out of while1\n")
        sensorRead = False
        while not sensorRead:
            if self.serialConnection.inWaiting() > 0:
                print("GettingSensor\n")
                sensorRead = True
                sensorValue = self.serialConnection.readline.decode()

        print("out of while2\n")
        print("Value: " + sensorValue)
        return sensorValue