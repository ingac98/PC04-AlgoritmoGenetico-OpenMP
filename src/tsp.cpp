#include "../include/tsp.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<City> readCities(const std::string& filename)
{
    std::vector<City> cities;

    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: No se pudo abrir el archivo "
                  << filename << std::endl;

        return cities;
    }

    City city;

    while (file >> city.id >> city.x >> city.y)
    {
        cities.push_back(city);
    }

    file.close();

    return cities;
}

double distance(
    const City& a,
    const City& b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;

    return std::sqrt(dx * dx + dy * dy);
}

std::vector<std::vector<double>> buildDistanceMatrix(
    const std::vector<City>& cities)
{
    int n = static_cast<int>(cities.size());

    std::vector<std::vector<double>> matrix(
        n,
        std::vector<double>(n, 0.0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = distance(cities[i], cities[j]);
        }
    }

    return matrix;
}

double calculateFitness(
    const Individual& individual,
    const std::vector<std::vector<double>>& distanceMatrix)
{
    double totalDistance = 0.0;

    int n = static_cast<int>(individual.route.size());

    for (int i = 0; i < n - 1; i++)
    {
        int from = individual.route[i];
        int to = individual.route[i + 1];

        totalDistance += distanceMatrix[from][to];
    }

    // Regresar a la ciudad inicial

    totalDistance += distanceMatrix[
        individual.route.back()][individual.route.front()];

    return totalDistance;
}