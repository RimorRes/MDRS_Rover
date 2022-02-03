#include <Arduino.h>

// OK, ce qui suit est assez moche comme code. Ca fait le job. On pourra améliorer ensuite.

class PIN_spec
{
  public:
    //void init();
    const int PIN_moteur1_1 = 4; // avant droit ?
    const int PIN_moteur1_2 = 5; // avant gauche ?
    const int PIN_moteur1_3 = 6; // arrière droit ?
    const int PIN_moteur1_4 = 7; // arrière gauche ?
    const int PIN_mesure_tension_alim = A0; // mesure tension alimentation des moteurs
    
    const int PIN_detectObst1_Trig = 8;
    const int PIN_detectObst1_Echo = 9;
    const int PIN_detectObst2_Trig = 10;
    const int PIN_detectObst2_Echo = 11;
    
    const int PIN_SCL = A5; // inutile, c'est juste pour penser � la r�server. On ne peut pas changer la valeur.
    const int PIN_SDA = A4; // inutile, c'est juste pour penser � la r�server. On ne peut pas changer la valeur.
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
};
