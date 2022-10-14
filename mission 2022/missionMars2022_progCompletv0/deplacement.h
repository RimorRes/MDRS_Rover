/***********************************************************************
 * Définition des classes utiles pour gérer la position, la vitesse, ...
************************************************************************/

#include <Arduino.h>

class Point
{
  public:
    Point();
    Point(float X, float Y);
    Point(float X, float Y, float T);
    float getX() const;
    float getY() const;
    float getT() const;
    void setX(float X);
    void setY(float Y);
    void setT(float T);
    String affichage() const; // chaîne formattée avec les 3 coordonnées. ajouté pour les besoins de test
    String toString() const;  // chaîne formattée avec les 2 coordonnées (sans _T)
    Point relative(Point PointOrigine) const; // pour changer l'origine du repère
  private:
    float _X;
    float _Y;
    float _T;
};

class Chemin  // utilise la classe Point
{
  public:
    Chemin();
    Chemin(Point pointDebut, Point pointFin);
    Point getPointDebut() const;
    Point getPointFin() const;
    Point getPointActuel() const;
    void setPointDebut(Point pointDebut);
    void setPointFin(Point pointFin);
    void setPointActuel(Point pointActuel);
    int getNumeroPointActuel() const;
    int getNombrePoints() const;
    String affichage() const; // ajouté pour les besoins de test. Chaine formattée avec les points.
    Point avancerPointSuivant(); // le point actuel passe au point suivant 
    Point getPointParNumero(int numero) const;
    void addPoint(int numero, Point point);
    void addPoint(Point point); // l'ajout du point se fait à la fin
    void removePoint(int numero);
    Chemin cheminInverse() const; // renvoie le chemin inverse, ne modifie rien
    String goToNext();  // Génère les ordres de marche permettant de rejoindre le point suivant
    void recalculer();  // calcule des points intermédiaires entre l'actuel et le final
    void actualiser(Point pointGPS);  // met à jour _PointActuel et liste cheminSuivi
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
