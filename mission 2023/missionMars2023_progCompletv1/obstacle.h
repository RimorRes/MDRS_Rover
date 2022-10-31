#include "Arduino.h"

class obstacle_map{
public:
    Point obstacle_position_from_rover(Point R, float d, float p)const;
    void add_obstacle(Point P);
    void remove_obstacle(int n);
    bool is_existing_obstacle(Point P, float r)const;
private:
    Point* obstacles_liste;
    int obstacles_liste_len = 0;// nombre d'élement dans obstacles_liste
};