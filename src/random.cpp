#include "../include/random.h"

#include <chrono>

/*--------------------------------------------------
    Generador local para cada hilo
---------------------------------------------------*/

std::mt19937& getGenerator()
{
    thread_local std::mt19937 generator(
        static_cast<unsigned>(
            std::chrono::steady_clock::now()
            .time_since_epoch()
            .count()));

    return generator;
}

/*--------------------------------------------------
    Entero aleatorio
---------------------------------------------------*/

int randomInt(
    int min,
    int max)
{
    std::uniform_int_distribution<int>
        distribution(min, max);

    return distribution(
        getGenerator());
}

/*--------------------------------------------------
    Real aleatorio
---------------------------------------------------*/

double randomDouble(
    double min,
    double max)
{
    std::uniform_real_distribution<double>
        distribution(min, max);

    return distribution(
        getGenerator());
}