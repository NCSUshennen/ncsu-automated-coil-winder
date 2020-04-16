# imports
from timeit import default_timer as timer
from guizero import App, Window, Text, TextBox, PushButton, Combo
from main import *


# ----------------------- Main Window Functions ---------------------------------------------------------------------- #

class GUI:
    # --------------------- Tooth Variables -------------------------------------------------------------------------- #
    statorToothLength = None
    statorToothHeight = None
    statorToothWidth = None
    statorWindHeight = None
    statorWindWidth = None
    statorShoeWidth = None
    distanceBetweenTeeth = None
    statorDiameter = None
    numberStatorTeeth = None
    numberWinds = None
    wireGauge = None
    wireMaterial = None

    # --------------------- Main Control Variables ------------------------------------------------------------------- #
    mainController = None

    # --------------------- GUI Construction Variables --------------------------------------------------------------- #
    app = None
    parameterWindow = None
    windingWindow = None
    postWindingWindow = None
    safetyInterruptWindow = None

    parameterButton = None
    windingButton = None
    postWindingButton = None

    predictedTimeMessage = None
    predictedFillFactorMessage = None
    predictedWindingResistanceMessage = None
    actualTimeMessage = None
    elongationMessage = None

    askStatorToothLength = None
    enteredStatorToothLength = None
    askStatorToothHeight = None
    enteredStatorToothHeight = None
    askStatorWindHeight = None
    enteredStatorWindHeight = None
    askStatorToothWidth = None
    enteredStatorToothWidth = None
    askStatorShoeWidth = None
    enteredStatorShoeWidth = None
    askNumberStatorTeeth = None
    enteredNumberStatorTeeth = None
    askNumberWinds = None
    enteredNumberWinds = None
    askWireGauge = None
    enteredWireGauge = None
    askWireMaterial = None
    enteredWireMaterial = None
    askDistanceBetweenTeeth = None
    enteredDistanceBetweenTeeth = None
    askStatorWindWidth = None
    enteredStatorWindWidth = None
    askStatorDiameter = None
    enteredStatorDiameter = None
    doneEnteringParamsButton = None
    parameterErrorMessage = None

    windingStatorMessage = None
    windingWindowCloseMessage = None

    postWindingTitle = None
    resistanceMessage = None
    capacitanceMessage = None
    inductanceMessage = None
    doneViewingPostWindingButton = None

    windingSafetyTitleMessage = None
    windingSafetyMessage = None
    windingSafetyTroubleshootMessage = None

    actualTime = None
    startWindingButton = None

    # --------------------- Functions -------------------------------------------------------------------------------- #
    # Float truncation
    def truncate(self, f, n):
        '''Truncates/pads a float f to n decimal places without rounding'''
        s = '{}'.format(f)
        if 'e' in s or 'E' in s:
            return '{0:.{1}f}'.format(f, n)
        i, p, d = s.partition('.')
        return '.'.join([i, (d + '0' * n)[:n]])

    # function for opening parameterWindow and halting other window activity
    def openParameterWindow(self):
        self.parameterWindow.show(wait=True)

    # function for closing parameterWindow
    def closeParameterWindow(self):
        self.parameterWindow.hide()

    # function for opening windingWindow and halting other window activity
    def openWindingWindow(self):
        self.windingWindow.show(wait=True)

    # function for closing windingWindow
    def closeWindingWindow(self):
        self.windingWindow.hide()

    # function for opening postWindingWindow and halting other window activity
    def openPostWindingWindow(self):
        self.postWindingWindow.show(wait=True)

    # function for closing postWindingWindow
    def closePostWindingWindow(self):
        self.postWindingWindow.hide()

    # function for opening parameterWindow when button pressed
    def parameterButtonPressed(self):
        # Disable buttons
        self.parameterButton.disable()
        self.zeroButton.disable()
        self.windingButton.disable()
        self.postWindingButton.disable()

        self.parameterErrorMessage.clear()
        self.windingMessage.clear()

        self.actualTimeMessage.clear()
        self.actualTimeMessage.append("Actual time: None (wind a coil)")

        self.openParameterWindow()

    def getErrorMessage(self, lookupCode):
        errorMessage = {
            -1: "ErrorHitOverPositionSwitch\n",
            -2: "ErrorHitXZeroingSwitch\n",
            -3: "ErrorHitYZeroingSwitch\n",
            -4: "ErrorHitZZeroingSwitch\n",
            -5: "ErrorDestinationOutOfBounds\n",
            -6: "ErrorAlarmX1\n",
            -7: "ErrorAlarmX2\n",
            -8: "ErrorAlarmY\n",
            -9: "ErrorAlarmY\n",
            -10: "ErrorFailedToHitXZeroingSwitch\n",
            -11: "ErrorFailedToHitYZeroingSwitch\n",
            -12: "ErrorFailedToHitZZeroingSwitch\n",
            -13: "ErrorBadCommand\n",
            -14: "ErrorNoWireDetected\n"
        }
        return errorMessage.get(lookupCode)

    # function for zeroing the machine when the zero button is pressed
    def zeroButtonPressed(self):
        # Have main send the zeroing signal to the arduino
        errorCode = self.mainController.sendZeroCommand()
        print("errorCode: " + str(errorCode))

        # Use error code to print error message if needed
        if (errorCode != 0):
            safetyMessage = self.getErrorMessage(errorCode)

            # Update message on safety window
            self.windingSafetyMessage.clear()
            self.windingSafetyMessage.append(safetyMessage)

            # Display safety screen and lock out everything else
            self.safetyInterruptWindow.show()
            self.parameterWindow.hide()
            self.windingWindow.hide()
            self.postWindingWindow.hide()

            # Disable buttons
            self.parameterButton.disable()
            self.zeroButton.disable()
            self.windingButton.disable()
            self.postWindingButton.disable()
        else:
            return

    def startWindingButtonPressed(self):
        # Wind
        start = timer()
        errorCode = self.mainController.startWinding()

        # Use error code to print error message
        if (errorCode != 0):
            safetyMessage = self.getErrorMessage(errorCode)

            # Update message on safety window
            self.windingSafetyMessage.clear()
            self.windingSafetyMessage.append(safetyMessage)

            # Display safety screen and lock out everything else
            self.safetyInterruptWindow.show()
            self.parameterWindow.hide()
            self.windingWindow.hide()
            self.postWindingWindow.hide()

            # Disable buttons
            self.parameterButton.disable()
            self.zeroButton.disable()
            self.windingButton.disable()
            self.postWindingButton.disable()
        else:
            # End windingTimer
            end = timer()
            actualTime = end - start

            # Update actual time message
            self.actualTimeMessage.clear()
            timeHours, timeSec = divmod(actualTime, 3600)
            timeMin, timeSec = divmod(timeSec, 60)
            self.actualTimeMessage.append(
                "Actual time: " + str(
                    str(self.truncate(timeHours, 0)) + " hour " + str(self.truncate(timeMin, 0)) + " min " + str(
                        self.truncate(timeSec, 3)) + " secs\n"))

            # Update elongation message
            # TODO: Test with Arduino Mega connected
            self.elongationMessage.clear()
            self.elongationMessage.append(str((self.truncate(self.mainController.getElongation()), 2) + " %"))

            # Enable buttons after winding is completed
            self.parameterButton.enable()
            self.zeroButton.enable()
            self.windingButton.enable()
            self.postWindingButton.enable()

            # Close Window
            self.closeWindingWindow()

        return

    # function for opening windingWindow when button pressed and starting winding process
    def windingButtonPressed(self):
        self.openWindingWindow()
        # self.windingMessage.clear()
        # self.windingMessage.append("Winding the stator")

        # Disable buttons
        self.parameterButton.disable()
        self.zeroButton.disable()
        self.windingButton.disable()
        self.postWindingButton.disable()

    def postWindingButtonPressed(self):
        self.mainController.startPostWindingTest()

        # Update Messages
        self.resistanceMessage.clear()
        self.resistanceMessage.append(str(self.mainController.getResistance()) + " Ohms")
        self.capacitanceMessage.clear()
        self.capacitanceMessage.append(str(self.mainController.getCapacitance()) + " F")
        self.inductanceMessage.clear()
        self.inductanceMessage.append(str(self.mainController.getInductance()) + " H")

        # Disable buttons
        self.parameterButton.disable()
        self.zeroButton.disable()
        self.windingButton.disable()
        self.postWindingButton.disable()

        # Open window
        self.openPostWindingWindow()

    def doneViewingPostWindingButtonPressed(self):
        # Enable buttons
        self.parameterButton.enable()
        self.zeroButton.enable()
        self.windingButton.enable()
        self.postWindingButton.enable()

        self.closePostWindingWindow()

    # ----------------------- Param Window Functions ----------------------------------------------------------------- #

    def doneEnteringParamsButtonPressed(self):
        try:
            # Set tooth parameters
            self.statorToothLength = float(self.enteredStatorToothLength.value)
            self.statorToothHeight = float(self.enteredStatorToothHeight.value)
            self.statorToothWidth = float(self.enteredStatorToothWidth.value)
            self.statorWindHeight = float(self.enteredStatorWindHeight.value)
            self.statorWindWidth = float(self.enteredStatorWindWidth.value)
            self.statorShoeWidth = float(self.enteredStatorShoeWidth.value)
            self.distanceBetweenTeeth = float(self.enteredDistanceBetweenTeeth.value)
            self.statorDiameter = float(self.enteredStatorDiameter.value)
            self.numberStatorTeeth = float(self.enteredNumberStatorTeeth.value)
            self.numberWinds = float(self.enteredNumberWinds.value)
            self.wireGauge = float(self.enteredWireGauge.value)
            self.wireMaterial = self.enteredWireMaterial.value

            # Instantiate Main and call the function for building gcode
            self.mainController.buildGCode(self.statorToothLength, self.statorToothHeight, self.statorWindHeight,
                                           self.statorToothWidth, self.statorShoeWidth, self.numberStatorTeeth,
                                           self.numberWinds,
                                           self.wireGauge, self.wireMaterial, self.distanceBetweenTeeth,
                                           self.statorWindWidth, self.statorDiameter)
            # Update predicted values
            self.predictedTimeMessage.clear()
            timeHours, timeSec = divmod(self.mainController.getPredictedTime(), 3600)
            timeMin, timeSec = divmod(timeSec, 60)
            self.predictedTimeMessage.append(
                "Predicted time: " + str(
                    str(self.truncate(timeHours, 0)) + " hour " + str(self.truncate(timeMin, 0)) + " min " + str(
                        self.truncate(timeSec, 3)) + " secs\n"))
            self.predictedFillFactorMessage.clear()
            self.predictedTimeMessage.append(
                "Predicted fill factor: " + str(
                    self.truncate(self.mainController.getPredictedFillFactor(), 3) + " %\n"))
            self.predictedWindingResistanceMessage.clear()
            self.predictedWindingResistanceMessage.append(
                "Predicted winding resistance: " + str(self.truncate(self.mainController.getPredictedResistance(),
                                                                     3) + " Ohms\n"))

            # Enable winding button
            self.parameterButton.enable()
            self.zeroButton.enable()
            self.windingButton.enable()

            # Close
            self.closeParameterWindow()

        except:
            self.parameterErrorMessage.clear()
            self.parameterErrorMessage.append("Non-valid parameters entered")
            self.parameterButton.enable()
            self.zeroButton.enable()

    # ----------------------- User Interface Creation ---------------------------------------------------------------- #

    def __init__(self):
        """Construct a new UserInterface
        """
        self.mainController = MainController()
        # Creates the GUI named app and appropriate windows
        self.app = App(title="Main window")
        self.parameterWindow = Window(self.app, title="Parameter window", layout="grid")
        # parameterWindow = Window(app, title="Parameter window", layout = "grid", bg = "black")
        # parameterWindow.text_color = "white"
        self.parameterWindow.hide()
        self.windingWindow = Window(self.app, title="Winding window")
        self.windingWindow.hide()
        self.postWindingWindow = Window(self.app, title="Post winding window", layout="grid")
        self.postWindingWindow.hide()
        self.safetyInterruptWindow = Window(self.app, title="Safety interrupt window")
        self.safetyInterruptWindow.hide()

        # ----------------------- Main Window Event Loop ------------------------------------------------------------- #
        # Event loop - Coil winder GUI Main window widget (text, text boxes, buttons, etc) code here
        self.parameterButton = PushButton(self.app, command=self.parameterButtonPressed, text="Enter Stator Parameters")
        self.zeroButton = PushButton(self.app, command=self.zeroButtonPressed, text="Zero the machine")
        self.windingButton = PushButton(self.app, command=self.windingButtonPressed, text="Start winding")
        self.postWindingButton = PushButton(self.app, command=self.postWindingButtonPressed,
                                            text="Start post winding test")

        self.predictedTimeMessage = Text(self.app, text="Predicted time: None (enter parameters)")
        self.predictedFillFactorMessage = Text(self.app, text="Predicted fill factor: None (enter parameters)")
        self.predictedWindingResistanceMessage = Text(self.app,
                                                      text="Predicted winding resistance: None (enter parameters)")
        self.actualTimeMessage = Text(self.app, text="Actual time: None (wind a coil)")
        self.elongationMessage = Text(self.app, text="Elongation: None (wind a coil)")
        self.windingMessage = Text(self.app, text="")

        # Disable other buttons except enter stator parameters
        self.windingButton.disable()
        self.postWindingButton.disable()

        # ----------------------- Parameter Window Event Loop -------------------------------------------------------- #
        # Event loop - Coil winder GUI Parameter window widget (text, text boxes, buttons, etc) code here
        i = 0
        self.askStatorToothLength = Text(self.parameterWindow, text="Stator tooth length (mm):", grid=[0, i],
                                         align="left")
        self.enteredStatorToothLength = TextBox(self.parameterWindow, width=40, grid=[1, i], align="left")
        i += 1

        self.askStatorToothHeight = Text(self.parameterWindow, text="Stator tooth height (mm):", grid=[0, i],
                                         align="left")
        self.enteredStatorToothHeight = TextBox(self.parameterWindow, width=40, grid=[1, i], align="left")
        i += 1

        self.askStatorToothWidth = Text(self.parameterWindow, text="Stator tooth width (mm):", grid=[0, i],
                                        align="left")
        self.enteredStatorToothWidth = TextBox(self.parameterWindow, width=40, grid=[1, i], align="left")
        i += 1

        self.askStatorWindHeight = Text(self.parameterWindow, text="Stator wind height (mm):", grid=[0, i],
                                        align="left")
        self.enteredStatorWindHeight = TextBox(self.parameterWindow, width=40, grid=[1, i], align="left")
        i += 1

        self.askStatorWindWidth = Text(self.parameterWindow, text="Stator wind width (mm):", grid=[0, i],
                                       align="left")
        self.enteredStatorWindWidth = TextBox(self.parameterWindow, width=40, grid=[1, i], align="left")
        i += 1

        self.askStatorShoeWidth = Text(self.parameterWindow, text="Stator shoe width (mm):", grid=[0, i], align="left")
        self.enteredStatorShoeWidth = TextBox(self.parameterWindow, width=40, grid=[1, i], align="left")
        i += 1

        self.askDistanceBetweenTeeth = Text(self.parameterWindow, text="Distance between teeth: (mm)", grid=[0, i],
                                            align="left")
        self.enteredDistanceBetweenTeeth = TextBox(self.parameterWindow, width=40, grid=[1, i], align="left")
        i += 1

        self.askStatorDiameter = Text(self.parameterWindow, text="Stator diameter:", grid=[0, i], align="left")
        self.enteredStatorDiameter = TextBox(self.parameterWindow, width=40, grid=[1, i], align="left")
        i += 1

        self.askNumberStatorTeeth = Text(self.parameterWindow, text="Number stator teeth:", grid=[0, i], align="left")
        self.enteredNumberStatorTeeth = TextBox(self.parameterWindow, width=40, grid=[1, i], align="left")
        i += 1

        self.askNumberWinds = Text(self.parameterWindow, text="Number winds per tooth:", grid=[0, i], align="left")
        self.enteredNumberWinds = TextBox(self.parameterWindow, width=40, grid=[1, i], align="left")
        i += 1

        self.askWireGauge = Text(self.parameterWindow, text="Wire gauge:", grid=[0, i], align="left")
        self.enteredWireGauge = Combo(self.parameterWindow, options=["13", "14", "15", "16", "17", "18"], grid=[1, i],
                                      align="left")
        i += 1

        self.askWireMaterial = Text(self.parameterWindow, text="Wire material:", grid=[0, i], align="left")
        self.enteredWireMaterial = Combo(self.parameterWindow, options=["Copper"], grid=[1, i], align="left")
        i += 1

        self.doneEnteringParamsButton = PushButton(self.parameterWindow, command=self.doneEnteringParamsButtonPressed,
                                                   text="Done",
                                                   grid=[0, i], align="left")
        i += 1

        self.parameterErrorMessage = Text(self.parameterWindow, text="", grid=[0, i],
                                          align="left")
        i += 1

        # ----------------------- Winding Window Event Loop ---------------------------------------------------------- #
        # Event loop - Coil winder GUI Parameter window widget (text, text boxes, buttons, etc) code here
        self.windingStatorMessage = Text(self.windingWindow, text="Winding", size=30, color="green")
        self.windingWindowCloseMessage = Text(self.windingWindow,
                                              text="Press the button to start winding.\n Window will close when winding is complete.")
        self.startWindingButton = PushButton(self.windingWindow, command=self.startWindingButtonPressed,
                                             text="Start winding")

        # ----------------------- Post Winding Window Event Loop ----------------------------------------------------- #
        # Event loop - Coil winder GUI Parameter window widget (text, text boxes, buttons, etc) code here
        self.postWindingTitle = Text(self.postWindingWindow, text="Post Winding Results ", size=20,
                                     font="Times New Roman",
                                     grid=[0, 0],
                                     align="left")
        self.resistanceMessage = Text(self.postWindingWindow, text="Resistance: ", font="Times New Roman", grid=[0, 1],
                                      align="left")
        self.capacitanceMessage = Text(self.postWindingWindow, text="Capacitance: ", font="Times New Roman",
                                       grid=[0, 2],
                                       align="left")
        self.inductanceMessage = Text(self.postWindingWindow, text="Inductance: ", font="Times New Roman", grid=[0, 3],
                                      align="left")
        self.doneViewingPostWindingButton = PushButton(self.postWindingWindow,
                                                       command=self.doneViewingPostWindingButtonPressed, text="Done",
                                                       grid=[0, 4], align="left")

        # ----------------------- Safety Interrupt Window Event Loop ------------------------------------------------- #
        self.windingSafetyTitleMessage = Text(self.safetyInterruptWindow, text="Safety Error", size=40,
                                              font="Times New Roman",
                                              color="red")
        self.windingSafetyMessage = Text(self.safetyInterruptWindow,
                                         text="Something went wrong and the program was halted.",
                                         font="Times New Roman")
        self.windingSafetyTroubleshootMessage = Text(self.safetyInterruptWindow,
                                                     text="Reset the machine and try again.",
                                                     font="Times New Roman")

        self.app.display()  # start the event loop - while true block - code after this doesn't execute


gui = GUI()
