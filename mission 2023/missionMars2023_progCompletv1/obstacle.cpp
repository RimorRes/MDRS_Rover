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

/*****************************
* GESTION DE _obstaclesListe *
*****************************/

Point Obstacle::obstaclePositionFromRover(Point R, float d, float p)const{
// ...avec R la position du rover, d la distance à l'obstacle et p la direction du rover. 
  return Point(R.getX() + cos(p) * d, R.getY() + sin(p) * d);
}

void Obstacle::addObstacle(ObstacleObject obj, float r){
  if(_obstaclesListeLen < _obstaclesListeSize){
    if(isExistingObstacle(obj._pos, r)){ return; } // l'obstacle existe déjà dans ces parages
    _obstaclesListeLen++;
    _obstaclesListe[_obstaclesListeLen - 1] = obj;
  }
}

void Obstacle::removeObstacle(int n){
  if(_obstaclesListeLen > 0 && n < _obstaclesListeLen){
    for(int i = n; i < _obstaclesListeLen; i++){
      _obstaclesListe[i] = _obstaclesListe[i + 1];
    }
    _obstaclesListeLen--;
  }
}

bool Obstacle::isExistingObstacle(Point P, float r_ignore)const{
  // ...avec r_ignore la précision. 
  r_ignore *= r_ignore;
  for(int i = 0; i < _obstaclesListeLen; i++){
    float x = _obstaclesListe[i]._pos.getX() - P.getX();
    float y = _obstaclesListe[i]._pos.getY() - P.getY();
    if(x * x + y * y <= r_ignore){ return true; }
  }
  return false;
  //Optimisation future : array bidimentionnel de pointeur de points. Inutile à ce stade.  
}

/********************
* GESTION DU CHEMIN *
********************/

Point Obstacle::cheminCorrection(Point P, float r, float p)const{
// ...avec P l'obstacle et r la distance entre l'obstacle et le nouveau Point.
  return Point(P.getX() - sin(p) * r, P.getY() + cos(p) * r);
}
