# INSTALL PYSERIAL: python -m pip install pyserial
# LIST OF AVAILABLE PORTS: python -m serial.tools.list_ports

import serial

port= serial.Serial('dev/ttyUSB0', 9600, timeout=1000) # modifiable

def serialPush(msg): # txt must be: function_argument; NO GRAMMAR VERIFICATION. 
  serial.write(msg)

def serialPull(None):
  return read_until(expected=';', size=None)

def Run(instruction): 
  if instruction != "": 
    separator='_'
    posFin=instruction.index(separator)
    print("       instruction: "+instruction+". ")
    print("       taille: "+str(len(instruction))+". ")
    function=''
    for i in range(posFin): 
      function[i]=instruction[i]
    print("       fonction: "+function+". ")
    argument=''
    for i in range(posFin-len(function): 
      argument[i]=instruction[i]
    argNum=0
    for i in range(len(argument)): 
      if argument[i]==separator: 
        numArg+=1
    print("       nombres d'arguments: "+numArg+". ")
    print("       arguments: "+argument+". ")
    match function: 
      case '': 
