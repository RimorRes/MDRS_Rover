#include "Arduino.h"

const int obstaclesListeSize = 20;

struct ObstacleObject{
    Point pos;
    float TTL;
};

class Obstacle{
public:
    Point obstaclePositionFromRover(Point R, float d, float p)const;
    void addObstacle(ObstacleObject obj, float r_ignore);
    void removeObstacle(int n);
    bool isExistingObstacle(Point P, float r)const;
    Point cheminCorrection(Point P, float r, float p)const;
private:
    ObstacleObject _obstaclesListe[obstaclesListeSize];
    int _obstaclesListeLen = 0;// nombre d'élement dans _obstaclesListe
    const int _obstaclesListeSize = obstaclesListeSize;
};
