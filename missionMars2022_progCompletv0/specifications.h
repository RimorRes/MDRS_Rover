#include <Arduino.h>
#if !defined DEPLACEMENT_H
  #include "deplacement.h"
  #define DEPLACEMENT_H
#endif 

// OK, ce qui suit est assez moche comme code. Ca fait le job. On pourra améliorer ensuite.

class PIN_spec
{
  public:
    const int PIN_moteur1_1 = 4; // moteur 1A sens 2
    const int PIN_moteur1_2 = 5; // moteur 1A sens 1
    const int PIN_moteur1_3 = 6; // moteur 1B sens 2
    const int PIN_moteur1_4 = 7; // moteur 1B sens 1
    const int PIN_moteur2_1 = 8; // moteur 2A sens 2
    const int PIN_moteur2_2 = 9; // moteur 2A sens 1
    const int PIN_moteur2_3 = 10; // moteur 2B sens 2
    const int PIN_moteur2_4 = 11; // moteur 2B sens 1
    const int PIN_mesure_tension_alim = A0; // mesure tension alimentation des moteurs

    const int PIN_detectObst1_Trig = 12;
    const int PIN_detectObst1_Echo = 13;
    const int PIN_detectObst2_Trig = 14;
    const int PIN_detectObst2_Echo = 15;

    const int PIN_RF_CE = 16;
    const int PIN_RF_CSN = 17;
    
    const int PIN_SCL = A5; // inutile, c'est juste pour penser à la réserver. On ne peut pas changer la valeur.
    const int PIN_SDA = A4; // inutile, c'est juste pour penser à la réserver. On ne peut pas changer la valeur.
    const int PIN_temp_alert = A3;  //Si on veut utiliser la sortie alert du détecteur de température MP102.
};

class Rover_spec
{
  public:
    //void init();
    const float rayonExterneRoueEnMetres = 0.175/2; // rayon des roues motrices
    const float tensionSeuilAlimMoteurs = 1.39; // tension à partir de laquelle les moteurs tournent, en V
    const float rayonSurPlaceEnMetres = 0.42; // Il s'agit du rayon du cercle quand on tourne sur place.
};

class Rover_config
{
  public:
    // Si l'une des deux valeurs de tolérance est trop élevée, modifier le code de String Chemin::goToNext() dans deplacement.cp
    const float tolerancePosition = 0.5;  // tolerance sur la position pour déclarer l'égalité, en mètres, > 0.1
    const float toleranceAngle = 5; // tolerance sur l'angle pour déclarer l'égalité, en degrés, > 1
    const float pasChemin = 3; // pas pour la génération des points intermédiaires sur le chemin, en mètres 
    const Point centreRepere = Point(0, 0, 0); // à initialiser d'après la carte
    const float directionInitiale = 0; // initialement, le rover pointe à l'Est.
    const Chemin cheminParDefaut = Chemin(); // à initialiser d'après la carte
    
    //const float tensionAlerteMin = 10;  // seuil d'alerte bas, en volts
    //const float tensionAlerteMax = 13;  // seuil d'alerte haut, en volts
    //const float tensionCodeMin = 10;  // le code ne prendra pas en compte des valeurs inférieures (protège contre défaillance mesure)
    //const float tensionCodeMax = 13;  // le code ne prendra pas en compte des valeurs supérieures (protège contre défaillance mesure)

    //const float Tint_min = 5; // seuil d'alerte bas pour la température interne, en degrés
    //const float Tint_max = 80; // seuil d'alerte haut pour la température interne, en degrés
};
