/*********************
 * Gestion des obstacles
**********************/

#include "deplacement.cpp"

// cartographie les obstacles afin de les éviter
class obstacle_map{
    // la carte est ces descriptions
    int map_precision = 100;// multiple de 1cm (par défaut 100<=>1m)
    /*
    *************** 
    *      |      *
    *  mp  |  pp  *
    *------+------*
    *  mm  |  pm  *
    *      |      *
    ***************
    */
    bool map_pp[999][999];// <=> 1km*1km
    bool map_pm[999][999];
    bool map_mp[999][999];
    bool map_mm[999][999];

    // place les nouveaux obstacles de la carte
    void new_obstacle(Point position_new_obstacle){}

    // retire les anciens obstacles de la carte
    void delete_obstacle(Point position_old_obstacle){}

    // cherche si l'obstacle est préexistante sur la carte
    int search_obstacle(Point position_unidentified_obstacle){}
};