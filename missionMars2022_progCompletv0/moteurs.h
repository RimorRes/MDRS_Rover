/*********************
 * Gestion des moteurs
**********************/

#include <Arduino.h>

class Moteur
{
  public:
    Moteur(int PIN_moteur_1, int PIN_moteur_2, int PIN_moteur_3, int PIN_moteur_4, int PIN_mesure_tension_alim);
    boolean init_moteur() const;
  private:
    int _PIN_moteur_1;
    int _PIN_moteur_2;
    int _PIN_moteur_3;
    int _PIN_moteur_4;
    int _PIN_mesure_tension_alim;
};
