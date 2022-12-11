#include "Arduino.h"
#include "Math.h"

#if !defined SPECIFICATIONS_H
#include "specifications.h"
#define SPECIFICATIONS_H
#endif
#if !defined DEPLACEMENT_H
#include "deplacement.h"
#define DEPLACEMENT_H
#endif
#if !defined OBSTACLE_H
#include "obstacle.h"
#define OBSTACLE_H
#endif
#if !defined MOTEURS_H
#include "moteurs.h"
#define MOTEURS_H
#endif

/*****************************
* GESTION DE _obstaclesListe *
*****************************/

Point Obstacle::obstaclePositionFromRover(Point R, float d, float p)const{
// ...avec R la position du rover, d la distance à l'obstacle et p la direction du rover. 
  return Point(R.getX() + cos(p) * d, R.getY() + sin(p) * d);
}

void Obstacle::addObstacle(Point P, float r){
  if(_obstaclesListeLen >= _obstaclesListeSize){
    removeObstacle(0);
  }
  if(isExistingObstacle(P, r)){ return; } // l'obstacle existe déjà dans ces parages
  _obstaclesListeLen++;
  _obstaclesListe[_obstaclesListeLen - 1] = P;
}

void Obstacle::removeObstacle(int n){
  if(_obstaclesListeLen > 0 && n < _obstaclesListeLen){
    _obstaclesListe[n] = _obstaclesListe[--_obstaclesListeLen];
  }
}

bool Obstacle::isExistingObstacle(Point P, float r_ignore)const{
  // ...avec r_ignore la précision. 
  r_ignore *= r_ignore;
  for(int i = 0; i < _obstaclesListeLen; i++){
    float x = _obstaclesListe[i].getX() - P.getX();
    float y = _obstaclesListe[i].getY() - P.getY();
    if(x * x + y * y <= r_ignore){ return true; }
  }
  return false;
}

void Obstacle::updateObstaclesListe(){
  for(int i = 0; i < _obstaclesListeLen; i++){
    if(distanceParcourue - _obstaclesListe[i].getDistanceDeDetection() > _distanceThreshold){
      removeObstacle(i);
    }
  }
}

/********************
* GESTION DU CHEMIN *
********************/

Point Obstacle::cheminCorrection(Point P, float r, float p)const{
// ...avec P l'obstacle et r la distance entre l'obstacle et le nouveau Point.
  return Point(P.getX() - sin(p) * r, P.getY() + cos(p) * r);
}
