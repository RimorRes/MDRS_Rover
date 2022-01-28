# INSTALL PYSERIAL: python -m pip install pyserial
# LISTS AVAILABLE PORTS: python -m serial.tools.list_ports

import serial

port= serial.Serial('dev/ttyUSB0', 9600, timeout=1000) # modifiable

def serialPush(txt): # txt must be: function_argument; NO GRAMMAR VERIFICATION. 
  serial.write(txt)

def serialPull(None):
  return read_until(expected=';', size=None)
