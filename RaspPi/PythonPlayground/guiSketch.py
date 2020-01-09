# imports
from guizero import App, Window, Text, TextBox, PushButton, Slider

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
parameterWindow = Window(app, title="Parameter window")
parameterWindow.hide();
windingWindow = Window(app, title="Parameter window")
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
doneEnteringParamsButton = PushButton(parameterWindow, command=doneEnteringParamsButtonPressed, text="Done")

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