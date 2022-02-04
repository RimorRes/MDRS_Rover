/*********************************************************************
  module Arduino de la mission Mars 2022, iOSubOrbital
  Programme père
    
  Version originale iOSubOrbital 2021-2022     
 **********************************************************/
/****
TO DO
*****/
// Le codage des classes de spec est pourri : il faudrait passer la valeur des "constantes" lors de l'initialisation. Tant pis.
// 
// Gestion des moteurs : gérer la direction
// Gestion des moteurs : savoir quoi allumer pour avancer ou tourner
// Ecrire la boucle principale
// implémenter le GPS --> fin du setup() et loop()
// mesure de distance à l'obstacle --> à implémenter dans loop()
// mesure de température interne --> finir implémenter dans loop()
// finir impléméntation dictionnaire des instruction (fonction Run() ci-dessous à la fin)
// 
// specifications.h : initialiser centreRepere d'après la carte, ou faire avec le GPS : décommenter à la fin du setup()
// specifications.h : initialiser cheminParDefaut
// 
// setup()
// décider du mode d'attente pour le port série USB : while() ou non ? (voir commentaire dans le code)
//
// tests à faire
// classe Point
// classe Chemin
// comm série avec le Raspberry(exécu-t-on bien les ordres ?)
// retester antenne RF 24 sur un helloWorld juste pour voir si spec OK
// 
// (manoeuvre de dégagement : fait par le raspberry)
// (manoeuvre de contournement : idem)
 
/*********************************************
PARAMETRAGE DU COMPORTEMENT AVEC L'UTILISATEUR
***********************************************/
// commenter ce qui suit pour économiser l'espace sur le microcontroleur
#define AFFICHAGE   // Pour indiquer qu'il y aura une sortie sur la console série, idem dans les modules

/*************
SPECIFICATIONS
**************/
// Pour passer en paramètre des fonctions
#if !defined SPECIFICATIONS_H
  #include "specifications.h"  // module codé par nous
  #define SPECIFICATIONS_H
#endif 
PIN_spec myPINs; // définition des broches. utilisé dans moteurs.cpp
       // déclarer extern en tête de moteurs.cpp
Rover_spec rover_spec; // spécifications du rover (géométrie, valeurs limites, ...)
       // déclarer extern en tête de déplacement.cpp
       // déclarer extern en tête de moteurs.cpp
Rover_config rover_config;  // configuration du rover (paramètres du code)
       // déclarer extern en tête de déplacement.cpp
       // déclarer extern en tête de moteurs.cpp

/*******************************************************************************
            MATERIEL
********************************************************************************/
/*********************************
CONNEXION DES PERIPHERIQUES EN I2C
**********************************
brancher GND
brancher Vin sur 3,3 V ou 5 V selon le capteur
  BMP180 : 3,3 V (entre 1,8 et 3,6 V, surtout pas plus de 4,25 V)
  TMP102 : 3,3 V (entre 1,4 et 3,6 V, surtout pas plus de 4,0 V)
  ...
brancher SCL sur A5 (qui est le port SCL pour les Arduino Uno R3)
brancher SDA sur A4 (qui est le port SDA pour les Arduino Uno R3)
L'adresse 7 bits du BMP180 est Ox77.
*/

/***************
MODULES EXTERNES
****************/
/* capteur température interne */
#include <SparkFunTMP102.h>
//const int ALERT_PIN = myPINs.PIN_temp_alert; //Si on veut utiliser la sortie alert
TMP102 sensorTinterne; // donnera la température interne à 0,0625°C près
/* On peut changer l'adresse par défaut, qui est 0x48 = 1001000.
 * Il faut connecter la broche ADD0 (par défaut : sur la masse)
 *    mass : 0x48
 *    VCC - 0x49
 *    SDA - 0x4A
 * mais en fait il n'y a pas la broche ADD0 sur le nôtre, donc l'adresse sera fixe à 0x48
 * On peut aussi modifier le bus utilisé avec E.g. sensor0.begin(0x49,Wire1)
 * It will return true on success or false on failure to communicate.
 */

/* détecteurs d'obstacle */
#include <Ultrasonic.h>
Ultrasonic ultrasonic_1(myPINs.PIN_detectObst1_Trig,myPINs.PIN_detectObst1_Echo); // le seul utilisé pour l'instant
// Ultrasonic ultrasonic_2(myPINs.PIN_detectObst2_Trig,myPINs.PIN_detectObst2_Echo);

/* communication série */
#include "serialComm.h" // module codé par nous

/* surveillance de la température interne du rover */
#include "temperatureInterne.h"  // module codé par nous

/* antenne RF */
#include <SPI.h>
#include <RF24.h>
#define tunnel  "PIPE1"       // On définit un "nom de tunnel" (5 caractères), pour pouvoir communiquer d'un NRF24 à l'autre
RF24 radio(myPINs.PIN_RF_CE, myPINs.PIN_RF_CSN);    // Instanciation du NRF24L01
const byte adresseAntenne[6] = tunnel;               // Mise au format "byte array" du nom du tunnel

/* moteurs */
#if !defined MOTEURS_H
  #include "moteurs.h"  // module codé par nous
  #define MOTEURS_H
#endif 
Moteur moteur1(myPINs.PIN_moteur1_1, myPINs.PIN_moteur1_2, myPINs.PIN_moteur1_3, myPINs.PIN_moteur1_4, myPINs.PIN_mesure_tension_alim);
Moteur moteur2(myPINs.PIN_moteur2_1, myPINs.PIN_moteur2_2, myPINs.PIN_moteur2_3, myPINs.PIN_moteur2_4, myPINs.PIN_mesure_tension_alim);

/*******************************************************************************
            GLOBALES
********************************************************************************/
/* déplacements */
#if !defined DEPLACEMENT_H
  #include "deplacement.h"  // module codé par nous
  #define DEPLACEMENT_H
#endif 
Point centreRepere = rover_config.centreRepere;
float directionRover = rover_config.directionInitiale; // déclarer extern en tête de déplacement.cpp
Chemin chemin = rover_config.cheminParDefaut;  // à initialiser !! // déclarer extern en tête de déplacement.cpp

/* indicateurs d'état */
boolean OK_init_Tint, OK_Tint;  // initialisation capteur température interne, état température interne
boolean OK_init_moteurs;  // initialisation moteurs
boolean OK_alim;  // tension alim
String msg_alerte = "tout va bien\n";
boolean goingHome = false; // déclarer extern en tête de déplacement.cpp

/* mémoire du rover */
String successionOrdresMarche = ""; // déclarer extern en tête de déplacement.cpp
String cheminSuivi = ""; // déclarer extern en tête de déplacement.cpp

/* mémoire tampon comm série USB */
String messageBus = ""; // déclarer extern en tête de SerialComm.cpp

/*******************************************************************************
            SETUP()
********************************************************************************/
void setup()
{
  // initialisation de la comm série USB (pour le raspberry)
  Serial.begin(9600);                       
  while(!Serial){;}                         // On attend que le port série soit disponible
// la ligne précédente est-elle à enlever lorsque ce ne sera plus le moniteur série ?
// On risque de paralyser le rover si la comm série ne fonctionne pas : remplacer par un temps d'attente comme 2000 ms par exemple ?
  delay(1000);
  Wire.begin(); //Join I2C Bus
  
  // initialisation du capteur de température interne
  OK_init_Tint = init_tmp102(sensorTinterne);
  if(!OK_init_Tint){
    msg_alerte.concat("problème d'initialisation du capteur de température interne (tmp102)\n");
  }

  // initialisation des moteurs
  OK_init_moteurs = moteur1.init_moteur();
  if(!OK_init_moteurs){  // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du moteur 1\n");
  }
  OK_init_moteurs = moteur2.init_moteur();
  if(!OK_init_moteurs){  // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du moteur 2\n");
  }
  
  // initialisation de l'antenne RF
  radio.begin();                      // Initialisation du module NRF24
  radio.openWritingPipe(adresseAntenne);     // Ouverture du tunnel en ÉCRITURE, avec le "nom" qu'on lui a donné
  radio.setPALevel(RF24_PA_MIN);      // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  radio.stopListening();              // Arrêt de l'écoute du NRF24 (signifiant qu'on va émettre, et non recevoir, ici)


#ifdef AFFICHAGE
  Serial.print("état du rover à l'initialisation : ");
  Serial.println(msg_alerte);
  Serial.println("");
#endif

  Point pointGPS; // à mesurer
  // centreRepere = Point(pointGPS);
  cheminSuivi = chemin.getPointDebut().toString();
}

/*******************************************************************************
            LOOP()
********************************************************************************/
void loop()
{
  if (OK_init_Tint){
    msg_alerte = msg_alerte.concat(test_temp_int(sensorTinterne, rover_config.Tint_min, rover_config.Tint_max));
  }
  int dist_1 = ultrasonic_1.Ranging(CM);
  //int dist_2 = ultrasonic_2.Ranging(CM);
#ifdef AFFICHAGE
   Serial.print("distance devant : ");
   Serial.print(dist_1);
   Serial.println(" cm");
/*   Serial.print("distance vers le bas : ");
   Serial.print(dist_2);
   Serial.println(" cm");*/
#endif
  
  float tension = moteur1.getTensionAlim();
  //Serial.println("tension d'alimentation moteur = " + String(tension) + " V");
  
  // avancer('a', 1, 1000);
/*  avancerMetres(0.1);
  tournerSurPlaceDegres(1, 90); //dir: 1 clockwise 2 counterclockwise, angle: angle de rotation en degrés
  avancerMetres(0.2);
  tournerSurPlaceDegres(2, 90);
  avancerMetres(0.1);

  successionOrdresMarche.concat("Av_0.100;");
  successionOrdresMarche.concat("Tg_90.00;");
  successionOrdresMarche.concat("Av_0.200;");
  successionOrdresMarche.concat("Td_90.00;");
  successionOrdresMarche.concat("Av_0.100;");*/
/*  
#ifdef AFFICHAGE
  Serial.print("récapitulation du chemin suivi : ");
  Serial.println(successionOrdresMarche);
  Serial.print("état du rover : ");
  Serial.println(msg_alerte);
  Serial.println("");
#endif
*/  

  //messageBus = msg_alerte;
  if (messageBus != ""){parlerBus();}
  delay(1000);  // Wait 1000ms
}

/*******************************************************************************
            FONCTIONS UTILES
********************************************************************************/
void serialEvent(){ // appeler automatiquement par Arduino en fin de loop() s'il y a du nouveau sur le bus série
  lectureBus();
  return;
}

/**************************
LA FONCTION D'EXECUTION CLE
***************************/

// déclarer en tête de serialComm.cpp
void Run(String INSTRUCTION){ // Reads the instruction to call it after.
  // Une seule instruction lue à la fois. Appeler plusieurs fois au besoin.
    if (INSTRUCTION != ""){
#ifdef AFFICHAGE
      Serial.println("instruction dans Run : " + INSTRUCTION);
      Serial.println("   taille instruction : " + String(INSTRUCTION.length())); 
#endif
      char separator='_'; // caractère de séparation entre fonction et arguments, ou entre arguments
      
      // récupération du code de la fonction appelée
      int posFin = INSTRUCTION.indexOf(separator); // repère le passage de la fonction aux arguments de la fonction
      int numFonction = INSTRUCTION.substring(0, posFin).toInt(); // lit le début de l'instruction comme un entier codant la future fonction à exécuter. Du coup, on aurait pu coder avec parseInt().
      String chaineArguments = INSTRUCTION.substring(posFin);
#ifdef AFFICHAGE
      Serial.println("   fonction n° : " + String(numFonction));
      Serial.println("   chaine d'arguments : " + chaineArguments);
#endif
      
      // récupération des arguments
      int i=0; int numArg=0;
      for (i=0;i<chaineArguments.length();i++){ // on compte les arguments
        if (chaineArguments[i] == separator){numArg++;}
      }
#ifdef AFFICHAGE
      Serial.print("   Il y a "); Serial.print(numArg); Serial.println(" arguments.");
#endif
      String arguments[numArg]; // le tableau des arguments
      for(i=0;i<numArg;i++){
        chaineArguments = chaineArguments.substring(1); // on enlève le séparateur
        posFin = chaineArguments.indexOf(separator);  // on repère la fin de l'argument
        arguments[i] = chaineArguments.substring(0, posFin); // on extrait l'argument
        chaineArguments = chaineArguments.substring(posFin); // on enlève ce qui a été lu
#ifdef AFFICHAGE
        Serial.println("   argument : " + arguments[i]);
#endif
      }
      
      switch(numFonction){  // cf dico des instructions
        case 0: // code d'urgence
          goHome();  // retour base
          messageBus = "0"; // on annonce qu'on revient
          parlerBus();
          break;
        case 1: // requête de transmission d'un message via l'antenne
          emettreMessage(arguments[0]);
          break;
        case 2: // requête de transmission de la position actuelle via le bus série
          messageBus+="2_" + chemin.getPointActuel().toString();  // ";" déjà inclus dans toString()
          break;
        case 3: // requête de transmission du bilan d'activité du rover
          // attention, ça peut être long.
          switch (arguments[0].toInt()){
            case 1: // demande de transmission des messages d'alerte
              messageBus+="3_1_" + msg_alerte + ";"; break;
            case 2: {// demande de transmission de la succession des ordres de marche
              String texte = successionOrdresMarche;
              texte.replace(";", "\n");
              messageBus+="3_2_" + texte + ";"; break;
            }
            case 3: {// demande de transmission de l'historique du chemin suivi
              String texte = cheminSuivi;
              texte.replace(";", "\n");
              messageBus+="3_3_" + texte + ";"; break;
            }
            default:
              break;
          }
          break;
        case 4: {// requête de modification du point cible, et donc du chemin
          Point pointFinNouveau = Point(arguments[0].toFloat(), arguments[1].toFloat());
          // la conversion se fait avec arrondi au centième, si la chaîne est trop longue, ça tronque
          chemin.setPointFin(pointFinNouveau);
          chemin.recalculer();
          break;
        }
        case 5: // requête de transmission des distances à l'obstacle
          ; break;
// ci-dessous les ordres de marche
//    deux caractères pour la nature de l'ordre de marche
//    un underscore
//    cinq caractères pour le paramètre numérique (à extraire)
// mais je pourrais assouplir, maintenant ?
        case 11:  // ordre de marche : avancer
          successionOrdresMarche += INSTRUCTION + ";";
          ;// ???
          break;
        case 12:  // ordre de marche : reculer
          successionOrdresMarche += INSTRUCTION + ";";
          ; break;
        case 13:  // ordre de marche : tourner à droite
          successionOrdresMarche += INSTRUCTION + ";";
          ; break;
        case 14:  // ordre de marche : tourner à gauche
          successionOrdresMarche += INSTRUCTION + ";";
          ; break;
// ce qui suit est à but de test
        case 101:
          Serial.println("Youpi !"); break;
        case 102:
          ;//noel(); break;
        case 103:
          if(numArg != 0){serialPush(arguments[0]);} break;
        case 104:
          if(numArg != 0){Serial.println(arguments[0]);} break;
        case 105:
          if(numArg != 0){
            String message = "";
            for (i=0;i<numArg;i++){
              message += arguments[i];
              message += " ";
            }
            Serial.println(message);
          } break;
        case 106:
          emettreMessage(msg_alerte); break;
        case 107:
          emettreMessage("123456789_123456789_123456789_123456789"); break;
        default: // Case of unmatched function. 
#ifdef AFFICHAGE
            Serial.println("Ca serait gentil de me donner des instructions que je comprends !");
#endif
            return; // Panic. Something better to do ?
      } // fin du  : switch(numFonction)
    } // fin du : if (INSTRUCTION != "")
}


/*************************
GROS BAZAR DE FONCTIONS...
**************************/

boolean emettreMessage(String message){
  int nbrCaracteresMax = 31; // déterminé empiriquement (36 en principe !)
  int nbrPhrases = int(message.length() / nbrCaracteresMax) +1;
  String phrase;
  for (int i=0;i<nbrPhrases;i++){
    phrase = message;
    int nbrCaracteres = min(nbrCaracteresMax, message.length());
    Serial.println("début : " + message + " nbr = " + String(nbrCaracteres));
    phrase = message.substring(0, nbrCaracteres); // on extrait la phrase à émettre
    phrase.concat('\n');
    Serial.println("   phrase : " + phrase);
    message = message.substring(nbrCaracteres); // et on l'enlève du message
    char msg[phrase.length()];     // Message à transmettre à l'autre NRF24 (32 caractères maxi, avec cette librairie)
    phrase.toCharArray(msg, sizeof(msg)); // mise en forme : tableau de caractères
    radio.write(&msg, sizeof(msg));     // Envoi de notre message 
    //Serial.write(msg, sizeof(msg));
  }
}
