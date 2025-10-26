import serial #get serial port from arduino

ser = serial.Serial('COM3', 9600,timeout=2) #get data from arduino

try:
    while True:
        value = ser.readline().decode('ascii').strip()
        potValue = int(value) #establish it as integer
        print("Potentiometer Value:", potValue)
        if potValue > 511: # turn on LED
            ser.write(b'f')
        elif potValue < 511: #turn off LED
            ser.write(b's')
    time.sleep(1)
    
except KeyboardInterrupt: ## press Ctrl + c to stop
    ser.close()
    print("Serial connection closed.")