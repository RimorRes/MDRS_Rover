# Créé par hpooeler r, le 01/01/2023 en Python 3.7
import serial
import time

timer0 = time.time()
duree = 60*30 # en secondes
chemin = "C:/Users/hpooeler r/Documents/Lycée/2022-2023/aéro/mission 2023/"
nomFichier = "output.txt"
cheminComplet = chemin + nomFichier
print(cheminComplet)

nomPortSerie=input("Nom du port série auquel est connecté le Arduino ? (par défaut : COM3) ")
if nomPortSerie is "":
    nomPortSerie = "COM3"

print("Ouverture port série (en 9600 bauds).")
try:
    with serial.Serial(port=nomPortSerie, baudrate=9600, timeout=1) as portSerie:
        print(portSerie)
        print("Ouverture fichier.")
        with open(cheminComplet, "a") as fichier:
            print("Tout va bien. Voici les données.")
            ## c'est là que ça se passe
            while time.time()-timer0 < duree:
                # lecture port série
                data = portSerie.readline().decode()
                if data != '':
                    print(data, end='') ## pour voir en direct ce qui arrive
                    # écriture fichier
                    if data[-1] == '\n':
                        data=''.join(data[i] for i in range(len(data)-1)) ##on enlève le dernier caractère
                    fichier.write(data)
except serial.serialutil.SerialException:
    print("Désolé, mais nous n'avons pas pu ouvrir le port série.")
    print("Merci de vérifier vos branchement.")
except:
    print("Erreur imprévue lors du transfert des données du port série vers le fichier.")
else:
    print("transfert des données : OK")
