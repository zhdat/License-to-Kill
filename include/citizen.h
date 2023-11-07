//
// Created by calliste Ravix on 07/11/2023.
//

#ifndef PROJECT_CITIZEN_H
#define PROJECT_CITIZEN_H

typedef struct citizen_s citizen_t;

struct citizen_s{
    int health_points;  /*10 Max*/
    int coord_x;
    int coord_y;
    int (*action)(citizen_t *);
};

#endif //PROJECT_CITIZEN_H
