/*********************
 * Gestion des obstacles
**********************/

#include "deplacement.cpp"

// cartographie les obstacles afin de les éviter
class obstacle_map{
    // la carte est ces descriptions
    int map_precision = 100;// multiple de 1cm (par défaut 100<=>1m)
    struct map{};
    // 1km*1km où 1 unité <=> map_precision cm | true si un obstacle est présent

    // place les nouveaux obstacles de la carte
    void new_obstacle(Point position_new_obstacle){}

    // retire les anciens obstacles de la carte
    void delete_obstacle(Point position_old_obstacle){}

    // cherche si l'obstacle est préexistante sur la carte
    int search_obstacle(Point position_unidentified_obstacle, int range = 1){// precision en unité de map_precision, par défaut 1m
        for(size_t x; x<=(position_unidentified_obstacle.getX() + range) && x>=(position_unidentified_obstacle.getX() - range); x++){
            for(size_t y; y<=(position_unidentified_obstacle.getY() + range) && y>=(position_unidentified_obstacle.getY() - range); y++){
                if((int)position_new_obstacle.getX())
            }
        }
    }
};