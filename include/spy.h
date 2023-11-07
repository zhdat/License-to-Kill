//
// Created by calliste Ravix on 07/11/2023.
//

#ifndef PROJECT_SPY_H
#define PROJECT_SPY_H

typedef struct spyt_s spyt_t;

struct spyt_s {
    int health_points;  /*10 Max*/
    int weapon_bool;    /*0 = no weapon, 1 = weapon*/
    int coord_x;
    int coord_y;
    int (*action)(spyt_t *);
};
#endif //PROJECT_SPY_H
