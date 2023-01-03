#include <Arduino.h>

class PIN_spec
{
  public:
    // PIN_spec();
    const int PIN_moteurAVD_1 = 7;//10; // moteur avant droit sens 2 // soudé
    const int PIN_moteurAVD_2 = 6;//11; // moteur avant droit sens 1 // soudé 
    const int PIN_moteurAVG_1 = 13;//6; // moteur avant gauche sens 2 // soudé
    const int PIN_moteurAVG_2 = 12;//7; // moteur avant droit sens 1 // soudé
    const int PIN_moteurARD_1 = 9;//12; // moteur arrière droit sens 2 // soudé
    const int PIN_moteurARD_2 = 8;//13; // moteur arrière droit sens 1 // soudé
    const int PIN_moteurARG_1 = 11;//8; // moteur arrière gauche sens 2 // soudé
    const int PIN_moteurARG_2 = 10;//9; // moteur arrière gauche sens 1 // soudé
    const int PIN_mesure_tension_alim = A0; // mesure tension alimentation des moteurs  --> à enlever ?

    const int PIN_servoAVD = 23;  // servomoteur pour la direction, avant droit. // soudé
    const int PIN_servoAVG = 22;  // servomoteur pour la direction, avant gauche. // soudé
    const int PIN_servoARD = 24;  // servomoteur pour la direction, arrière droit. // soudé
    const int PIN_servoARG = 17;  // servomoteur pour la direction, arrière gauche. // soudé

    const int PIN_detectObst1_Trig = 3; // soudé
    const int PIN_detectObst1_Echo = 2; // soudé
    //const int PIN_detectObst2_Trig = 14;
    //const int PIN_detectObst2_Echo = 15;

    const int PIN_RF_CE = 4; // soudé
    const int PIN_RF_CSN = 53; // à verifier
    
    const int PIN_SCL = 21; // soudé
    const int PIN_SDA = 20; // soudé
    //const int PIN_temp_alert = A3;  //Si on veut utiliser la sortie alert du détecteur de température MP102.

//    const int PIN_GPS_TX = 19; // soudé (port série Serial1) --> GPS
//    const int PIN_GPS_RX = 18; // soudé (port série Serial1) --> GPS
};

class Rover_spec
{
  public:
    //Rover_spec();
    const float rayonExterneRoueEnMetres = 0.175/2; // rayon des roues motrices
    const float tensionSeuilAlimMoteurs = 1.39; // tension à partir de laquelle les moteurs tournent, en V
    const float numeroMagique = 6*30.*1.08;  // nombre magique pour déterminer la conversion nombre de tours / temps d'activation pour les moteurs
    const float numeroMagique2 = 0.75;  // idem numeroMagique mais pour la rotation sur place
    const float rayonSurPlaceEnMetres = 0.42; // Il s'agit du rayon du cercle quand on tourne sur place.
    const int angleSurPlace = -83;  // l'angle dont il faut tourner les roues pour qu'elles soient tangentes au cercle circonscrit.
    const int angleZeroAVD = 45;  // l'angle qui permet au servomoteur AVD de mettre la roue droite, dans l'axe du rover.
    const int angleZeroAVG = 125;  // l'angle qui permet au servomoteur AVG de mettre la roue droite, dans l'axe du rover.
    const int angleZeroARD = 125;  // l'angle qui permet au servomoteur ARD de mettre la roue droite, dans l'axe du rover.
    const int angleZeroARG = 60;  // l'angle qui permet au servomoteur ARG de mettre la roue droite, dans l'axe du rover.
};

class Rover_config
{
  public:
    //Rover_config();
//    const float Tint_min = 5; // seuil d'alerte bas pour la température interne, en degrés
//    const float Tint_max = 80; // seuil d'alerte haut pour la température interne, en degrés

//    const float tensionAlerteMin = 10;  // seuil d'alerte bas, en volts  --> à enlever ?
//    const float tensionAlerteMax = 13;  // seuil d'alerte haut, en volts  --> à enlever ?
    const float tensionCodeMin = 10;  // le code ne prendra pas en compte des valeurs inférieures (protège contre défaillance mesure)  --> à enlever ?
    const float tensionCodeMax = 13;  // le code ne prendra pas en compte des valeurs supérieures (protège contre défaillance mesure)  --> à enlever ?

    const float distanceMin = 0.1;  // distance seuil (en m), en-deça de laquelle on détecte qu'il y a problème (détection d'obstacle)

    // Si l'une des deux valeurs de tolérance est trop élevée, modifier le code de String Chemin::goToNext() dans deplacement.cpp
    const float tolerancePosition = 0.5;  // tolerance sur la position pour déclarer l'égalité, en mètres, > 0.1
    const float toleranceAngle = 5; // tolerance sur l'angle pour déclarer l'égalité, en degrés, > 1
    const float pasChemin = 3; // pas pour la génération des points intermédiaires sur le chemin, en mètres. précision GPS : 5 à 10 mètres.
    const float directionInitiale = 0; // initialement, le rover pointe à l'Est.
};

extern const PIN_spec myPINs;
extern const Rover_spec rover_spec;
extern const Rover_config rover_config;
