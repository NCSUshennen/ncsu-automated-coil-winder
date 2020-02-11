# imports
from guizero import App, Text, Combo
# Modal Window object for other windows -> true to wait parameter of show for window

# Creates the GUI named app
app = App(title="Second gui app", width=300, height=200, layout="grid")

# Event loop - GUI widget (text, text boxes, buttons, etc) code here
film_description = Text(app, text="Which film?", grid=[0,0], align="left")
film_choice = Combo(app, options=["Star Wars", "Frozen", "Lion King"], grid=[1,0], align="left")

app.display()  # start the event loop - while true block - code after this doesn't execute