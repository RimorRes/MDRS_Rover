#include "Arduino.h"

class Obstacle{
public:
    Point obstaclePositionFromRover(Point R, float d, float p)const;
    void addObstacle(Point P, float r_ignore);
    void removeObstacle(int n);
    bool isExistingObstacle(Point P, float r)const;
    Point cheminCorrection(Point P, float r, float p)const;
    void updateObstaclesListe;
private:
    Point _obstaclesListe[20];
    int _obstaclesListeLen = 0;// nombre d'élement dans _obstaclesListe
    const int _obstaclesListeSize = 20;
};
