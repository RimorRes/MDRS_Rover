// On admet que la position parfaite Point(0, 0) n'existe pas. Cela permet dedéfinir une position inexistante. 

#include "Arduino.h"
#include "Math.h"

#if !defined DEPLACEMENT_H
#include "deplacement.h"
#define DEPLACEMENT_H
#endif
#if !defined OBSTACLE_H
#include "obstacle.h"
#define OBSTACLE_H
#endif

/*****************************
* GESTION DE obstacles_liste *
*****************************/

Point obstacle_map::obstacle_position_from_rover(Point R, float d, float p)const{
// ...avec R la position du rover, d la distance à l'obstacle et p la direction du rover. 
    return Point(R.getX() + cos(p) * d, R.getY() + cos(p)*d);
}

void obstacle_map::add_obstacle(Point P){
    if(is_existing_obstacle(P, 0.1f)){return;}// (en m)
// Pour ne pas laisser des cases vides...
    for(int i = obstacles_liste_len; i<obstacles_liste_len; i++){
        if(obstacles_liste[i] == Point(0, 0)){
            obstacles_liste[i] = P;
            return;
        }
    }
// Si toutes les cases sont occupés...
    obstacles_liste = new Point[0];
    obstacles_liste_len++;
    obstacles_liste[obstacles_liste_len] = P;
}

void obstacle_map::remove_obstacle(int n){
// ...avec n l'emplacement de l'obstacle à retirer. 
    obstacles_liste[n] = Point(0, 0);
}

bool obstacle_map::is_existing_obstacle(Point P, float r)const{
// ...avec r la précision. 
    for(int i = obstacles_liste_len; i<=obstacles_liste_len; i++){
        if((obstacles_liste[i].getX()*obstacles_liste[i].getX())+(obstacles_liste[i].getY()*obstacles_liste[i].getY())>=r*r){
            return true;
        }
    }
    return false;
}

/********************
* GESTION DU CHEMIN *
********************/