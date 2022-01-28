/***********************************************************************
 * Définition des classes utiles pour gérer la position, la vitesse, ...
************************************************************************/

#include <Arduino.h>

class Point
{
  public:
    void init(float X, float Y, float T);
    float getX();
    float getY();
    float getT();
    void setX(float X);
    void setY(float Y);
    void setT(float T);
    String affichage(); // ajouté pour les besoins de test
  private:
    float _X;
    float _Y;
    float _T;
};

class Chemin  // utilise la classe Point
{
  public:
    void init(Point PointDebut, Point PointFin);
    Point getPointDebut();
    Point getPointFin();
    Point getPointActuel();
    void setPointDebut(Point PointDebut);
    void setPointFin(Point PointFin);
    void setPointActuel(Point PointActuel);
    int getNumeroPointActuel();
    int getNombrePoints();
    String affichage(); // ajouté pour les besoins de test
    Point avancerPointSuivant();
    Point getPointParNumero(int numero);
    void addPoint(int numero, Point point);
    void removePoint(int numero);
  private:
    Point _PointDebut;
    Point _PointFin;
    Point _PointActuel;
    int _numeroPointActuel;
    int _nombrePoints;
    Point _Points[];
};
