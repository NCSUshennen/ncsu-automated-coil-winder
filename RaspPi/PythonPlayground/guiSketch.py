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

'''
# ----------------------- Test Code Functions ------------------------ #

# function for changing text from text box
def say_my_name():
    message.value = my_name.value
    # Coil winding: break in a function (???) then come back when main says so

# function for slider
def change_text_size(slider_value):
    message.size = slider_value
'''
# ----------------------- User Interface Creation ------------------------ #

# Creates the GUI named app and appropriate windows
app = App(title="Main window")
parameterWindow = Window(app, title="Parameter window", layout = "grid")
parameterWindow.hide();
windingWindow = Window(app, title="Winding window")
windingWindow.hide();
postWindingWindow = Window(app, title="Post winding window")
postWindingWindow.hide();

# ----------------------- Main Window Event Loop ------------------------ #
# Event loop - Coil winder GUI Main window widget (text, text boxes, buttons, etc) code here
parameterButton = PushButton(app, command=parameterButtonPressed, text="Enter Stator Parameters")
windingButton = PushButton(app, command=windingButtonPressed, text="Start winding")
postWindingButton = PushButton(app, command=postWindingButtonPressed, text="Start post winding test")

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

# ----------------------- Post Winding Window Event Loop ------------------------ #
# Event loop - Coil winder GUI Parameter window widget (text, text boxes, buttons, etc) code here

''' # ----------------------- Test Code Event Loop ------------------------ #
# Event loop - GUI widget (text, text boxes, buttons, etc) code here
message = Text(app, text="Welcome to my app", size=40, font="Times New Roman", color="lightblue")
my_name = TextBox(app, width=40)
update_text = PushButton(app, command=say_my_name, text="Display my name")
text_size = Slider(app, command=change_text_size, start=10, end=80)
'''

app.display()  # start the event loop - while true block - code after this doesn't execute