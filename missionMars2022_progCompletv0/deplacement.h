/***********************************************************************
 * Définition des classes utiles pour gérer la position, la vitesse, ...
************************************************************************/

#include <Arduino.h>

class Point
{
  public:
    Point();
    Point(float X, float Y, float T);
    float getX() const;
    float getY() const;
    float getT() const;
    void setX(float X);
    void setY(float Y);
    void setT(float T);
    String affichage() const; // ajouté pour les besoins de test
  private:
    float _X;
    float _Y;
    float _T;
};

class Chemin  // utilise la classe Point
{
  public:
    Chemin();
    Chemin(Point PointDebut, Point PointFin);
    Point getPointDebut() const;
    Point getPointFin() const;
    Point getPointActuel() const;
    void setPointDebut(Point PointDebut);
    void setPointFin(Point PointFin);
    void setPointActuel(Point PointActuel);
    int getNumeroPointActuel() const;
    int getNombrePoints() const;
    String affichage() const; // ajouté pour les besoins de test
    Point avancerPointSuivant();  
    Point getPointParNumero(int numero) const;
    void addPoint(int numero, Point point);
    void removePoint(int numero);
    Chemin cheminRetour();
    String goToNext();  // Génère les ordres de marche permettant de rejoindre le point suivant
  private:
    Point _PointDebut;
    Point _PointFin;
    Point _PointActuel;
    int _numeroPointActuel;
    int _nombrePoints;
    Point _Points[];
};

float distance(Point A, Point B);

void goHome();
