# INSTALL PYSERIAL: python -m pip install pyserial
# LIST OF AVAILABLE PORTS: python -m serial.tools.list_ports

import serial
import time

port= serial.Serial('dev/ttyUSB0', 9600, timeout=1000) # modifiable

def lectureBus():
  dureeAttente = 1000
  reception = serialPull()
  if reception == "A": 
    port.write("R")
    time.sleep(dureeAttente)
    while /*available*/: ;;; pas d'équivalent
      reception = serialPull()
      Run(reception)
   return

def parlerBus():
  dureeAttente = 1000
  nombreEssais = 2
  for in in range(nombreEssais): 
    port.write("A")
    time.sleep(dureeAttente)
    reception = serialPull()
    if reception == "R": 
      port.write(messageBus)
      messageBus=""
      break
  return

def Run()
