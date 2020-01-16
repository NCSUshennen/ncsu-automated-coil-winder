# imports
from guizero import App, Window, Text, TextBox, PushButton, Slider, Combo

# ----------------------- Main Window Functions ------------------------ #

# function for opening parameterWindow and halting other window activity
def openParameterWindow():
    parameterWindow.show(wait=True);

# function for closing parameterWindow
def closeParameterWindow():
    # TODO: Make winding thing next to be pressed
    parameterWindow.hide();

# function for opening windingWindow and halting other window activity
def openWindingWindow():
    windingWindow.show(wait=True);

# function for closing windingWindow
def closeWindingWindow():
    # TODO: call this in program when things are done winding
    windingWindow.hide();

# function for opening postWindingWindow and halting other window activity
def openPostWindingWindow():
    postWindingWindow.show(wait=True);

# function for closing postWindingWindow
def closePostWindingWindow():
    # TODO: call this in program when things are done winding
    postWindingWindow.hide();

# function for opening parameterWindow when button pressed
def parameterButtonPressed():
    openParameterWindow();

# function for opening windingWindow when button pressed and starting winding process
def windingButtonPressed():
    # TODO: Winding stuff -> call other function
    openWindingWindow();

def postWindingButtonPressed():
    openPostWindingWindow();

# ----------------------- Param Window Functions ------------------------ #
def doneEnteringParamsButtonPressed():
    closeParameterWindow();
    # ex
    statorToothLength = enteredStatorToothLength.value
    statorToothHeight = enteredStatorToothHeight.value
    statorToothWidth = enteredStatorToothWidth.value
    statorShoe = enteredStatorShoe.value
    numberStatorTeeth = enteredNumberStatorTeeth.value
    numberWinds = enteredNumberWinds.value
    wireGauge = enteredWireGauge.value
    wireMaterial = enteredWireMaterial.value
    distanceBetweenTeeth = enteredDistanceBetweenTeeth.value

# ----------------------- User Interface Creation ------------------------ #

# Creates the GUI named app and appropriate windows
app = App(title="Main window")
parameterWindow = Window(app, title="Parameter window", layout = "grid")
# parameterWindow = Window(app, title="Parameter window", layout = "grid", bg = "black")
# parameterWindow.text_color = "white"
parameterWindow.hide();
windingWindow = Window(app, title="Winding window")
windingWindow.hide();
postWindingWindow = Window(app, title="Post winding window", layout = "grid")
postWindingWindow.hide();
safetyInterruptWindow = Window(app, title="Safety interrupt window")

# ----------------------- Main Window Event Loop ------------------------ #
# Event loop - Coil winder GUI Main window widget (text, text boxes, buttons, etc) code here
parameterButton = PushButton(app, command=parameterButtonPressed, text="Enter Stator Parameters")
windingButton = PushButton(app, command=windingButtonPressed, text="Start winding")
postWindingButton = PushButton(app, command=postWindingButtonPressed, text="Start post winding test")

predictedTimeMessage = Text(app, text="Predicted time: None (enter parameters)");
predictedFillFactorMessage = Text(app, text="Predicted fill factor: None (enter parameters)");
predictedWindingResistanceMessage = Text(app, text="Predicted winding resistance: None (enter parameters)");
actualTimeMessage = Text(app, text="Actual time: None (wind a coil)");
elongationMessage = Text(app, text="Elongation: None (wind a coil)");

# ----------------------- Parameter Window Event Loop ------------------------ #
# Event loop - Coil winder GUI Parameter window widget (text, text boxes, buttons, etc) code here
askStatorToothLength = Text(parameterWindow, text="Stator tooth length (mm):", grid=[0,0], align="left")
enteredStatorToothLength = TextBox(parameterWindow, width=40, grid=[1,0], align="left")

askStatorToothHeight = Text(parameterWindow, text="Stator tooth height (mm):", grid=[0,1], align="left")
enteredStatorToothHeight = TextBox(parameterWindow, width=40, grid=[1,1], align="left")

askStatorToothWidth = Text(parameterWindow, text="Stator tooth width (mm):", grid=[0,2], align="left")
enteredStatorToothWidth = TextBox(parameterWindow, width=40, grid=[1,2], align="left")

askStatorShoe = Text(parameterWindow, text="Stator shoe width (mm):", grid=[0,3], align="left")
enteredStatorShoe = TextBox(parameterWindow, width=40, grid=[1,3], align="left")

askNumberStatorTeeth = Text(parameterWindow, text="Number stator teeth:", grid=[0,4], align="left")
enteredNumberStatorTeeth = TextBox(parameterWindow, width=40, grid=[1,4], align="left")

askNumberWinds= Text(parameterWindow, text="Number winds per tooth:", grid=[0,5], align="left")
enteredNumberWinds = TextBox(parameterWindow, width=40, grid=[1,5], align="left")

askWireGauge = Text(parameterWindow, text="Wire gauge:", grid=[0,6], align="left")
enteredWireGauge = Combo(parameterWindow, options=["13", "14", "15", "16", "17", "18"], grid=[1,6], align="left")

askWireMaterial = Text(parameterWindow, text="Wire material:", grid=[0,7], align="left")
enteredWireMaterial = Combo(parameterWindow, options=["Copper"], grid=[1,7], align="left")

askDistanceBetweenTeeth = Text(parameterWindow, text="Distance between teeth: (mm)", grid=[0,8], align="left")
enteredDistanceBetweenTeeth = TextBox(parameterWindow, width=40, grid=[1,8], align="left")

doneEnteringParamsButton = PushButton(parameterWindow, command=doneEnteringParamsButtonPressed, text="Done", grid=[0,9], align="left")


# ----------------------- Winding Window Event Loop ------------------------ #
# Event loop - Coil winder GUI Parameter window widget (text, text boxes, buttons, etc) code here
windingStatorMessage = Text(windingWindow, text="Winding stator", size=40, font="Times New Roman", color="green")
windingWindowCloseMessage = Text(windingWindow, text="Window will close when winding is complete", font="Times New Roman")

# ----------------------- Post Winding Window Event Loop ------------------------ #
# Event loop - Coil winder GUI Parameter window widget (text, text boxes, buttons, etc) code here
postWindingTitle = Text(postWindingWindow, text="Post Winding Results ", size=20, font="Times New Roman", grid=[0, 0], align="left")
resistanceMessage = Text(postWindingWindow, text="Resistance: ", font="Times New Roman", grid=[0,1], align="left")
capacitanceMessage = Text(postWindingWindow, text="Capacitance: ", font="Times New Roman", grid=[0,2], align="left")
inductanceMessage = Text(postWindingWindow, text="Inductance: ", font="Times New Roman", grid=[0,3], align="left")

# ----------------------- Safety Interrupt Window Event Loop ------------------------ #
windingStatorMessage = Text(safetyInterruptWindow, text="Safety Error", size=40, font="Times New Roman", color="red")
windingStatorMessage = Text(safetyInterruptWindow, text="Something went wrong and the program was halted.",font="Times New Roman")
windingStatorMessage = Text(safetyInterruptWindow, text="Reset the machine and try again.",font="Times New Roman")

app.display()  # start the event loop - while true block - code after this doesn't execute