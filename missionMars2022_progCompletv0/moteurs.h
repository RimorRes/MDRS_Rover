/*********************
 * Gestion des moteurs
**********************/

#include <Arduino.h>

class Moteur
{
  public:
    Moteur(int PIN_moteur_1, int PIN_moteur_2, int PIN_moteur_3, int PIN_moteur_4, int PIN_mesure_tension_alim);
    boolean init_moteur();
    float getTensionAlim() const;
    float actualiseDelayTime();
    void activer(char moteur, int sens) const;
    void desactiver(char moteur) const;
    void avancer_t(char moteur, int sens, float tours); //fonction pour avancer un nombre <t> de tours
    void avancer2_t(int sens, float tours); //idem les deux moteurs ensemble
    void avancer_m(char moteur, int sens, float m); //fonction pour avancer un nombre <m> de mètres
    void avancer2_m(int sens, float m); //idem les deux moteurs ensemble
  private:
    int _PIN_moteur_1;
    int _PIN_moteur_2;
    int _PIN_moteur_3;
    int _PIN_moteur_4;
    int _PIN_mesure_tension_alim;
    float _delayTime; // temps d'activation du moteur POUR 1 TOUR
};
