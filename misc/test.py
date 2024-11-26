# import pygame, and use mappings from mapping.py to test the controller

import pygame
import time
import mapping
import os

# Initialize the joystick
pygame.init()
pygame.joystick.init()

# Check if there is at least one joystick connected
if pygame.joystick.get_count() == 0:
    print("No joystick connected")
    exit()

# Initialize the first joystick
joystick = pygame.joystick.Joystick(0)
joystick.init()

def print_controller_state():
    os.system('cls' if os.name == 'nt' else 'clear')  # Clear the screen

    # Print axes
    for name, id in mapping.controller['joystickAxis'].items():
        if id < joystick.get_numaxes():
            value = joystick.get_axis(id)
            print(f"{name} ({id}): {value:.2f}")
        else:
            print(f"{name} ({id}): Not available")

    # Print buttons
    for name, id in mapping.controller['joystickButtons'].items():
        if id < joystick.get_numbuttons():
            value = joystick.get_button(id)
            print(f"{name} ({id}): {value}")
        else:
            print(f"{name} ({id}): Not available")

    # Print hats
    dpad_id = mapping.controller['joystickHats']['DPAD']
    if dpad_id < joystick.get_numhats():
        hat_value = joystick.get_hat(dpad_id)
        horizontal, vertical = hat_value
        print(f"AXIS-HORIZONTAL ({dpad_id}): {horizontal}")
        print(f"AXIS-VERTICAL ({dpad_id}): {vertical}")
    else:
        print(f"AXIS-HORIZONTAL ({dpad_id}): Not available")
        print(f"AXIS-VERTICAL ({dpad_id}): Not available")

while True:
    pygame.event.pump()  # Process event queue
    print_controller_state()
    time.sleep(0.1)  # Adjust the delay as needed