/***********************************************************************
 * Définition des classes utiles pour gérer la position, la vitesse, ...
************************************************************************/

#include <Arduino.h>
#include <Math.h>
#include "specifications.h"
extern Rover_spec rover_spec;
#include "deplacement.h"
extern float directionRover;
extern String successionOrdresMarche;

/**************
 * classe Point
 * ************/
 
Point::Point()
{
  _X = 0;
  _Y = 0;
  _T = 0;
}

Point::Point(float X, float Y, float T)
{
  _X = X;
  _Y = Y;
  _T = T;
}

float Point::getX() const{return _X;}
float Point::getY() const{return _Y;}
float Point::getT() const{return _T;}

void Point::setX(float X){_X = X;}
void Point::setY(float Y){_Y = Y;}
void Point::setT(float T){_T = T;}

String Point::affichage() const{
  return String(_X) + String(", ") + String(_Y) + String(", ") + String(_T);
}

/***************
 * classe Chemin
 * *************/
 
Chemin::Chemin()
{
  _numeroPointActuel = 0;
  _nombrePoints = 0;
}

Chemin::Chemin(Point PointDebut, Point PointFin)
{
  _PointDebut = PointDebut;
  _PointFin = PointFin;
  _Points[0] = _PointDebut;
  _nombrePoints = 1;
}

Point Chemin::getPointDebut() const{return _PointDebut;}
Point Chemin::getPointFin() const{return _PointFin;}
Point Chemin::getPointActuel() const{return _PointActuel;}

void Chemin::setPointDebut(Point PointDebut){_PointDebut = PointDebut;}
void Chemin::setPointFin(Point PointFin){_PointFin = PointFin;}
void Chemin::setPointActuel(Point PointActuel){_PointActuel = PointActuel;}

int Chemin::getNumeroPointActuel() const{return _numeroPointActuel;}
int Chemin::getNombrePoints() const{return _nombrePoints;}
 
String Chemin::affichage() const
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

Point Chemin::getPointParNumero(int numero) const
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

Chemin Chemin::cheminRetour(){
  // still to code
  Chemin chemin;
  chemin.setPointDebut(this->getPointActuel());
  int longueur =1;
  for (int i=_numeroPointActuel; i>=0; i--){
    chemin.addPoint(longueur, this->_Points[i]);
    longueur++;
  }
  return chemin;
}

// Génère les ordres de marche permettant de rejoindre le point suivant
String Chemin::goToNext(){
  String listeOrdresDeMarche = "";
  
  Point pointDepart = getPointParNumero(_numeroPointActuel+1);
  Point pointArrivee = _PointActuel;
  float deltaX = pointArrivee.getX()-pointDepart.getX();
  float deltaY = pointArrivee.getY()-pointDepart.getY();

  // déterminer la direction cible
  float directionCible; // angle en degré, par rapport à la demi-droite Ox, sens trigonométrique
  if (abs(deltaX) <= rover_spec.tolerancePosition){
    if (abs(deltaY) <= rover_spec.tolerancePosition) {directionCible = directionRover;}  // si on est au bon point, on ne change rien !
    else {
      if (deltaY > 0) {directionCible = 90;} else {directionCible = -90;} // direction verticale
    }
  } else {
    directionCible = atan(deltaY / deltaX); // en radians
    directionCible *= 180 / 3.141592654;  // en degrés
    if (deltaX < 0) {
      if (directionCible > 0) {directionCible += 90;} else {directionCible -= 90;}
    }  // valeur entre -180 et 180 degrés
  }
  
  // déterminer l'ordre de marche pour la direction
  float angleRotation = directionCible - directionRover;
  if (abs(angleRotation) > rover_spec.toleranceAngle) {
    if(angleRotation > 0) {
      listeOrdresDeMarche += "14_"; // tourner à gauche (cf dico des instructions)
    } else {
      listeOrdresDeMarche += "13_"; // tourner à droite (cf dico des instructions)
    }
    String texteAngle = String(abs(angleRotation),0); // arrondi au degré près
    listeOrdresDeMarche += texteAngle;
    listeOrdresDeMarche += ";";
  }

  // déterminer l'ordre de marche pour la propulsion
  float distanceCible = distance(pointDepart, pointArrivee);
  distanceCible = max(distanceCible, 999);  // pour éviter d'avoir plus de 5 caractères au final, avec la virgule et la décimale
  if (distanceCible > rover_spec.tolerancePosition) {
    listeOrdresDeMarche += "11_"; // avancer (cf dico des instructions)
    String texteDistance = String(distanceCible,1); // arrondi au décimètre près
    listeOrdresDeMarche += texteDistance;
    listeOrdresDeMarche += ";";    
  }
  
  return listeOrdresDeMarche;
}

/******************
 * autres fonctions
 * ****************/
 
float distance(Point A, Point B) {
  return sqrt( sq(B.getX()-A.getX()) + sq(B.getY()-A.getY()) );
}

void goHome(){
  Serial.println("E.T. maison ...");
  Serial.println("...encore à coder, désolé !");
}
