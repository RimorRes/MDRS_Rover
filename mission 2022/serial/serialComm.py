# INSTALL PYSERIAL: python -m pip install pyserial
# LIST OF AVAILABLE PORTS: python -m serial.tools.list_ports

import serial
import time # to use sleep()

COM_PORT = 'COM3'
BAUDRATE = 9600
TIMEOUT = 1 # 1000
WRITETIMEOUT = 1 # 1000
port_serie = serial.Serial(COM_PORT)##, BAUDRATE, TIMEOUT, WRITETIMEOUT) # modifiable : dev/ttyUSB0 ?
port_serie.baudrate = 9600
port_serie.timeout = 1
port_serie.write_timeout = 1
time.sleep(1.5) # argument may be a float, and experiment says >=1.5
print("Le port série est ouvert.")

messageBus = "default"

def serialPull():
  """   lit sur le port série
        lit les caractères un par un jusqu'à constituer une phrase
        une phrase est terminée par un caractère prédéfini

        renvoit la phrase comme chaîne de caractères,
           sans le caractère de terminaison
  """
  phrase=''
  nombreCarMax = 200    # nombre de caractères max dans une phrase
  terminator = ';'  # caractère de terminaison prévu au protocole
  for i in range(nombreCarMax):
    car = port_serie.read().decode()    # lit un seul caractère, et l'interprète comme ascii
    if car not in [terminator,'']:  # \r\n : juste le temps d'avoir un bon automate en face
      if car == '': print("J'ai reçu une phrase mal terminée.")
      phrase += car
    else:
      break # donc le caractère de terminaison n'est pas dans la chaîne
            # s'il y en a plusieurs, ça peut bugger !
  return phrase

def serialPush(txt):
  """   écrit une phrase sur le port série

        txt est le texte à écrire (chaîne de caractère)
            txt doit tout contenir, y compris le caractère de terminaison
        la fonction ne renvoit rien
  """
  if len(txt) != 0:
    port_serie.write(txt.encode())
  return

def lectureBus():
  """ détecte une demande de parole
      puis accorde la parole et attend une seconde
      puis lit toutes les phrases contenues dans le message (10 secondes max)
      les phrases sont exécutées au fur et à mesure de leur lecture
  """
  ##print("début lecture bus")
  dureeAttente = 1
  dureeMaxEcoute = 10   # durée max admise pour un message
  reception = serialPull()
  ##print("   Première lecture : reception = ", reception)
  if reception == "A":  # Le protocole prévoit que c'est la seule chose qu'on
                        # puisse recevoir à ce stade
    ##print("                      Je nettoie le bus.")
    while serialPull() == "A": continue # On nettoie le bus.
    ##print("                      Je réponds 'R'.")
    serialPush("R;")    # On répond qu'on est prêt.
    time.sleep(dureeAttente)    # On laisse le temps de parler.
    instantInitial = time.time()
    while time.time() < instantInitial + dureeMaxEcoute:    # protège de bugs côté émetteur
        reception = serialPull()    # on récupère une phrase
        ##print("      On reçoit : ", reception)
        if reception == '': # rien = c'est fini
            ##print("         C'est fini.")
            break
        if reception == "A":    # ne devrait pas arriver si l'émetteur attend assez entre deux "A".
            print("erreur de protocole de communication : demande de parole redondante")
            ##print("                      Je réponds 'R'.")
            serialPush("R;")
            time.sleep(dureeAttente)
            continue
        Run(reception)
  elif reception != "":
    print("erreur de protocole : message sans demande de parole")
  return

def parlerBus():
  """   demande la parole
        attend l'autorisation pour parler
        transmet tel quel le contenu de la variable globale messageBus
            (ne pas oublier le caractère de terminaison)
        accède à messageBus pour le vider
  """
  global messageBus # la variable est réinitialisée après utilisation
  dureeAttente = 1
  nombreEssais = 2
  ##print("essai parole")
  for i in range(nombreEssais):
    ##print("   essai n° ", i+1)
    serialPush("A;")    # demande parole
    time.sleep(dureeAttente)    # on attend que ça réagisse
    reception = serialPull()    # on lit une phrase
    if reception == "R":    # la seule réponse prévue au protocole :
                            # l'autre est prêt. Sinon on réessaie.
      ##print("   OK, ça a répondu. Je parle. Nombre d'essais : ", i+1)
      serialPush(messageBus)    # on délivre le message
      messageBus="" # on vide le tampon
      break
  ##print("   fin essai parole.")
  return

def Run(instruction):
  """ doc à écrire !
  """
  print("Coucou, je suis dans Run, j'ai reçu '" + instruction + "'")
  return # Juste le temps de tout vérifier
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
    argument=[]
    for i in range(numArg):
      chaîneArguments = substr(chaîneArguments)
      posFin = chaîneArguments.index(separator)
      argument[i] = chaîneArguments.substr(0, posFin)
      chaîneArguments = chaîneArguments.substr(posFin)
      print("argument: "+argument[i]+". ")

    if numFonction==1:
        print("Youpi !")
    elif numFonction==2:
        print("Coucou")

messageBus = "Bonjour !;"
print("message à envoyer : ", messageBus)
parlerBus()
time.sleep(2)
lectureBus()

port_serie.close()

