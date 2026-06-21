#ifndef TSP_H
#define TSP_H

#include <vector>
#include <string>

#include "genetic.h"

std::vector<City> readCities(
    const std::string& filename);

double distance(
    const City& a,
    const City& b);

std::vector<std::vector<double>> buildDistanceMatrix(
    const std::vector<City>& cities);

double calculateFitness(
    const Individual& individual,
    const std::vector<std::vector<double>>& distanceMatrix);
#endif

