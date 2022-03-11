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
    while port.readlines():
      reception = serialPull()
      Run(reception)
   return

def parlerBus():
  dureeAttente = 1000
  nombreEssais = 2
  for i in range(nombreEssais): 
    port.write("A")
    time.sleep(dureeAttente)
    reception = serialPull()
    if reception == "R": 
      port.write(messageBus)
      messageBus=""
      break
  return

def Run(instruction):
  if instruction != "": 
    print("instruction: "+instruction+". ")
    print("taille de l'instruction: "+str(len(instruction))+". ")
    separator = '_'
    posFin = instruction.index(separator)
    numFonction = int(substr(0, posFin))
    chaîneArguments = instruction.substr(posFin)
    print("fonction n: "+str(numFonction)+". ")
    print("chaîne d'argument: "+chaîneArguments+". ")
    numArg = 0
    for i in range(len(chaîneArguments)): 
      if chaîneArguments[i] == separator: 
        numArg+=1
    print("Il y a "+numArg+" arguments. ")
    string argument[numArg]
    for i in range(numArg): 
      chaîneArguments = substr(chaîneArguments)
      posFin = chaîneArguments.index(separator)
      argument[i] = chaîneArguments.substr(0, posFin)
      chaîneArguments = chaîneArguments.substr(posFin)
      print("argument: "+argument[i]+". ")

    match numFonction:
      case 1: 
        print("Youpi !")
      case 2: 

def serialPull(): 
  string phrase
  terminator = ';'
  for i in range(200): 
    phrase[i] = port.read()
    if phrase[i]==terminator: 
      break
  posFin = phrase.index(char(10))
  if posFin != -1: 
    phrase.remove(posFin)
  return phrase

def serialPush(string txt): 
  print("Coucou, nous sommes dans serialPush(). ")
  longueur = len(txt)
  if longueur != 0: 
    message[longueur+1]
    txt=list(txt)
    print("Texte à transmettre: "+txt+" de longueur "+str(longueur)+". ")
    print("Texte envoyé: "+str(message))
    port.write(message, longueur)
    print("     <-- C'est fait. ")
  return
