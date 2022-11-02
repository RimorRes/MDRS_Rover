/*********************************************************************
  module Arduino de la mission Mars 2023, iOSubOrbital
  version 1 : ultra-simplifiée.
  liste des évolutions prévues, par priorité croissante :
  - version sans GPS, ni raspberryPI : implémentation non-prévue, même à long terme
  - version sans contrôle de la température interne : implémentation prévue
  - version sans contrôle de la tension de la batterie : implémentation prévue
  - version sans boîte noire : implémentation prévue à moyen terme
  - version sans gestion des obstacles : prochaine amélioration prévue
  - version sans acquisition de données : évolution prioritaire, essentielle à la mission
  
  Programme père. listes des modules appelés : voir ci-dessous.
  Version originale iOSubOrbital 2022-2023
 **********************************************************/

/************************************
 * LISTE DES MODULES APPELES
 * deplacement : gère la trajectoire, et ses modifications
 * (gps : conversions de données utiles pour le positionnement)
 * moteurs : gère les moteurs propulsion
 * servoMoteurs : gère les servomoteurs direction
 * specifications : paramètres chiffrés pour la configuration
 * temperatureInterne : gère le thermomètre interne
 ************************************/
 
/**********************
 * PLAN DU PROGRAMME :
 * ------------------- 
 *  PRELIMINAIRES  
 *    TO DO : commentaire à destination des codeurs
 *    PARAMETRAGE DU COMPORTEMENT AVEC L'UTILISATEUR
 *    SPECIFICATIONS : charge les spécifications du rover (branchements, cotes, paramètres de configuration)
 *    MATERIEL : appel des modules et variables globales pour gérer le matériel
 *        capteur température interne du rover
 *        détecteurs d'obstacle
 *        antenne RF
 *        moteurs (propulsion)
 *        servomoteurs (direction)
 *    CONTROLE DU ROVER : variables globales pour coder le comportement du rover
 *        indicateurs d'état
 *        déplacements
 *        mémoire du rover
 *        mémoire tampon comm série USB
 *  --> SETUP() : la fonction qu'Arduino exécute une unique fois, au tout début
 *        initialisation de la comm série I2C (pour le capteur de température interne)
 *        initialisation du capteur de température interne
 *        initialisation des moteurs
 *        initialisation des servomoteurs
 *        initialisation de l'antenne RF
 *        initialisation du chemin à suivre
 *  --> LOOP() : la fonction exécutée en boucle infinie après setup() 
 *      (puis serialEvent() s'exécute en fin de loop() s'il y a du nouveau sur le bus série)
 *        tests
 *        test température
 *        test distance obstacle
 *        test tension alimentation
 *    GESTION DES SEQUENCES D'ORDRES : interprète les séquences d'ordres passés au rover et déclenche leur exécution
 *    GROS BAZAR DE FONCTIONS... : pas encore bien classé. actuellement : gestion de l'émission RF.
 *        boolean emettreMessage(String message)
 **********************/

/********
  TO DO : short term list of technical required implementations
*********/
// réviser les globales
// mesure de distance à l'obstacle --> finir implémenter dans loop() si on veut une réaction automatique, comme l'arrêt
// mesure de température interne --> finir implémenter dans loop() si on veut une réaction automatique, comme l'arrêt
// révision intégrale du dictionnaire des instruction (fonction Run() ci-dessous à la fin) concernant le code 6 et le code 2
// gestion de PointActuel dans addPoint() et removePoint (classe Chemin dans deplacement.cpp) modifiée : à retester
//
// setup()
// décider du mode d'initialisation du centre du repère (voir commentaire dans le code)
//
// loop()
// décider du temps d'attente de fin de boucle
//
// tests à faire
// en particulier retester antenne RF 24 (code 1) sur un helloWorld juste pour voir si spec OK
// tests de calibration des moteurs : vitesse rotation (ajuster les spec)
//
// gros problème sur la température interne à gérer. la lecture de température fait bugger.
//    <SparkFunTMP102.h> inclus deux fois ?
//
//  comment exporter les données ?
//  préparer un module de réception qui :
//  - reçoive les données
//  - les stockent dans un tampon
//  - vide le tampon dans un fichier de l'ordinateur lorsqu'un ordinateur est branché

/***********************************************
  PARAMETRAGE DU COMPORTEMENT AVEC L'UTILISATEUR
*************************************************/
// commenter ce qui suit pour économiser l'espace sur le microcontroleur
#define AFFICHAGE   // Pour indiquer qu'il y aura une sortie sur la console série, idem dans les modules. Ca rend le code bavard.

/***************
  SPECIFICATIONS
****************/
// Pour passer des paramètres de config aux fonctions
#if !defined SPECIFICATIONS_H
#include "specifications.h"  // module codé par nous
#define SPECIFICATIONS_H
#endif
// variables globales (de specifications.h)
// ------------------
//PIN_spec myPINs;
//  définition des broches.
//  utilisé dans moteurs.cpp
//  utilisé dans servoMoteurs.cpp
//Rover_spec rover_spec;
//  spécifications du rover (géométrie, valeurs limites, ...)
//  utilisé dans déplacement.cpp
//  utilisé dans moteurs.cpp
//  utilisé dans servoMoteurs.cpp
//Rover_config rover_config;
//  configuration du rover (paramètres du code)
//  utilisé dans déplacement.cpp
//  utilisé dans moteurs.cpp
//  utilisé dans servoMoteurs.cpp

/*********
  MATERIEL
**********/

/* CONNEXION DES PERIPHERIQUES EN I2C
  ************************************
  brancher GND
  brancher Vin sur 3,3 V ou 5 V selon le capteur
  BMP180 : 3,3 V (entre 1,8 et 3,6 V, surtout pas plus de 4,25 V)
  TMP102 : 3,3 V (entre 1,4 et 3,6 V, surtout pas plus de 4,0 V)
  ...
  brancher SCL sur A5 (qui est le port SCL pour les Arduino Uno R3)
  brancher SDA sur A4 (qui est le port SDA pour les Arduino Uno R3)
  L'adresse 7 bits du BMP180 est Ox77.
*/

/* capteur température interne du rover */

#include <SparkFunTMP102.h>
//const int ALERT_PIN = myPINs.PIN_temp_alert; //Si on veut utiliser la sortie alert
TMP102 sensorTinterne; // donnera la température interne à 0,0625°C près
#if !defined TEMPERATUREINTERNE_H
#include "temperatureInterne.h"  // module codé par nous
#define TEMPERATUREINTERNE_H
#endif
/* On peut changer l'adresse par défaut, qui est 0x48 = 1001000.
   Il faut connecter la broche ADD0 (par défaut : sur la masse)
      mass : 0x48
      VCC - 0x49
      SDA - 0x4A
   mais en fait il n'y a pas la broche ADD0 sur le nôtre, donc l'adresse sera fixe à 0x48
   On peut aussi modifier le bus utilisé avec E.g. sensor0.begin(0x49,Wire1)
   It will return true on success or false on failure to communicate.
*/

/* détecteurs d'obstacle */
#include <Ultrasonic.h>
Ultrasonic ultrasonic_1(myPINs.PIN_detectObst1_Trig, myPINs.PIN_detectObst1_Echo); // le seul utilisé pour l'instant
// Ultrasonic ultrasonic_2(myPINs.PIN_detectObst2_Trig,myPINs.PIN_detectObst2_Echo);

/* antenne RF */
#include <SPI.h>
#include <RF24.h>
#define tunnel  "PIPE1"       // On définit un "nom de tunnel" (5 caractères), pour pouvoir communiquer d'un NRF24 à l'autre
RF24 radio(myPINs.PIN_RF_CE, myPINs.PIN_RF_CSN);    // Instanciation du NRF24L01 // déclarer extern en tête de RF.cpp
const byte adresseAntenne[6] = tunnel;              // Mise au format "byte array" du nom du tunnel (6 caractère à cause du caractère de fin de chaîne)
#include "RF.h"

/* moteurs (propulsion) */
#if !defined MOTEURS_H
#include "moteurs.h"  // module codé par nous
#define MOTEURS_H
#endif
Moteur moteurAVD(myPINs.PIN_moteurAVD_1, myPINs.PIN_moteurAVD_2, myPINs.PIN_mesure_tension_alim);
Moteur moteurAVG(myPINs.PIN_moteurAVG_1, myPINs.PIN_moteurAVG_2, myPINs.PIN_mesure_tension_alim);
Moteur moteurARD(myPINs.PIN_moteurARD_1, myPINs.PIN_moteurARD_2, myPINs.PIN_mesure_tension_alim);
Moteur moteurARG(myPINs.PIN_moteurARG_1, myPINs.PIN_moteurARG_2, myPINs.PIN_mesure_tension_alim);

/* servomoteurs (direction) */
#if !defined SERVO_H
#include <Servo.h>
#define SERVO_H
#endif
Servo servoAVD, servoAVG, servoARD, servoARG; // les servomoteurs
#if !defined SERVOMOTEURS_H
#include "servoMoteurs.h"  // module codé par nous
#define SERVOMOTEURS_H
#endif
DirectionServo directionServo = DirectionServo(servoAVD, servoAVG, servoARD, servoARG);  // l'objet pour piloter les servomoteurs

/******************
  CONTROLE DU ROVER
*******************/
/* indicateurs d'état */
boolean OK_init_Tint, OK_Tint;  // initialisation capteur température interne, état température interne
boolean OK_init_moteurs;  // initialisation moteurs
boolean OK_alim;  // tension alim
String msg_alerte = "tout va bien\n";
boolean goingHome = false; // déclarer extern en tête de déplacement.cpp

/* déplacements */
#if !defined DEPLACEMENT_H
#include "deplacement.h"  // module codé par nous
#define DEPLACEMENT_H
#endif
float directionRover = rover_config.directionInitiale; // déclarer extern en tête de déplacement.cpp
Chemin chemin = rover_config.cheminParDefaut;  // déclarer extern en tête de déplacement.cpp

/* mémoire du rover */
String successionOrdresMarche = ""; // pour une mémoire de tous les ordres de marche exécutés
String cheminSuivi = ""; // déclarer extern en tête de déplacement.cpp

/* mémoire tampon comm série USB */
String messageBus = ""; // déclarer extern en tête de SerialComm.cpp

/* mémoire tampon comm RF */
String messageRF = "";

/*******************************************************************************
            SETUP()
********************************************************************************/
void setup()
{
/*  // initialisation de la comm série USB (pour le raspberry)
  Serial.begin(9600); // pour afficher correctement le GPS, il faudrait 115200 ? A voir <---------------------------------------
  delay(1000);
  Serial.println("coucou, setup()");*/
/*  while (!Serial) {
    ; // On attend que le port série soit disponible
  }
  // la ligne précédente est-elle à enlever lorsque ce ne sera plus le moniteur série ?
  // On risque de paralyser le rover si la comm série ne fonctionne pas : remplacer par un temps d'attente comme 2000 ms par exemple ?
*/

  // initialisation de la comm série I2C (pour le capteur de température interne)
  Wire.begin(); //Join I2C Bus

  // initialisation du capteur de température interne
  OK_init_Tint = init_tmp102(sensorTinterne);
  if (!OK_init_Tint) {
    msg_alerte.concat("problème d'initialisation du capteur de température interne (tmp102)\n");
  }
#ifdef AFFICHAGE
  Serial.println("fin de l'initialisation du thermomètre");
#endif

  // initialisation des moteurs
  OK_init_moteurs = moteurAVD.init_moteur();
  if (!OK_init_moteurs) { // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du moteur avant droit\n");
  }
  OK_init_moteurs = moteurAVG.init_moteur();
  if (!OK_init_moteurs) { // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du moteur avant gauche\n");
  }
  OK_init_moteurs = moteurARD.init_moteur();
  if (!OK_init_moteurs) { // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du moteur arrière droit\n");
  }
  OK_init_moteurs = moteurARG.init_moteur();
  if (!OK_init_moteurs) { // impossible avec le code actuel
    msg_alerte.concat("problème d'initialisation du moteur arrière gauche\n");
  }
#ifdef AFFICHAGE
  Serial.println("fin de l'initialisation des moteurs");
#endif

  // initialisation des servomoteurs
  directionServo.init();
  directionServo.positionNormale(); delay(500);
  //directionServo.positionSurPlace(); delay(500);
  //directionServo.positionNormale(); delay(500);
#ifdef AFFICHAGE
  Serial.println("fin de l'initialisation des servomoteurs");
#endif
  
  // initialisation de l'antenne RF
  radio.begin();                      // Initialisation du module NRF24
  radio.openWritingPipe(adresseAntenne);     // Ouverture du tunnel en ÉCRITURE, avec le "nom" qu'on lui a donné
  radio.setPALevel(RF24_PA_MIN);      // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  radio.stopListening();              // Arrêt de l'écoute du NRF24 (signifiant qu'on va émettre, et non recevoir, ici)
#ifdef AFFICHAGE
  Serial.println("fin de l'initialisation de l'antenne RF");
#endif

  // initialisation du chemin à suivre
  /*  setCentreRepere(convertSphereToPlan(gps.latitude, gps.longitude, Point(0,0,0))); // remplacer par les valeurs issues du GPS, x et y en mètres
    positionGPS = pointGPS(); // Ca vaut (0,0,0) si tout va bien.
    Serial.print("position GPS actuelle, par rapport à la carte : "); Serial.println(positionGPS.affichage());
    Serial.print("centre du repère : "); Serial.println(getCentreRepere().affichage());
    Serial.print("                   "); Serial.print(gps.lat); Serial.print(" N et "); Serial.print(gps.lon); Serial.println(" E");*/
  cheminSuivi = chemin.getPointDebut().toString();

  /*  Rover_config rover_config_test = rover_config;
    Serial.println("test sur les chemins");
    Serial.print("centre du repère : "); Serial.println(rover_config_test.getCentreRepere().affichage());
    Serial.print("limiteSO : "); Serial.println(rover_config_test.getLimiteSO().affichage());
    Serial.print("limiteNO : "); Serial.println(rover_config_test.getLimiteNO().affichage());
    Serial.print("limiteNE : "); Serial.println(rover_config_test.getLimiteNE().affichage());
    Serial.println("chemin par défaut : ");
    Serial.println(rover_config_test.cheminParDefaut.affichage());
    Serial.println("chemin actuel : ");
    Serial.println(chemin.affichage());
    chemin.addPoint(1, rover_config.getLimiteNO());
    Serial.println("chemin actuel : ");
    Serial.println(chemin.affichage());
    Serial.println("fin du test sur les chemins");
    Serial.println("");
  */

#ifdef AFFICHAGE
  Serial.print("état du rover à l'initialisation : ");
  Serial.println(msg_alerte);
  Serial.println("");
#endif
}

/*******************************************************************************
            LOOP()
********************************************************************************/

void loop()
{
  // tests
  // -----

  // test température
if (OK_init_Tint) {
//    msg_alerte = msg_alerte.concat(test_temp_int(sensorTinterne, rover_config.Tint_min, rover_config.Tint_max));
    //msg_alerte += test_temp_int(sensorTinterne, rover_config.Tint_min, rover_config.Tint_max);
    ;
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
  if (dist_1 < rover_config.distanceMin) { // obstacle trop proche
    messageBus += "5_" + String(dist_1) + ";";  // transmission de la distance, même sans requête
  }

  // test tension alimentation
  float tension = getTensionAlim();
  //Serial.println("tension d'alimentation moteur = " + String(tension) + " V");
  if (tension < rover_config.tensionCodeMin) {
    msg_alerte += "tension d'alimentation anormalement basse\n";
  } else if (tension > rover_config.tensionCodeMax) {
    msg_alerte += "tension d'alimentation anormalement haute\n";
  }

#ifdef AFFICHAGE
  Serial.println("Voici le message d'alerte : ");
  Serial.println(msg_alerte);
  Serial.println("fin du message");Serial.println(" ");
#endif

  messageRF += "Hello world !";
  String monOrdre ="";
  monOrdre += "1_"; monOrdre += messageRF; monOrdre += ";";
  Run(monOrdre);
  messageRF ="";

  delay(500);  // Wait 1000ms // bien le temps des tests, mais ça peut être réduit ensuite. jusqu'à zéro ? déjà 100 serait plus fluide.
} // FIN DE LOOP()

void serialEvent() { // appelé automatiquement par Arduino en fin de loop() s'il y a du nouveau sur le bus série
  return;
} // FIN DE serialEvent()

/********************************
  GESTION DES SEQUENCES D'ORDRES
*********************************/

// déclarer en tête de serialComm.cpp
void Run(String INSTRUCTION) { // Reads the instruction to call it after.
  // Une seule instruction lue à la fois. Appeler plusieurs fois au besoin.
  if (INSTRUCTION != "") {
#ifdef AFFICHAGE
    Serial.print("instruction dans Run : "); Serial.println(INSTRUCTION);
    Serial.print("   taille instruction : "); Serial.println(String(INSTRUCTION.length()));
#endif
    char separator = '_'; // caractère de séparation entre fonction et arguments, ou entre arguments

    // récupération du code de la fonction appelée
    int posFin = INSTRUCTION.indexOf(separator); // repère le passage de la fonction aux arguments de la fonction
    int numFonction = INSTRUCTION.substring(0, posFin).toInt(); // lit le début de l'instruction comme un entier codant la future fonction à exécuter. Du coup, on aurait pu coder avec parseInt().
    String chaineArguments = INSTRUCTION.substring(posFin);
#ifdef AFFICHAGE
    Serial.print("   fonction n° : "); Serial.println(String(numFonction));
    Serial.print("   chaine d'arguments : "); Serial.println(chaineArguments);
#endif

    // récupération des arguments
    int i = 0; int numArg = 0;
    for (i = 0; i < chaineArguments.length(); i++) { // on compte les arguments
      if (chaineArguments[i] == separator) {
        numArg++;
      }
    }
#ifdef AFFICHAGE
    Serial.print("   Il y a "); Serial.print(numArg); Serial.println(" arguments.");
#endif
    String arguments[numArg]; // le tableau des arguments
    for (i = 0; i < numArg; i++) {
      int posFinArg = chaineArguments.indexOf(separator, 1);  // on repère la fin de l'argument
      String copie = chaineArguments;
      arguments[i] = copie.substring(1, posFinArg);
      String resteArguments = chaineArguments.substring(posFinArg);
      chaineArguments = resteArguments;
#ifdef AFFICHAGE
      Serial.print("   argument : "); Serial.println(String(arguments[i]));
#endif
    }

    switch (numFonction) { // cf dico des instructions
      case 0: // code d'urgence
        goHome();  // retour base
        break;
      case 1: // requête de transmission d'un message via l'antenne
        emettreMessage(arguments[0]);
        break;
      case 3: // requête de transmission du bilan d'activité du rover
        // attention, ça peut être long.
        switch (arguments[0].toInt()) {
          case 1: // demande de transmission des messages d'alerte
            messageBus += "3_1_" + msg_alerte + ";"; break;
          case 2: {// demande de transmission de la succession des ordres de marche
              String texte = successionOrdresMarche;
              texte.replace(";", "\n");
              messageBus += "3_2_" + texte + ";"; break;
            }
          case 3: {// demande de transmission de l'historique du chemin suivi
              String texte = cheminSuivi;
              texte.replace(";", "\n");
              messageBus += "3_3_" + texte + ";"; break;
            }
          default:
            break;
        }
        break;
      case 6: {// requête de transmission des données des capteurs pour la cartographie
          String message = "6_";
message += String(read_temperature(sensorTinterne));  // température, en degrés, décimal avec un point
message += String("");  // pression, unité ?, décimal avec un point
          message += String("");  // vitesse vent (norme du vecteur), unité ?, décimal avec un point
          message += String("");  // angle horizontal direction vent, en degrés ? par rapport à [Ox) sens trigo ?, décimal avec un point
          messageBus += message + ";";
          break;
        }
      case 11:  // ordre de marche : avancer
        successionOrdresMarche += INSTRUCTION + ";";
        directionServo.positionNormale();
        delay(1500);  // pour que les servos aient le temps de finir de tourner
        avancerTous(1, arguments[0].toFloat()); // arguments[0].toFloat() est la distance en mètres
        break;
      case 12:  // ordre de marche : reculer
        successionOrdresMarche += INSTRUCTION + ";";
        directionServo.positionNormale();
        delay(1500);  // pour que les servos aient le temps de finir de tourner
        avancerTous(2, arguments[0].toFloat()); // arguments[0].toFloat() est la distance en mètres
        break;
      case 13:  // ordre de marche : tourner à droite
        successionOrdresMarche += INSTRUCTION + ";";
        directionServo.positionSurPlace();
        delay(1500);  // pour que les servos aient le temps de finir de tourner
        tournerSurPlace('D', arguments[0].toFloat()); // arguments[0].toFloat() est l'angle en degrés
        //directionServo.positionNormale();
        break;
      case 14:  // ordre de marche : tourner à gauche
        successionOrdresMarche += INSTRUCTION + ";";
        directionServo.positionSurPlace();
        delay(1500);  // pour que les servos aient le temps de finir de tourner
        tournerSurPlace('G', arguments[0].toFloat()); // arguments[0].toFloat() est l'angle en degrés
        //directionServo.positionNormale();
        break;
        // ce qui suit est à but de test
      default: // Case of unmatched function.
#ifdef AFFICHAGE
        Serial.println("Ca serait gentil de me donner des instructions que je comprends !");
#endif
        return; // Panic. Something better to do ?
    } // fin du  : switch(numFonction)
  } // fin du : if (INSTRUCTION != "")
}

// la même pour le cas où il peut y avoir plusieurs instructions à la chaîne
void RunChaineOrdres(String INSTRUCTIONS) {
  int numOrdres = 1;
  for (int i = 0; i < INSTRUCTIONS.length(); i++) { // on compte les arguments
    if (INSTRUCTIONS[i] == ';') {
      numOrdres++;
    }
  }
  for (int i = 0; i < numOrdres; i++) {
    int posFin = INSTRUCTIONS.indexOf(';');
    String ordre = INSTRUCTIONS.substring(0, posFin);
    INSTRUCTIONS = INSTRUCTIONS.substring(posFin + 1);
#ifdef AFFICHAGE
    Serial.println(INSTRUCTIONS);
#endif
    Run(ordre);
  }
  return;
}

/***************************
  GROS BAZAR DE FONCTIONS...
****************************/

// Pour la RF : à enlever, car c'est externalisé dans RH.cpp
/*boolean emettreMessage(String message) {
  int nbrCaracteresMax = 31; // déterminé empiriquement (36 en principe !)
  int nbrPhrases = int(message.length() / nbrCaracteresMax) + 1;
  String phrase;
  for (int i = 0; i < nbrPhrases; i++) {
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
}*/
