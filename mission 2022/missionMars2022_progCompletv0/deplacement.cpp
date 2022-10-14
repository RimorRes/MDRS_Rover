/***********************************************************************
 * Définition des classes utiles pour gérer la position, la vitesse, ...
************************************************************************/

//#define AFFICHAGE // à commenter pour rendre moins bavard...
#include <Arduino.h>
#include <Math.h>
#if !defined SPECIFICATIONS_H
  #include "specifications.h"
  #define SPECIFICATIONS_H
#endif 
extern Rover_spec rover_spec;
extern Rover_config rover_config;
#if !defined DEPLACEMENT_H
  #include "deplacement.h"
  #define DEPLACEMENT_H
#endif 
extern Chemin chemin;
extern float directionRover;
extern boolean goingHome;
extern String cheminSuivi;

/**************
 * classe Point
 * ************/
 
Point::Point()
{
  _X = 0;
  _Y = 0;
  _T = 0;
}

Point::Point(float X, float Y)
{
  _X = X;
  _Y = Y;
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

String Point::toString() const{
  // chaîne formattée avec les coordonnées
  return String(_X) + "_" + String(_Y) + ";";
  // Je n'ai pas mis _T : si on le fait, attention à changer le dictionnaire des instructions
}

Point Point::relative(Point PointOrigine) const{
  // pour changer l'origine du repère
  return Point(_X-PointOrigine.getX(), _Y-PointOrigine.getY(), _T-PointOrigine.getT());
}


/***************
 * classe Chemin
 * *************/
 
Chemin::Chemin()
{
  _numeroPointActuel = 0;
  _nombrePoints = 0;
}

Chemin::Chemin(Point pointDebut, Point pointFin)
{
  _PointDebut = pointDebut;
  _PointFin = pointFin;
  _Points[0] = _PointDebut;
  _Points[1] = _PointFin;
  _nombrePoints = 2;
  _PointActuel = pointDebut;
  _numeroPointActuel = 0;
}

Point Chemin::getPointDebut() const{return _PointDebut;}
Point Chemin::getPointFin() const{return _PointFin;}
Point Chemin::getPointActuel() const{return _PointActuel;}

void Chemin::setPointDebut(Point pointDebut){
  _PointDebut = pointDebut;
  _Points[0] = _PointDebut;
  }
void Chemin::setPointFin(Point pointFin){
  _PointFin = pointFin;
  _Points[_nombrePoints-1] = _PointFin;
  }
void Chemin::setPointActuel(Point pointActuel){_PointActuel = pointActuel;}

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
  Point point = _PointActuel; // valeur pour défaut pour minimiser l'impact des bugs éventuels, j'espère
  if(numero>=0 && numero<_nombrePoints){
    point = _Points[numero];
  }
  return point;
}

void Chemin::addPoint(int numero, Point point){
  if (numero>_numeroPointActuel && numero<=_nombrePoints){ // On ne peut changer les points déjà parcourus...
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

void Chemin::addPoint(Point point){// l'ajout du point se fait à la fin
  addPoint(_nombrePoints, point);
  return;
}

void Chemin::removePoint(int numero){
  if (numero>_numeroPointActuel && numero<_nombrePoints){  // On ne peut changer les points déjà parcourus...
    if (numero<_nombrePoints-1){  // Le point est enlevé au milieu...
      for (int i=numero;i<_nombrePoints-1;i++){ // On décale tout ce qui suit.
        _Points[i]=_Points[i+1];
      }
    } else {  // Le point est enlevé à la fin...
      _PointFin = _Points[_nombrePoints-2];  // On change le point final.
    }
    // Le point _Points[_nombrePoints-1] est laissé à son ancienne valeur,
    // mais il ne sera jamais lu, peut-on espérer.
    _nombrePoints--;
  }
  return;
}

Chemin Chemin::cheminInverse() const{
  Chemin cheminInv = Chemin(getPointActuel(), getPointDebut());
  int longueur =1;
  for (int i=_numeroPointActuel; i>0; i--){
    cheminInv.addPoint(longueur, _Points[i]);
    longueur++;
  }
  return cheminInv;
}

// Génère les ordres de marche permettant de rejoindre le point suivant
String Chemin::goToNext(){
  String listeOrdresDeMarche = "";
  
  Point pointDepart = _PointActuel;
  Point pointArrivee = getPointParNumero(_numeroPointActuel+1);
  float deltaX = pointArrivee.getX()-pointDepart.getX();
  float deltaY = pointArrivee.getY()-pointDepart.getY();
#ifdef AFFICHAGE
  Serial.print("deltaX = "); Serial.println(deltaX);
  Serial.print("deltaY = "); Serial.println(deltaY);
#endif

  // déterminer la direction cible
  float directionCible; // angle en degré, par rapport à la demi-droite Ox, sens trigonométrique
  if (abs(deltaX) <= rover_config.tolerancePosition){
    if (abs(deltaY) <= rover_config.tolerancePosition) {directionCible = directionRover;}  // si on est au bon point, on ne change rien !
    else {
      if (deltaY > 0) {directionCible = 90;} else {directionCible = -90;} // direction verticale
    }
  } else {
    directionCible = atan(deltaY / deltaX); // en radians
    directionCible *= 180 / 3.141592654;  // en degrés
    if (deltaX < 0) {
      if (directionCible > 0) {directionCible -= 180;} else {directionCible += 180;}
    }  // valeur entre -180 et 180 degrés
  }
  int dir = (int)directionCible;
  dir += 180;
  dir %= 360;
  dir -= 180;
  directionCible = dir;
#ifdef AFFICHAGE
  Serial.print("direction cible : "); Serial.println(directionCible);
#endif
  
  // déterminer l'ordre de marche pour la direction
  float angleRotation = directionCible - directionRover;
  if (abs(angleRotation) > rover_config.toleranceAngle) {
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
  distanceCible = min(distanceCible, 999);  // pour éviter d'avoir plus de 5 caractères au final, avec la virgule et la décimale
  if (distanceCible > rover_config.tolerancePosition) {
    listeOrdresDeMarche += "11_"; // avancer (cf dico des instructions)
    String texteDistance = String(distanceCible,1); // arrondi au décimètre près
    listeOrdresDeMarche += texteDistance;
    listeOrdresDeMarche += ";";    
  }
  
  return listeOrdresDeMarche;
}

void Chemin::recalculer() {
  // recalcule le chemin entre le point actuel et le point final
  
  if (_numeroPointActuel == _nombrePoints-1){return;}  // rien à faire : on est à la fin
  float distanceCible = distance(_PointActuel, _PointFin);
  if (distanceCible < rover_config.tolerancePosition){return;}  // rien à faire : on est juste à côté
  
  // nettoyer le chemin : on enlève tout entre l'actuel et le final
  for (int i=_numeroPointActuel+1; i<_nombrePoints-1; i++){
    removePoint(i);
  }
  
  // on recalcule des points intermédiaires
  int nbrPoints = distanceCible / rover_config.pasChemin; // nombre de points à rajouter pour interpoler
  float taillePasX = ( _PointFin.getX() - _PointActuel.getX() ) / (nbrPoints+1);  // X à parcourir pour chaque pas (un pas de plus que le nombre de points supplémentaires)
  float taillePasY = ( _PointFin.getY() - _PointActuel.getY() ) / (nbrPoints+1);  // idem sur Y
  Point pointAjout; // le point à ajouter
  for (int i=1; i<nbrPoints+1; i++){
    pointAjout = Point(_PointActuel.getX()+i*taillePasX, _PointActuel.getY()+i*taillePasY);
    addPoint(_numeroPointActuel+i, pointAjout);
  }
  
  return;
}

void Chemin::actualiser(Point pointGPS){
  // met à jour _PointActuel
  if (_numeroPointActuel == _nombrePoints-1){return;}  // rien à faire : on est à la fin
  
  float distanceToNext = distance(pointGPS, _Points[_numeroPointActuel+1]);
  if (distanceToNext < rover_config.tolerancePosition){ // on est arrivé au point suivant
    _numeroPointActuel++;
    setPointActuel(_Points[_numeroPointActuel]);
    cheminSuivi+=_PointActuel.toString();
  }
  return;
}


/******************
 * autres fonctions
 * ****************/
 
float distance(Point A, Point B) {
  return sqrt( sq(B.getX()-A.getX()) + sq(B.getY()-A.getY()) );
}

void goHome(){
  // retour à la base, suite à un code d'urgence
  goingHome = true;
  
  // générer le chemin à suivre
  // Il est contenu dans les points antérieurs au point actuel
  Chemin cheminBase = Chemin(chemin.getPointActuel(), chemin.getPointDebut());
  for (int i=chemin.getNombrePoints()-2; i>0; i--){ // on parcourt le chemin actuel à rebours
    cheminBase.addPoint(cheminBase.getNombrePoints()-2, chemin.getPointParNumero(i));  // ajout en avant-dernière position
  }

  chemin = cheminBase;
  return;
}
