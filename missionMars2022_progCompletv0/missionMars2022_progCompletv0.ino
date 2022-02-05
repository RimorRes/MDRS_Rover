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
// Gestion des moteurs : gérer la direction (les servo-moteurs qui pivotent entre position droite et position tourner sur place)
// implémenter le GPS --> fin du setup() et loop() et Run() (code 2)
// mesure de distance à l'obstacle --> finir implémenter dans loop() si on veut une réaction automatique, comme l'arrêt
// mesure de température interne --> finir implémenter dans loop() si on veut une réaction automatique, comme l'arrêt
// finir impléméntation dictionnaire des instruction (fonction Run() ci-dessous à la fin) concernant le code 6 et le code 2
// 
// specifications.h : initialiser centreRepere d'après la carte (valeur actuelle = pour jouer)
// specifications.h : initialiser cheminParDefaut (valeur actuelle = pour jouer)
// 
// setup()
// décider du mode d'attente pour le port série USB : while() ou non ? (voir commentaire dans le code)
// décider du mode d'initialisation du centre du repère (voir commentaire dans le code)
//
// loop()
// décider du temps d'attente de fin de boucle
//
// tests à faire
// comm série avec le Raspberry(exécute-t-on bien les ordres ?) tester les codes 1, 5, 6, 7
// en particulier retester antenne RF 24 (code 1) sur un helloWorld juste pour voir si spec OK
// tests de calibration des moteurs : sens de rotation de chacun, et vitesse rotation (ajuster les spec)
 
/*********************************************
PARAMETRAGE DU COMPORTEMENT AVEC L'UTILISATEUR
***********************************************/
// commenter ce qui suit pour économiser l'espace sur le microcontroleur
//#define AFFICHAGE   // Pour indiquer qu'il y aura une sortie sur la console série, idem dans les modules. Ca rend le code bavard.
#define TESTS //Pour des fonctions supplémentaires, liées aux tests

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
Moteur moteurAVD(myPINs.PIN_moteurAVD_1, myPINs.PIN_moteurAVD_2, myPINs.PIN_mesure_tension_alim);
Moteur moteurAVG(myPINs.PIN_moteurAVG_1, myPINs.PIN_moteurAVG_2, myPINs.PIN_mesure_tension_alim);
Moteur moteurARD(myPINs.PIN_moteurARD_1, myPINs.PIN_moteurARD_2, myPINs.PIN_mesure_tension_alim);
Moteur moteurARG(myPINs.PIN_moteurARG_1, myPINs.PIN_moteurARG_2, myPINs.PIN_mesure_tension_alim);

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
  OK_init_moteurs = moteurAVD.init_moteur();
  if(!OK_init_moteurs){  // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du moteur avant droit\n");
  }
  OK_init_moteurs = moteurAVG.init_moteur();
  if(!OK_init_moteurs){  // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du moteur avant gauche\n");
  }
  OK_init_moteurs = moteurARD.init_moteur();
  if(!OK_init_moteurs){  // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du moteur arrière droit\n");
  }
  OK_init_moteurs = moteurARG.init_moteur();
  if(!OK_init_moteurs){  // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du moteur arrière gauche\n");
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

  //Point pointGPS = Point(???, ???); // remplacer par les valeurs issues du GPS, x et y en mètres
  // centreRepere = Point(pointGPS);
  cheminSuivi = chemin.getPointDebut().toString();
  
}

/*******************************************************************************
            LOOP()
********************************************************************************/
void loop()
{
  // tests
  // -----
  
  // test température
  if (OK_init_Tint){
    msg_alerte = msg_alerte.concat(test_temp_int(sensorTinterne, rover_config.Tint_min, rover_config.Tint_max));
  }
  
  // test distance obstacle
  int dist_1 = ultrasonic_1.Ranging(CM);
  //int dist_2 = ultrasonic_2.Ranging(CM);
#ifdef AFFICHAGE
  Serial.print("distance devant : ");
  Serial.print(dist_1);
  Serial.println(" cm");
/*  Serial.print("distance vers le bas : ");
  Serial.print(dist_2);
  Serial.println(" cm");*/
#endif
  if(dist_1<rover_config.distanceMin){ // obstacle trop proche
    messageBus += "5_" + String(dist_1) + ";";  // transmission de la distance, même sans requête
  }
  
  // test tension alimentation
  float tension = getTensionAlim();
  //Serial.println("tension d'alimentation moteur = " + String(tension) + " V");
  if (tension<rover_config.tensionCodeMin){
    msg_alerte += "tension d'alimentation anormalement basse\n";
  } else if (tension>rover_config.tensionCodeMax){
    msg_alerte += "tension d'alimentation anormalement haute\n";
  }
  
  // intermède communication, si nécessaire
  if (messageBus != ""){parlerBus();}

  // déplacement
  // -----------
  //chemin.setPointFin(Point(20,-5));
  Point pointGPS = Point(0, 0); // remplacer par les valeurs issues du GPS, x et y en mètres
  Point positionCarte = pointGPS.relative(rover_config.centreRepere); // pour avoir la position relative
//  Serial.println(positionCarte.affichage());
//  Serial.println(chemin.getPointParNumero(1).affichage());
  chemin.actualiser(positionCarte); // situe la position actuelle par rapport aux positions intermédiaires du chemin
  //chemin.recalculer();  // à décommenter si on veut un recalcul systématique des points intermédiaires du chemin : plus robuste et plus long
  String chaineOrdresMarche = chemin.goToNext();  // génère les ordres de marche pour atteindre le point intermédiaire suivant
//  Serial.println(chaineOrdresMarche);
  RunChaineOrdres(chaineOrdresMarche);  // exécution des ordres de marche
  
  // intermède communication, si nécessaire
  if (messageBus != ""){parlerBus();}

  delay(1000);  // Wait 1000ms // bien le temps des tests, mais ça peut être réduit ensuite. jusqu'à zéro ? déjà 100 serait plus fluide.
}

/*******************************************************************************
            FONCTIONS UTILES
********************************************************************************/
void serialEvent(){ // appeler automatiquement par Arduino en fin de loop() s'il y a du nouveau sur le bus série
  lectureBus();
  // intermède communication, si nécessaire
  if (messageBus != ""){parlerBus();}
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
      Serial.print("instruction dans Run : "); Serial.println(INSTRUCTION); 
      Serial.print("   taille instruction : "); Serial.println(String(INSTRUCTION.length())); 
#endif
      char separator='_'; // caractère de séparation entre fonction et arguments, ou entre arguments
      
      // récupération du code de la fonction appelée
      int posFin = INSTRUCTION.indexOf(separator); // repère le passage de la fonction aux arguments de la fonction
      int numFonction = INSTRUCTION.substring(0, posFin).toInt(); // lit le début de l'instruction comme un entier codant la future fonction à exécuter. Du coup, on aurait pu coder avec parseInt().
      String chaineArguments = INSTRUCTION.substring(posFin);
#ifdef AFFICHAGE
      Serial.print("   fonction n° : "); Serial.println(String(numFonction));
      Serial.print("   chaine d'arguments : "); Serial.println(chaineArguments);
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
        int posFinArg = chaineArguments.indexOf(separator, 1);  // on repère la fin de l'argument
        String copie = chaineArguments;
        arguments[i] = copie.substring(1, posFinArg);
        String resteArguments = chaineArguments.substring(posFinArg);
        chaineArguments = resteArguments;
#ifdef AFFICHAGE
        Serial.print("   argument : "); Serial.println(String(arguments[i]));
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
          messageBus += "2_" + chemin.getPointActuel().toString();  // ";" déjà inclus dans toString()
          // remplacer le code ci-dessus par le code ci-dessous dès qu'on aura implémenté le GPS
          // messageBus += "2_" + String("") + "_" + String("") + ";";  // compléter avec x puis y, en mètres, valeurs décimales avec un point
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
        case 5: {// requête de transmission des distances à l'obstacle
          int dist = ultrasonic_1.Ranging(CM);
          messageBus += "5_" + String(dist) + ";";
          break;
        }  
        case 6: {// requête de transmission des données des capteurs pour la cartographie
          String message = "6_";
          message += String(read_temperature(sensorTinterne));  // température, en degrés, décimal avec un point
          message += String("");  // pression, unité ?, décimal avec un point
          message += String("");  // vitesse vent (norme du vecteur), unité ?, décimal avec un point
          message += String("");  // angle horizontal direction vent, en degrés ? par rapport à [Ox) sens trigo ?, décimal avec un point
          messageBus += message + ";";
          break;
        }
        case 7: {// réponse à la requête de transmission de l'orientation du rover
          directionRover = arguments[0].toInt();  // orientation horizontale du rover, en degrés par rapport à [Ox) sens trigo , valeur entière
          break;
        }
        case 11:  // ordre de marche : avancer
          successionOrdresMarche += INSTRUCTION + ";";
          avancerTous(1, arguments[0].toFloat()); // arguments[0].toFloat() est la distance en mètres
          break;
        case 12:  // ordre de marche : reculer
          successionOrdresMarche += INSTRUCTION + ";";
          avancerTous(2, arguments[0].toFloat()); // arguments[0].toFloat() est la distance en mètres
          break;
        case 13:  // ordre de marche : tourner à droite
          successionOrdresMarche += INSTRUCTION + ";";
          // ici le code pour le servomoteur. Tourner au max en position "sur place".
          tournerSurPlace('D', arguments[0].toFloat()); // arguments[0].toFloat() est l'angle en degrés
          // ici le code pour le servomoteur. Revenir en position "normale".
          break;
        case 14:  // ordre de marche : tourner à gauche
          // ici le code pour le servomoteur. Tourner au max en position "sur place".
          tournerSurPlace('G', arguments[0].toFloat()); // arguments[0].toFloat() est l'angle en degrés
          // ici le code pour le servomoteur. Revenir en position "normale".
          break;
// ce qui suit est à but de test
#ifdef TESTS
        case 101:
          Serial.println("Youpi !"); break;
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
#endif
        default: // Case of unmatched function. 
#ifdef AFFICHAGE
            Serial.println("Ca serait gentil de me donner des instructions que je comprends !");
#endif
            return; // Panic. Something better to do ?
      } // fin du  : switch(numFonction)
    } // fin du : if (INSTRUCTION != "")
}

// la même pour le cas où il peut y avoir plusieurs instructions à la chaîne
void RunChaineOrdres(String INSTRUCTIONS){
  int numOrdres=1;
  for (int i=0;i<INSTRUCTIONS.length();i++){ // on compte les arguments
    if (INSTRUCTIONS[i] == ';'){numOrdres++;}
  }
  for (int i=0;i<numOrdres;i++){
    int posFin = INSTRUCTIONS.indexOf(';');
    String ordre = INSTRUCTIONS.substring(0,posFin);
    INSTRUCTIONS = INSTRUCTIONS.substring(posFin+1);
#ifdef AFFICHAGE
    Serial.println(INSTRUCTIONS);
#endif
    Run(ordre);
  }
  return;
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
