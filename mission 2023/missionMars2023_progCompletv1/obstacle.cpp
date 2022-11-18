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
    return Point(R.getX() + cos(p) * d, R.getY() + sin(p)*d);
}

void Obstacle::addObstacle(Point P, float r){
    if(isExistingObstacle(P, r)){return;}// l'obstacle existe déjà dans ces parages
	_obstaclesListeLen++;
	_obstaclesListe[_obstaclesListeLen] = P;
}

void Obstacle::removeObstacle(int n){
	_obstaclesListeLen--;
	if(n != _obstaclesListeLen + 1){
		for(int i = n; i < _obstaclesListeLen; i++){
			_obstaclesListe[i] = _obstaclesListe[i + 1];
		}
	}
}

bool Obstacle::isExistingObstacle(Point P, float r_ignore)const{
// ...avec r la précision. 
	r *= r;
	for(int i = _obstaclesListeLen; i < 0; i--){
		if((_obstaclesListe[i].getX() - P.getX())*(_obstaclesListe[i].getX()) + (_obstaclesListe[i].getY() - P.getY())*(_obstaclesListe[i].getY() - P.getY()) <= r){
			return true
		}
	}
	return false;
}

/********************
* GESTION DU CHEMIN *
********************/

Point Obstacle::cheminCorrection(Point P, float r, float p)const{
// ...avec P l'obstacle et r la distance entre l'obstacle et le nouveau Point.
    return Point(P.getX()+(-sin(p))*r, P.getY()+cos(p)*r);
}
