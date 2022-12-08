/**************************
 * Gestion des servomoteurs
***************************/

/*
 * Pour la rotation des servos, comme ça prend un peu de temps, il serait préférable
 * d'intégrer cette durée dans le code, par exemple en faisant une rotation plus lente
 * par exemple 15 ms par degré (voir doc du module).
 * 
 * garder en mémoire la position actuelle du servo (déjà dans le module Servo.h ?) si 
 * on veut faire de la rotation progressive.
 * 
 * position zéro : à caler à la main
 * 
 * faire tourne la roue en même temps ? pour éviter d'user les pneus ?
 */

#include <Arduino.h>
#if !defined SPECIFICATIONS_H
  #include "specifications.h"
  #define SPECIFICATIONS_H
#endif 
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
  positionNormale();
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

void testServo(Servo servo){
  servo.write(0); delay(500);
  servo.write(90); delay(500);
  servo.write(180); delay(500);
  return;  
}
