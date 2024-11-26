import pygame
import time
import serial
import serial.tools.list_ports
import threading
import mapping
from serial import SerialException

def find_vex_v5_port():
    ports = serial.tools.list_ports.comports()
    print("Available serial ports:")
    port_name = None
    for port in ports:
        print(f"Port: {port.device}, Description: {port.description}")
        if "Communications Port" in port.description:
            print(f"Found VEX V5 Communications Port on port: {port.description}")
        if "User Port" in port.description:
            print(f"Found VEX V5 User Port on port: {port.device}")
            port_name = port.device
    return port_name

def connect_serial(port_name, retries=5, delay=2):
    for attempt in range(retries):
        try:
            ser = serial.Serial(port_name, 115200, timeout=1)
            if ser is not None and ser.is_open:
                print(f"Connected to {port_name}")
                return ser
            else:
                print(f"Failed to connect to {port_name}")
        except serial.SerialException as e:
            print(f"Failed to connect to {port_name}: {e}")
            time.sleep(delay)
    print(f"Failed to connect to {port_name} after {retries} attempts")
    return None      

def initialize_controller():
    pygame.joystick.quit()
    pygame.joystick.init()
    if pygame.joystick.get_count() == 0:
        print("No joystick connected")
        return None
    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    print(f"Joystick name: {joystick.get_name()}")
    print(f"Number of axes: {joystick.get_numaxes()}")
    print(f"Number of buttons: {joystick.get_numbuttons()}")
    return joystick
def read_console(ser, stop_event):
    buffer = ""
    while not stop_event.is_set():
        try:
            if ser and ser.is_open and ser.in_waiting > 0:
                buffer += ser.read(ser.in_waiting).decode('utf-8')
                while '\n' in buffer:
                    line, buffer = buffer.split('\n', 1)
                    print(line.strip())
        except SerialException as e:
            print(f"Serial exception in read_console: {e}")
            break
        except Exception as e:
            print(f"Unexpected exception in read_console: {e}")
            break
        if not ser or not ser.is_open:
            print("Serial connection closed in read_console")
            break

def main():
    port_name = find_vex_v5_port()
    ser = connect_serial(port_name)
    while ser is None:
        port_name = find_vex_v5_port()
        ser = connect_serial(port_name)
        time.sleep(1)
    
    pygame.init()
    joystick = initialize_controller()
    while joystick is None:
        print("Reconnecting joystick...")
        time.sleep(1)
        joystick = initialize_controller()
    
    stop_event = threading.Event()
    console_thread = threading.Thread(target=read_console, args=(ser, stop_event))
    console_thread.daemon = True
    console_thread.start()
    
    prev_axes = {name: 0 for name in mapping.controller['joystickAxis']}
    prev_buttons = {name: 0 for name in mapping.controller['joystickButtons']}
    prev_hats = {'AXIS-HORIZONTAL': 0, 'AXIS-VERTICAL': 0}

    try:
        ser.write("XBOXRESTART\n".encode('utf-8'))
        ser.flush()

        while True:
            try:
                pygame.event.pump()
                if pygame.joystick.get_count() == 0:
                    print("Joystick disconnected. Reconnecting...")
                    joystick = initialize_controller()
                    while joystick is None:
                        time.sleep(1)
                        joystick = initialize_controller()
                    prev_axes = {name: 0 for name in mapping.controller['joystickAxis']}
                    prev_buttons = {name: 0 for name in mapping.controller['joystickButtons']}
                    prev_hats = {'AXIS-HORIZONTAL': 0, 'AXIS-VERTICAL': 0}

                axes = {name: joystick.get_axis(id) for name, id in mapping.controller['joystickAxis'].items()}
                buttons = {name: joystick.get_button(id) for name, id in mapping.controller['joystickButtons'].items()}
                dpad_id = mapping.controller['joystickHats']['DPAD']
                if dpad_id < joystick.get_numhats():
                    hat_value = joystick.get_hat(dpad_id)
                    hats = {'AXIS-HORIZONTAL': hat_value[0], 'AXIS-VERTICAL': hat_value[1]}
                else:
                    hats = {'AXIS-HORIZONTAL': 0, 'AXIS-VERTICAL': 0}

                for name, value in axes.items():
                    if value != prev_axes[name]:
                        data_str = f"xboxaxis_{name}$${value}\n".encode('utf-8')
                        if ser and ser.is_open:
                            ser.write(data_str)
                            ser.flush()
                        prev_axes[name] = value

                for name, value in buttons.items():
                    if value != prev_buttons[name]:
                        data_str = f"xboxbutton_{name}$${value}\n".encode('utf-8')
                        if ser and ser.is_open:
                            ser.write(data_str)
                            ser.flush()
                        prev_buttons[name] = value

                for name, value in hats.items():
                    if value != prev_hats[name]:
                        data_str = f"xboxdpad_{name}$${value}\n".encode('utf-8')
                        if ser and ser.is_open:
                            ser.write(data_str)
                            ser.flush()
                        prev_hats[name] = value
            except SerialException as se:
                print(f"Serial exception: {se}. Attempting to reconnect...")
                # keep attempting to reconnect
                ser.close()
                stop_event.set()
                ser = connect_serial(find_vex_v5_port())
                while (ser is None) or (not ser.is_open):
                    ser = connect_serial(find_vex_v5_port())
                    time.sleep(1)
                # Now restart read_console thread
                stop_event.clear()
                console_thread = threading.Thread(target=read_console, args=(ser, stop_event))
                console_thread.daemon = True
                console_thread.start()
            except pygame.error as pe:
                print(f"Pygame error: {pe}")
                # keep attempting to reconnect
                while pygame.joystick.get_count() == 0:
                    time.sleep(1)
                joystick = initialize_controller()
                prev_axes = {name: 0 for name in mapping.controller['joystickAxis']}
                prev_buttons = {name: 0 for name in mapping.controller['joystickButtons']}
                prev_hats = {'AXIS-HORIZONTAL': 0, 'AXIS-VERTICAL': 0}
            # time.sleep(0.1)

    except KeyboardInterrupt:
        print("Exiting...")

    finally:
        pygame.quit()
        if ser and ser.is_open:
            ser.close()
        stop_event.set()

if __name__ == "__main__":
    main()