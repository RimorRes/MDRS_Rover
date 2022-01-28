# Serial communication between Arduino and Raspberry Pi. 
# The exact same functions as in the "serial.ino". 

#FX;ARG;;

#!/usr/bin/env python3
import serial

# ERRORCODES
SUCCESS=hex(0)
ERROR_SERIAL_COMMUNICATION_FAIL=hex(1)

# Set serial port
arduino=serial.Serial()
arduino.baudrate=19200 #?
arduino.port='COM1' # Reconfigure later?

def serialPull(TIMEOUT): 
    RETURN=[]
    for i in range(TIMEOUT):
        RETURN[i]=arduino.read()
        if RETURN[i]==";" and RETURN[i-1]==";":
            break
    return RETURN

def serialPush(TXT):
    arduino.write(TXT)
    return 0

def Run(INSTRUCTION):
    FX=""
    ARG=""
    INDEX=0
    for i in range(INSTRUCTION):
        if INSTRUCTION[i]!=";" and INSTRUCTION[i+1]!=";":
            FX[i]=INSTRUCTION[i]
        else:
            INDEX=i 
            break
    for i in range(INSTRUCTION):
        if INSTRUCTION[i]!=";" and INSTRUCTION[i]!="!":
            ARG[i]=INSTRUCTION[i]
        else:
            break
    if FX=="push":
        serialPush(ARG)
    else: 
        serialPush(ERROR_SERIAL_COMMUNICATION_FAIL)
