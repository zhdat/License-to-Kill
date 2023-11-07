//
// Created by calliste Ravix on 07/11/2023.
//

#ifndef PROJECT_COUNTER_INTELLIGENCE_OFFICER_H
#define PROJECT_COUNTER_INTELLIGENCE_OFFICER_H

typedef struct counter_intelligence_officer_s counter_intelligence_officer_t;

struct counter_intelligence_officer_s {
    int health_points;  /*10 Max*/
    int coord_x;
    int coord_y;
    int (*action)(counter_intelligence_officer_t *);
};

#endif //PROJECT_COUNTER_INTELLIGENCE_OFFICER_H
