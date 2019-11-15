#! /usr/bin/python3
# ------------------------------------------------- #
# NCSU Senior Design: Automated Coil Winder
#
# UserInterface.py
#    Handles getting parameters from and displaying
#    outputs to the user
#
# Created: November 2019
# Last modified: November 2019
# ------------------------------------------------- #

class UserInterface:
    # --------------------- Variables --------------------- #
    # statorToothLength = None

    # --------------------- Functions --------------------- #
    def __init__(self):
        """Construct a new UserInterface
        """
    def userZero(self):
        """Ask the user if it's okay to zero the machine.
            Return True when user okays.
        """
        # --------------------- Variables --------------------- #
        userOkayed = False

        # If the user wants to zero, return True
        # Otherwise, keep asking.
        while not userOkayed:
            userInput = input("Zero the machine? (y/n): ")
            if userInput == "y":
                userOkayed = True
                return True
            elif userInput == "n":
                print("Not zeroing.\n")
            else:
                print("Input error, please input 'y' or 'n'\n")

    def userStartWinding(self):
        """Ask the user if it's okay to start winding in general.
            Return True when user okays.
        """
        # --------------------- Variables --------------------- #
        userOkayed = False

        # If the user wants to start winding, return True
        # Otherwise, keep asking.
        while not userOkayed:
            userInput = input("Begin winding? (y/n): ")
            if userInput == "y":
                userOkayed = True
                return True
            elif userInput == "n":
                print("Not winding.\n")
            else:
                print("Input error, please input 'y' or 'n'\n")

    def userStartWindingTooth(self):
        """Ask the user if it's okay to start winding a tooth.
            Return True when user okays.
        """
        # --------------------- Variables --------------------- #
        userOkayed = False

        # If the user wants to start winding, return True
        # Otherwise, keep asking.
        while not userOkayed:
            userInput = input("Begin winding a tooth? (y/n): ")
            if userInput == "y":
                userOkayed = True
                return True
            elif userInput == "n":
                print("Not winding.\n")
            else:
                print("Input error, please input 'y' or 'n'\n")

    def userStartPostWindingTest(self):
        """Ask the user if it's okay to start the post test.
            Return True when user okays.
        """
        # --------------------- Variables --------------------- #
        userOkayed = False

        # If the user wants to zero, return True
        # Otherwise, keep asking.
        while not userOkayed:
            userInput = input("Start post test? (y/n): ")
            if userInput == "y":
                userOkayed = True
                return True
            elif userInput == "n":
                print("Not doing post test.\n")
            else:
                print("Input error, please input 'y' or 'n'\n")

    def displayMessage(self, message):
        """Displays a given string message to the user then begins
           a new line

        Keyword arguments:
        message -- String message to display to the user
        """
        print(message + "\n")



    # ------------------------- Parameter getters ------------------------- #



    def getStatorToothLength(self):
        """Return statorToothLength from user
        """
        # --------------------- Variables --------------------- #
        gotValue = False

        # Ask user for statorToothLength until they enter a valid int
        while not gotValue:
            try:
                userInput = int(input("Enter stator tooth length (in cm): "))
                gotValue = True
                return userInput
            except ValueError:
                print("Please enter a valid integer")

    def getStatorToothHeight(self):
        """Return statorToothHeight from user
        """
        # --------------------- Variables --------------------- #
        gotValue = False

        # Ask user for statorToothHeight until they enter a valid int
        while not gotValue:
            try:
                userInput = int(input("Enter stator tooth height (in cm): "))
                gotValue = True
                return userInput
            except ValueError:
                print("Please enter a valid integer")

    def getStatorToothWidth(self):
        """Return statorToothWidth from user
        """
        # --------------------- Variables --------------------- #
        gotValue = False

        # Ask user for statorToothWidth until they enter a valid int
        while not gotValue:
            try:
                userInput = int(input("Enter stator tooth width (in cm): "))
                gotValue = True
                return userInput
            except ValueError:
                print("Please enter a valid integer")

    def getStatorLipRadius(self):
        """Return statorLipRadius from user
        """
        # --------------------- Variables --------------------- #
        gotValue = False

        # Ask user for statorLipRadius until they enter a valid int
        while not gotValue:
            try:
                userInput = int(input("Enter stator lip radius (in cm): "))
                gotValue = True
                return userInput
            except ValueError:
                print("Please enter a valid integer")

    def getNumberStatorTeeth(self):
        """Return numberStatorTeeth from user
        """
        # --------------------- Variables --------------------- #
        gotValue = False

        # Ask user for numberStatorTeeth until they enter a valid int
        while not gotValue:
            try:
                userInput = int(input("Enter the number of stator teeth (in cm): "))
                gotValue = True
                return userInput
            except ValueError:
                print("Please enter a valid integer")

    def getNumberWireLayers(self):
        """Return numberWireLayers from user
        """
        # --------------------- Variables --------------------- #
        gotValue = False

        # Ask user for statorToothHeight until they enter a valid int
        while not gotValue:
            try:
                userInput = int(input("Enter number of wire layers (in cm): "))
                gotValue = True
                return userInput
            except ValueError:
                print("Please enter a valid integer")

    def getWireGauge(self):
        """Return wireGauge from user
        """
        # --------------------- Variables --------------------- #
        gotValue = False

        # Ask user for statorToothHeight until they enter a valid int
        while not gotValue:
            try:
                userInput = int(input("Enter stator wireGauge (in cm): "))
                gotValue = True
                return userInput
            except ValueError:
                print("Please enter a valid integer")

    def getWireMaterial(self):
        """Return wireMaterial from user
        """
        # --------------------- Variables --------------------- #
        gotValue = False

        # Ask user for statorToothHeight until they enter a valid int
        while not gotValue:
            try:
                userInput = input("Enter wire material: ")
                gotValue = True
                return userInput
            except ValueError:
                print("Please enter a valid string")

    def getDistanceBetweenTeeth(self):
        """Return distanceBetweenTeeth from user
        """
        # --------------------- Variables --------------------- #
        gotValue = False

        # Ask user for distanceBetweenTeeth until they enter a valid int
        while not gotValue:
            try:
                userInput = int(input("Enter distance between teeth (in cm): "))
                gotValue = True
                return userInput
            except ValueError:
                print("Please enter a valid integer")

