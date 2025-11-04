import serial
import time

AUTHORIZED_TAG = "0013084287"
arduino = serial.Serial('COM3', 9600, timeout=1) 
print("Serial connection established.")
try:
    while True:
        print("\nPlease scan your card:")
        try:
            uid = input().strip()
        except EOFError:
            continue 
        if not uid: 
            continue
            
        print(f"Card Scanned: {uid}")

        if uid == AUTHORIZED_TAG:
            print("RFID authorized.")
            time.sleep(1)
            
            print("Please do a circular motion. You have 5 seconds...")
            
            motion_detected = False  
            start_time = time.time() 
            duration = 5             
            
            try:
                arduino.flushInput() 
                while time.time() - start_time < duration:
                    if arduino.in_waiting > 0:
                        line = arduino.readline().decode('ascii').strip()
                        if line: 
                            print(f"Received motion data: {line}")
                            try:
                                value = int(line)
                                if value == 1:
                                    motion_detected = True 
                                    break 
                            except ValueError:
                                print(f"Warning: Received non-integer data: '{line}'")
                    time.sleep(0.05) 
                if motion_detected:
                    print("Motion verified. Access granted.")
                    arduino.write(b'a') 
                else:
                    print("Motion not detected in time. Access denied.")
                    arduino.write(b'b')
            except Exception as e:
                print(f"An error occurred during motion detection: {e}")
                arduino.write(b'b')
        else:
            print("UID unauthorized. Access denied.")
            arduino.write(b'b')
            
except KeyboardInterrupt:
    print("\nProgram terminated.")
    arduino.close() 
    print("Serial connection closed.")