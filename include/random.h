#ifndef RANDOM_H
#define RANDOM_H

#include <random>

/*--------------------------------------------------
    Generador aleatorio por hilo
---------------------------------------------------*/

std::mt19937& getGenerator();

int randomInt(
    int min,
    int max);

double randomDouble(
    double min,
    double max);

#endif