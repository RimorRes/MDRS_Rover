/**************************
 * Gestion des servomoteurs
***************************/

#include <Arduino.h>
#if !defined SPECIFICATIONS_H
  #include "specifications.h"
  #define SPECIFICATIONS_H
#endif 
extern PIN_spec myPINs;
extern Rover_spec rover_spec;
extern Rover_config rover_config;
#if !defined SERVO_H
  #include <Servo.h>
  #define SERVO_H
#endif
#if !defined SERVOMOTEURS_H
  #include "servoMoteurs.h"  // module codé par nous
  #define SERVOMOTEURS_H
#endif

DirectionServo::DirectionServo(Servo servoAVD, Servo servoAVG, Servo servoARD, Servo servoARG) {
  _servoAVD = servoAVD;
  _servoAVG = servoAVG;
  _servoARD = servoARD;
  _servoARG = servoARG;
}

void DirectionServo::init(){
  _servoAVD.attach(myPINs.PIN_servoAVD);
  _servoAVG.attach(myPINs.PIN_servoAVG);
  _servoARD.attach(myPINs.PIN_servoARD);
  _servoARG.attach(myPINs.PIN_servoARG);
}

// roues avant tournées d'un angle <angle> et arrière tournées de -<angle>, angle en degrés par rapport à l'avant, sens trigo
void DirectionServo::angle(int angle){
  _servoAVD.write(rover_spec.angleZeroAVD + angle);
  _servoAVG.write(rover_spec.angleZeroAVG + angle);
  _servoARD.write(rover_spec.angleZeroARD - angle);
  _servoARG.write(rover_spec.angleZeroARG - angle);
}

// les roues droites
void DirectionServo::positionNormale(){
  angle(0);
}

// les roues tangentes au cercle circonscrit : roues AVD et ARG à gauche et les autres à droite
void DirectionServo::positionSurPlace(){
  int angle = rover_spec.angleSurPlace;
  _servoAVD.write(rover_spec.angleZeroAVD + angle);
  _servoAVG.write(rover_spec.angleZeroAVG - angle);
  _servoARD.write(rover_spec.angleZeroARD - angle);
  _servoARG.write(rover_spec.angleZeroARG + angle);
}

// les roues avant à droite, et arrière à gauche
void DirectionServo::positionADroite(){
  angle(-45);
}

// les roues avant à gauche, et arrière à droite
void DirectionServo::positionAGauche(){
  angle(45);
}
