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
        sensorRead = False
        while not sensorRead:
            if self.serialConnection.inWaiting() > 0:
                inputValue = self.serialConnection.readline().decode()
                if inputValue == arduinoReadyForCommand:
                        sensorRead = True
                        # TODO: store sensor values as float and return