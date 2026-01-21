import os
import time
from smbus2 import SMBus
import subprocess
from gpiozero import Button 
from pyhuskylens import HuskyLens, ALGORITHM_OBJECT_RECOGNITION

# --- CONFIGURATION ---
UART_PORT = '/dev/serial0'
BAUD_RATE = 115200
I2C_BUS = 1
ESP32_ADDR = 0x08 # Connect to ESP32 Slave

btn_sos = Button(17) 
btn_voice = Button(27)
time.sleep(15)


# Connect to HuskyLens
hl = HuskyLens(1)
hl.set_alg(ALGORITHM_OBJECT_RECOGNITION)
try:
    bus = SMBus(I2C_BUS)
    print("Done")
except:
    print(f"I2C error {e}")
    
def speak(text): # Calling for the subprocess to enable TTS in Rasp Pi 5
    print(f"Master saying: {text}")
    subprocess.Popen(["espeak-ng", "-v", "en-us+f2", "-s", "140", text])

# --- FUNCTIONS ---
def manual_sos():
    speak("Emergency button pressed. Notifying family.")
    try:
        bus.write_byte(ESP32_ADDR,1) #sending number 1 to esp32 for further processing
        print("I2C Command Sent:SOS")
    except Exception as e:
        print(f"I2C ERROR : {e}")


def voice_report():
    blocks = hl.get_blocks()
    people = [b for b in blocks if b.ID == 1] #This is to detect human
    count = len(people)
    msg = f"There are {count} people in front." if count > 0 else "Path is clear."
    speak(msg)

speak("Welcome to intellicane")
try:
    while True:

        # 2. Check Buttons
        if btn_sos.is_pressed:
            manual_sos()
            time.sleep(1) # Simple debounce
            
            
        if btn_voice.is_pressed:
            voice_report()
            time.sleep(1)
        

        time.sleep(0.05)

except KeyboardInterrupt:
    print("System Shutdown.")
