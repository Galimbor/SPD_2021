//
// Created by artur on 3/5/21.
//

#include <sys/time.h>

#ifndef SPD_LAB1_TIMEMEASURINGLIB_H
#define SPD_LAB1_TIMEMEASURINGLIB_H


//----------Functions used to measure the program's execution time--------------
// Author : Ricardo Correia
float timedifference_msec(struct timeval t0, struct timeval t1) {
    return (float) (t1.tv_sec - t0.tv_sec) * 1000.0f + (float) (t1.tv_usec - t0.tv_usec) / 1000.0f;
}




#endif //SPD_LAB1_TIMEMEASURINGLIB_H
