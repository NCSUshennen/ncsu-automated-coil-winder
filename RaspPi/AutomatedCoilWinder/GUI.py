# imports
from guizero import App, Window, Text, TextBox, PushButton, Combo
from main import *


# ----------------------- Main Window Functions ---------------------------------------------------------------------- #

class GUI:
    # --------------------- Tooth Variables -------------------------------------------------------------------------- #
    statorToothLength = None
    statorToothHeight = None
    statorWindHeight = None
    statorToothWidth = None
    statorShoeWidth = None
    numberStatorTeeth = None
    numberWinds = None
    wireGauge = None
    wireMaterial = None
    distanceBetweenTeeth = None

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
    doneEnteringParamsButton = None

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

    # --------------------- Functions -------------------------------------------------------------------------------- #
    # function for opening parameterWindow and halting other window activity
    def openParameterWindow(self):
        self.parameterWindow.show(wait=True)

    # function for closing parameterWindow
    def closeParameterWindow(self):
        # TODO: Make winding thing next to be pressed
        self.parameterWindow.hide()

    # function for opening windingWindow and halting other window activity
    def openWindingWindow(self):
        self.windingWindow.show(wait=True)

    # function for closing windingWindow
    def closeWindingWindow(self):
        # TODO: call this in program when things are done winding
        self.windingWindow.hide()

    # function for opening postWindingWindow and halting other window activity
    def openPostWindingWindow(self):
        self.postWindingWindow.show(wait=True)

    # function for closing postWindingWindow
    def closePostWindingWindow(self):
        # TODO: call this in program when things are done winding
        self.postWindingWindow.hide()

    # function for opening parameterWindow when button pressed
    def parameterButtonPressed(self):
        self.openParameterWindow()

    # function for zeroing the machine when the zero button is pressed
    def zeroButtonPressed(self):
        # Have main send the zeroing signal to the arduino
        self.mainController.sendZeroCommand()

    # function for opening windingWindow when button pressed and starting winding process
    def windingButtonPressed(self):
        # TODO: Winding stuff -> call other function
        self.openWindingWindow()

    def postWindingButtonPressed(self):
        self.openPostWindingWindow()

    # ----------------------- Param Window Functions ----------------------------------------------------------------- #

    def doneEnteringParamsButtonPressed(self):
        self.closeParameterWindow()
        # Set tooth parameters
        self.statorToothLength = float(self.enteredStatorToothLength.value)
        self.statorToothHeight = float(self.enteredStatorToothHeight.value)
        self.statorWindHeight = float(self.enteredStatorWindHeight.value)
        self.statorToothWidth = float(self.enteredStatorToothWidth.value)
        self.statorShoeWidth = float(self.enteredStatorShoeWidth.value)
        self.numberStatorTeeth = float(self.enteredNumberStatorTeeth.value)
        self.numberWinds = float(self.enteredNumberWinds.value)
        self.wireGauge = float(self.enteredWireGauge.value)
        self.wireMaterial = self.enteredWireMaterial.value
        self.distanceBetweenTeeth = float(self.enteredDistanceBetweenTeeth.value)

        # Instantiate Main and call the function for building gcode
        self.mainController = MainController()
        self.mainController.buildGCode(self.statorToothLength, self.statorToothHeight, self.statorWindHeight,
                                       self.statorToothWidth, self.statorShoeWidth, self.numberStatorTeeth,
                                       self.numberWinds,
                                       self.wireGauge, self.wireMaterial, self.distanceBetweenTeeth)
        # TODO: Update predicted values
        self.predictedTimeMessage.clear()
        self.predictedTimeMessage.append("Predicted time: " + str(self.mainController.getPredictedTime()) + " secs")
        self.predictedFillFactorMessage.clear()
        self.predictedTimeMessage.append(
            "Predicted fill factor: " + str(self.mainController.getPredictedFillFactor()) + " %")
        self.predictedWindingResistanceMessage.clear()
        self.predictedWindingResistanceMessage.append(
            "Predicted winding resistance: " + str(self.mainController.getPredictedResistance()) + " Ohms")

    # ----------------------- User Interface Creation ---------------------------------------------------------------- #

    def __init__(self):
        """Construct a new UserInterface
        """
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

        # ----------------------- Parameter Window Event Loop -------------------------------------------------------- #
        # Event loop - Coil winder GUI Parameter window widget (text, text boxes, buttons, etc) code here
        self.askStatorToothLength = Text(self.parameterWindow, text="Stator tooth length (mm):", grid=[0, 0],
                                         align="left")
        self.enteredStatorToothLength = TextBox(self.parameterWindow, width=40, grid=[1, 0], align="left")

        self.askStatorToothHeight = Text(self.parameterWindow, text="Stator tooth height (mm):", grid=[0, 1],
                                         align="left")
        self.enteredStatorToothHeight = TextBox(self.parameterWindow, width=40, grid=[1, 1], align="left")

        self.askStatorWindHeight = Text(self.parameterWindow, text="Stator wind height (mm):", grid=[0, 2],
                                        align="left")
        self.enteredStatorWindHeight = TextBox(self.parameterWindow, width=40, grid=[1, 2], align="left")

        self.askStatorToothWidth = Text(self.parameterWindow, text="Stator tooth width (mm):", grid=[0, 3],
                                        align="left")
        self.enteredStatorToothWidth = TextBox(self.parameterWindow, width=40, grid=[1, 3], align="left")

        self.askStatorShoeWidth = Text(self.parameterWindow, text="Stator shoe width (mm):", grid=[0, 4], align="left")
        self.enteredStatorShoeWidth = TextBox(self.parameterWindow, width=40, grid=[1, 4], align="left")

        self.askNumberStatorTeeth = Text(self.parameterWindow, text="Number stator teeth:", grid=[0, 5], align="left")
        self.enteredNumberStatorTeeth = TextBox(self.parameterWindow, width=40, grid=[1, 5], align="left")

        self.askNumberWinds = Text(self.parameterWindow, text="Number winds per tooth:", grid=[0, 6], align="left")
        self.enteredNumberWinds = TextBox(self.parameterWindow, width=40, grid=[1, 6], align="left")

        self.askWireGauge = Text(self.parameterWindow, text="Wire gauge:", grid=[0, 7], align="left")
        self.enteredWireGauge = Combo(self.parameterWindow, options=["13", "14", "15", "16", "17", "18"], grid=[1, 7],
                                      align="left")

        self.askWireMaterial = Text(self.parameterWindow, text="Wire material:", grid=[0, 8], align="left")
        self.enteredWireMaterial = Combo(self.parameterWindow, options=["Copper"], grid=[1, 8], align="left")

        self.askDistanceBetweenTeeth = Text(self.parameterWindow, text="Distance between teeth: (mm)", grid=[0, 9],
                                            align="left")
        self.enteredDistanceBetweenTeeth = TextBox(self.parameterWindow, width=40, grid=[1, 9], align="left")

        self.doneEnteringParamsButton = PushButton(self.parameterWindow, command=self.doneEnteringParamsButtonPressed,
                                                   text="Done",
                                                   grid=[0, 10], align="left")

        # ----------------------- Winding Window Event Loop ---------------------------------------------------------- #
        # Event loop - Coil winder GUI Parameter window widget (text, text boxes, buttons, etc) code here
        self.windingStatorMessage = Text(self.windingWindow, text="Winding stator", size=40, font="Times New Roman",
                                         color="green")
        self.windingWindowCloseMessage = Text(self.windingWindow, text="Window will close when winding is complete",
                                              font="Times New Roman")

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
        self.doneViewingPostWindingButton = PushButton(self.postWindingWindow, text="Done", grid=[0, 4], align="left")

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
