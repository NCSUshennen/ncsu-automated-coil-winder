# imports
from guizero import App, Text, TextBox, PushButton, Slider

# function for changing text from text box
def say_my_name():
    message.value = my_name.value
    # Coil winding: break in a function (???) then come back when main says so

# function for slider
def change_text_size(slider_value):
    message.size = slider_value

# Creates the GUI named app
app = App(title="Hello world")

# Event loop - GUI widget (text, text boxes, buttons, etc) code here
message = Text(app, text="Welcome to my app", size=40, font="Times New Roman", color="lightblue")
my_name = TextBox(app, width=40)
update_text = PushButton(app, command=say_my_name, text="Display my name")
text_size = Slider(app, command=change_text_size, start=10, end=80)

app.display()  # start the event loop - while true block - code after this doesn't execute