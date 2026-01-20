import serial
import time
import random

# Inicializace sériového portu
ser = serial.Serial(
    port='COM3',
    baudrate=115200,
    timeout=0.1
)

if not ser.is_open:
    print("COM3 port is not open, opening...")
    ser.open()

def send_message(message):
    print(f"Message sended to bus: {message}")
    ser.write(message.encode("utf-8"))

try:
    print("Emulator started, waiting for commands...")
    while True:
        res = ser.readline()
        if res:
            print(f"Message received from bus: {res.decode('utf-8')}")
            if "update" in res.decode('utf-8').lower():
                print("Update command received.")
                send_message("?id=0&status=1&temp=100&alarm=0")
            elif "config" in res.decode('utf-8').lower():
                print("Config command received.")
                send_message("?id=0&status=1")
            elif "connect" in res.decode('utf-8').lower():
                print("Connect command received.")
                send_message("?id=0&status=1")
            elif "disconnect" in res.decode('utf-8').lower():
                print("Disconnect command received.")
                send_message("?id=0&status=1")
        time.sleep(0.05)

except KeyboardInterrupt:
    print("Comm closing...")

finally:
    ser.close()
