import serial #using serial port
import matplotlib.pyplot as plt #to plot the graph
import time #for delay in coding

ser = serial.Serial('COM3', baudrate = 9600,timeout=2) #to get information from arduino
plt.ion() #real time plotting
fig, ax = plt.subplots() #function for plots
x_value, y_value = [], [] #storing the data into x and y value

try: #used to ignore error and execute command
    while True:
        ser.write(b'f') #send byte 'f' to arduino
        arduinoData = ser.readline().decode('ascii').strip()
        if arduinoData:
            data = arduinoData.split(',') #split data from ',' to two data
            if len(data) == 2:
                potValue = int(data[0]) #potentiometer data
                x_value.append(len(x_value)) #assign data to x value
                y_value.append(int(potValue)) #assign data to y value
                angle = int(data[1]) #servo data
                
                print(f"Potentiometer: {potValue}  |  Servo Position: {angle}") #print into 2 separate data
                
                ax.clear() #clear the plot
                plt.title("Potentiometer Reading")
                plt.xlabel("Integer")
                plt.ylabel("Potentiometer Value") 
                plt.pause(0.1)
    time.sleep(1) #allow the code to run smoother and slower

except KeyboardInterrupt: #this function will happen if 'Ctrl + c" is pressed
    ser.write(b's') #send byte 's' to arduino
    ser.close()
    plt.ioff()
    plt.show()
    print("Serial connection closed.")