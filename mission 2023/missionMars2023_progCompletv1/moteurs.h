/*********************
 * Gestion des moteurs
**********************/

#include <Arduino.h>

extern float distanceParcourue;
extern float directionRover;

class Moteur
{
  public:
    Moteur(int PIN_moteur_1, int PIN_moteur_2, int PIN_mesure_tension_alim);
    boolean init_moteur();
    float getTensionAlim() const;
    float actualiseDelayTime();
    void activer(int sens) const;
    void desactiver() const;
    void avancer_t(int sens, float tours); //fonction pour avancer un nombre <t> de tours
    void avancer_m(int sens, float m); //fonction pour avancer un nombre <m> de mètres
  private:
    int _PIN_moteur_1;
    int _PIN_moteur_2;
    int _PIN_mesure_tension_alim;
    float _delayTime; // temps d'activation du moteur POUR 1 TOUR
};

void avancerTous(int sens, float m); // faire avancer tous les moteurs, dans le sens <sens> du nombre de mètres <m>
void tournerSurPlace(char sens, int angle); // faire avancer les deux moteurs d'un côté, et reculer les deux autres, dans le sens <sens>, d'un angle <angle> en degrés
float getTensionAlim();
float getDelayTime(); // calcule le temps pour qu'un moteur fasse un tour exactement, selon la tension d'alimentation.
