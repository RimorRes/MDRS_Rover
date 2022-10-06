/**************************
 * Gestion des servomoteurs
***************************/

#include <Arduino.h>

class DirectionServo
{
  public:
    DirectionServo(Servo servoAVD, Servo servoAVG, Servo servoARD, Servo servoARG);
    void init();
    void angle(int angle); // roues avant tournées d'un angle <angle> et arrière tournées de -<angle>, angle en degrés par rapport à l'avant, sens trigo
    void positionNormale();  // les roues droites
    void positionSurPlace(); // les roues tangentes au cercle circonscrit : roues AVD et ARG à gauche et les autres à droite
    void positionADroite();  // les roues avant à droite, et arrière à gauche
    void positionAGauche();  // les roues avant à gauche, et arrière à droite
  private:
    Servo _servoAVD;
    Servo _servoAVG;
    Servo _servoARD;
    Servo _servoARG;
};

void testServo(Servo servo);
