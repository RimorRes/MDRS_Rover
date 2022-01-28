/***********************************************************************
 * Définition des classes utiles pour gérer la position, la vitesse, ...
************************************************************************/

#include <Arduino.h>
#include "deplacement.h"

/**************
 * classe Point
 * ************/
 
void Point::init(float X, float Y, float T)
{
  _X = X;
  _Y = Y;
  _T = T;
}

float Point::getX(){return _X;}
float Point::getY(){return _Y;}
float Point::getT(){return _T;}

void Point::setX(float X){_X = X;}
void Point::setY(float Y){_Y = Y;}
void Point::setT(float T){_T = T;}

String Point::affichage(){
  return String(_X) + String(", ") + String(_Y) + String(", ") + String(_T);
}

/***************
 * classe Chemin
 * *************/
 
void Chemin::init(Point PointDebut, Point PointFin)
{
  _PointDebut = PointDebut;
  _PointFin = PointFin;
  _Points[0] = _PointDebut;
  _nombrePoints = 1;
}

Point Chemin::getPointDebut(){return _PointDebut;}
Point Chemin::getPointFin(){return _PointFin;}
Point Chemin::getPointActuel(){return _PointActuel;}

void Chemin::setPointDebut(Point PointDebut){_PointDebut = PointDebut;}
void Chemin::setPointFin(Point PointFin){_PointFin = PointFin;}
void Chemin::setPointActuel(Point PointActuel){_PointActuel = PointActuel;}

int Chemin::getNumeroPointActuel(){return _numeroPointActuel;}
int Chemin::getNombrePoints(){return _nombrePoints;}
 
String Chemin::affichage()
{
  String texte="";
  for (int i=0; i<_nombrePoints; i++){
    texte += _Points[i].affichage();
    texte += "\n";
  }
  return texte;
}

Point Chemin::avancerPointSuivant()
{
  if (_numeroPointActuel < _nombrePoints){
    _numeroPointActuel++;
    setPointActuel(_Points[_numeroPointActuel]);
  }
  return _PointActuel;
}

Point Chemin::getPointParNumero(int numero)
{
  Point point = _PointActuel;
  if(numero>=0 && numero<_nombrePoints){
    point = _Points[numero];
  }
  return point;
}

void Chemin::addPoint(int numero, Point point){
  if (numero>0 && numero<=_nombrePoints){ // On ne peut changer le point initial...
    if (numero<_nombrePoints){  // Le point se rajoute au milieu...
      for (int i=_nombrePoints;i>numero;i--){ // On décale tout ce qui suit.
        _Points[i]=_Points[i-1];
      }
    } else {  // Le point se rajoute à la fin...
      _PointFin = point;  // On change le point final.
    }
    _Points[numero] = point;  // On ajoute le point.
    _nombrePoints++;
  }
  return;
}

void Chemin::removePoint(int numero){
  if (numero>0 && numero<_nombrePoints){  // On ne peut changer le point initial...
    if (numero<_nombrePoints-1){  // Le point est enlevé au milieu...
      for (int i=numero;i<_nombrePoints-1;i++){ // On décale tout ce qui suit.
        _Points[i]=_Points[i+1];
      }
    } else {  // Le point est enlevé à la fin...
      _PointFin = _Points[_nombrePoints-2];  // On change le point final.
    }
    // Le point _Points[_nombrePoints-1] est laissé à son ancienne valeur.
    _nombrePoints--;
  }
  return;
}
