# importing neccessary libraries and modules
import tkinter as tk
from tkinter import messagebox
import serial
from tkinter import ttk

# defining the port on which the bluetooth module is connected on the computer
bluetooth_port = 'COM5'  # Replace 'COM5' with the appropriate port
baud_rate = 38400        # baud rate at which bluetooth communicates with the device

command = ""  # Global variable to store the selected command

# initial confirmation box to start the program
messagebox.showinfo("Confirmation", "Click OK to proceed to the ROVER CONTROL!")

# this function will send the command based upon the command string and value of time from the entry/input box and the speed slider 
# over the serial port defined for bluetooth communication, if no time value is available 0 is read
def send_command():
    global command
    if command:
        speed = speed_slider.get()
        time = time_entry.get() or "0"
        full_command = f"{command}-{speed}-{time}"
        try:
            bluetooth = serial.Serial(
                port=bluetooth_port,
                baudrate=baud_rate,
            )
            bluetooth.write((full_command).encode())
            bluetooth.close()
            print("Command: " + full_command)
        except serial.SerialException:
            messagebox.showerror("Error", "Failed to establish a connection with the Bluetooth module.")
    else:
        messagebox.showinfo("Warning", "Please select a command.")

# this function is used to change between the three colours for button border and text when the heading label is clicked
def toggle_color(do_default):
    if(do_default):
        button_style.configure(button_forward['style'], background='#87CEEB', foreground='#87CEEB')
    else:
        current_color = button_style.lookup(button_forward['style'], 'background')
        if current_color == '#008080':
            button_style.configure(button_forward['style'], background='#FF7F50', foreground='#FF7F50')
        elif current_color == '#FF7F50':
            button_style.configure(button_forward['style'], background='#87CEEB', foreground='#87CEEB')
        elif current_color == '#87CEEB':
            button_style.configure(button_forward['style'], background='#008080', foreground='#008080')
        
# Thesee functions correspond to the action when each of the respective buttons is pressed
def forward_clicked():
    global command
    command = "forward"

def backward_clicked():
    global command
    command = "backward"

def left_clicked():
    global command
    command = "left"

def right_clicked():
    global command
    command = "right"

def three_clicked():
    global command
    time_entry.delete(0, tk.END)
    time_entry.insert(0, "3000")

def five_clicked():
    global command
    time_entry.delete(0, tk.END)
    time_entry.insert(0, "5000")

def stop_clicked():
    global command
    command = "stop"
    send_command()

def reset_clicked():
    global command
    speed_slider.set(0)             # setting speed slider to 0
    time_entry.delete(0, tk.END)    # setting time input/entry box to 0
    command = ""
    toggle_color(1)                 # switching to default color

# Create the main window
window = tk.Tk()
window.geometry("1280x720")
window.title("Arduino Car Control")

# Disable window resizing to avoid distortions
window.resizable(False, False)

# Load background image
background_image = tk.PhotoImage(file="E:/gui_HD/3.png")

# Creating a  canvas for the background
canvas = tk.Canvas(window, width=1280, height=720)
canvas.pack(fill="both", expand=True)

# Setting the background image
canvas.create_image(0, 0, anchor=tk.NW, image=background_image)

# Creating the main heading label and placing and the respective coordinates
heading_label = tk.Label(window, text=" Rover Remote Control ", font=("Arial", 36, "bold"), fg="black", bg="#E6E6FA")
heading_label.place(x=320, y=30)

# making the background button clickable through the label by binding the label to button's click action
heading_label.bind("<Button-1>", lambda event: toggle_color(0))

# Creating the command buttons
button_forward = ttk.Button(window, text="Forward", width=10, style="RoundedButton.TButton", command=forward_clicked)
button_backward = ttk.Button(window, text="Backward", width=10, style="RoundedButton.TButton", command=backward_clicked)
button_left = ttk.Button(window, text="Left", width=10, style="RoundedButton.TButton", command=left_clicked)
button_right = ttk.Button(window, text="Right", width=10, style="RoundedButton.TButton", command=right_clicked)
button_stop = ttk.Button(window, text="STOP", width=10, style="RoundedButton.TButton", command=stop_clicked)
button_three_sec = ttk.Button(window, text="3 Seconds", width=10, style="RoundedButton.TButton", command=three_clicked)
button_five_sec = ttk.Button(window, text="5 Seconds", width=10, style="RoundedButton.TButton", command=five_clicked)

# placing the created buttons on the specified corrdinates
button_forward.place(x=460, y=150)
button_backward.place(x=460, y=310)
button_left.place(x=300, y=230)
button_right.place(x=620, y=230)
button_stop.place(x=460, y=230)
button_three_sec.place(x=390, y=540)
button_five_sec.place(x=545, y=540)

# Creating speed slider
speed_slider = tk.Scale(window, from_=0, to=255, orient=tk.HORIZONTAL, label="Speed", length=300, width=20, font=("Helvetica", 12), bg="#E6E6FA")
speed_slider.place(x=380, y=420)

# Creating Time label
time_label = tk.Label(window, text="Time (in millis)", font=("Times New Roman", 12, "bold"), fg="black", bg="#E6E6FA")
time_label.place(x=400, y=620)

# Create time entry
time_entry = tk.Entry(window, width=12, font=("Helvetica", 12), bg="#E6E6FA")
time_entry.place(x=535, y=620)

# Create send button
send_button = tk.Button(window, text="Send Command", width=15, height=2, font=("Helvetica", 12), bg="#E6E6FA", command=send_command)
send_button.place(x=920, y=300)

# Create reset button
reset_button = tk.Button(window, text="Reset", width=10, height=2, font=("Helvetica", 12), bg="#E6E6FA", command=reset_clicked)
reset_button.place(x=950, y=400)

# Create name label
name_label = tk.Label(window, text="Abhinav Sharma", font=("Times New Roman", 8, "bold"), fg="black", bg="#E6E6FA")
name_label.place(x=1150, y=690)

# defining style for the movement and time buttons to apply on them
button_style = ttk.Style()
button_style.configure(
    "RoundedButton.TButton",
    relief="raised",
    background="#87CEEB",
    foreground="#87CEEB",
    font=("Helvetica", 12),
    borderwidth=1,
    bordercolor="#4CAF50",
    focuscolor="#45A049",
    padding=8,
)

button_style.map(
    "RoundedButton.TButton",
    bordercolor=[("active", "#45A049"), ("disabled", "#A0A0A0")],
    background=[("active", "#45A049"), ("disabled", "#D0D0D0")],
)

# Starting the main event loop
window.mainloop()
