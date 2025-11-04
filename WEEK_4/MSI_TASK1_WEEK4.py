import serial
import numpy as np
import matplotlib.pyplot as plt
from drawnow import drawnow

arduino = serial.Serial('COM3',115200)
plt.ion()
fig, ax = plt.subplots()
# Data buffers
x_pos = [0]
y_pos = [0]
z_pos = [0]
vx, vy, vz = 0, 0, 0  # Initial velocities
dt = 0.02  # Time step (~20ms)
last_gesture = "None"

def make_plot():
    plt.title("Real-Time 3D Path")
    plt.xlabel("X Position")
    plt.ylabel("Y Position")
    plt.plot(x_pos, y_pos)
    plt.xlim(-100,100)
    plt.ylim(-100,100)
    plt.grid(True)
while True:
    while arduino.in_waiting == 0:
        pass
    line = arduino.readline().decode('utf-8').strip()
    values = line.split(',')
    if len(values) == 4:
        try:
            ax = int(values[0]) / 16384.0  # Convert to g
            ay = int(values[1]) / 16384.0
            az = int(values[2]) / 16384.0 - 1.0  # Remove gravity
            gesture_status = int(values[3])
            if gesture_status == 1:
                last_gesture = "Circle Gesture"
            else:
                last_gesture = "None"
            print(f"Detected Gesture: {last_gesture}")
            vx += ax * 9.81 * dt
            vy += ay * 9.81 * dt
            vz += az * 9.81 * dt
            
            x_pos.append(x_pos[-1] + vx * dt)
            y_pos.append(y_pos[-1] + vy * dt)
            z_pos.append(z_pos[-1] + vz * dt)
            # Limit buffer size
            if len(x_pos) > 200:
                x_pos.pop(0)
                y_pos.pop(0)
                z_pos.pop(0)
            drawnow(make_plot)
        except:
            continue