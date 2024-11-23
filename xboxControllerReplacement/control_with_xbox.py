import pygame
import time
import serial
import serial.tools.list_ports
import threading

# List all available serial ports
ports = serial.tools.list_ports.comports()
print("Available serial ports:")
port_name = None
for port in ports:
    if "VEX V5 Communications Port" in port.description:
        print(f"Found VEX V5 Communications Port on port: {port.description}")
        
    if "VEX V5 User Port" in port.description:
        print(f"Found VEX V5 User Port on port: {port_name}")
        port_name = port.device

# Open the serial port
ser = serial.Serial(port_name, 115200, timeout=1)

# Pause the script
input("Press Enter to continue...")

# Initialize pygame
pygame.init()

# Initialize the joystick
pygame.joystick.init()

# Check if there is at least one joystick connected
if pygame.joystick.get_count() == 0:
    print("No joystick connected")
    exit()

# Get the first joystick
joystick = pygame.joystick.Joystick(0)
joystick.init()

print(f"Joystick name: {joystick.get_name()}")
print(f"Number of axes: {joystick.get_numaxes()}")
print(f"Number of buttons: {joystick.get_numbuttons()}")

# Function to read console input and send it over serial
def read_console():
    while True:
        console_input = input()
        ser.write(console_input.encode('utf-8'))

# Start a thread to read from the console
console_thread = threading.Thread(target=read_console)
console_thread.daemon = True
console_thread.start()

# Initialize previous state
prev_axes = [0] * joystick.get_numaxes()
prev_buttons = [0] * joystick.get_numbuttons()
prev_hats = [0] * joystick.get_numhats()

"""
AXIS:
0: left horizontal
1: left vertical
2: right horizontal
3: right vertical
4: left trigger
5: right trigger

BUTTONS:
0: A
1: B
2: X
3: Y
4: LB
5: RB
6: VIEW
7: MENU
8: LSB
9: RSB
10: 
11:
12:
13:
14:
15:
16:

HATS (DPAD):
0: HDAPD
1: VDPAD
"""

try:
    ser.write("XBOXRESTART\n".encode('utf-8'))
    ser.flush()
    while True:
        # Process pygame events (required to update joystick state)
        pygame.event.pump()

        # Read axis values
        axes = [joystick.get_axis(i) for i in range(joystick.get_numaxes())]

        # Read button values
        buttons = [joystick.get_button(i) for i in range(joystick.get_numbuttons())]

        # Read hat values
        hats = [joystick.get_hat(i) for i in range(joystick.get_numhats())]

        # Check for changes in axes
        # format string: type_name--value
        # example: axis_lsv--0.5
        # example: button_x--0
        # example: dpad--(0, 1)
        for i in range(len(axes)):
            if axes[i] != prev_axes[i]:
                data_str = f"xboxaxis_{i}$${axes[i]}\n".encode('utf-8')
                ser.write(data_str)
                ser.flush()
                prev_axes[i] = axes[i]
                

        # Check for changes in buttons
        for i in range(len(buttons)):
            if buttons[i] != prev_buttons[i]:
                data_str = f"xboxbutton_{i}$${buttons[i]}\n".encode('utf-8')
                ser.write(data_str)
                ser.flush()
                prev_buttons[i] = buttons[i]
                

        # Check for changes in hats
        for i in range(len(hats)):
            if hats[i] != prev_hats[i]:
                # left value is horizontal, right value is vertical
                horizontal, vertical = hats[i]
                data_str = f"xboxdpad_{i}$$({horizontal}, {vertical})\n".encode('utf-8')
                ser.write(data_str)
                ser.flush()
                prev_hats[i] = hats[i]
                
    
        #print the data from the serial port
        # if ser.in_waiting > 0:
        #     data = ser.readline().decode('utf-8').strip()
        #     print(data)


except KeyboardInterrupt:
    print("Exiting...")

finally:
    # Quit pygame
    pygame.quit()
    # Close the serial port
    ser.close()