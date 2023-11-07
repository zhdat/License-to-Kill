//
// Created by calliste Ravix on 07/11/2023.
//

#ifndef PROJECT_CASE_OFFICER_H
#define PROJECT_CASE_OFFICER_H

typedef struct case_officer_s case_officer_t;

struct case_officer_s {
    int health_points;  /*10 Max*/
    int coord_x;
    int coord_y;
    int (*action)(case_officer_t *);
};

#endif //PROJECT_CASE_OFFICER_H
