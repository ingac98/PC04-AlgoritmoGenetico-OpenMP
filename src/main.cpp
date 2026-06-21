#include "../include/tsp.h"
#include "../include/config.h"

#include <iostream>
#include <chrono>

int main()
{
    std::cout << "=====================================\n";
    std::cout << " Algoritmo Genetico con OpenMP\n";
    std::cout << "=====================================\n\n";

    std::vector<City> cities =
        readCities("data/cities.txt");

    std::cout << "Ciudades cargadas: "
              << cities.size()
              << "\n\n";

    for (const auto& city : cities)
    {
        std::cout
            << "ID: " << city.id
            << "   X: " << city.x
            << "   Y: " << city.y
            << std::endl;
    }

    GAConfig config =
        loadConfiguration("config.txt");

    std::cout
    << "\nCONFIGURACION\n\n";

std::cout
    << "Poblacion      : "
    << config.populationSize
    << '\n';

std::cout
    << "Generaciones   : "
    << config.generations
    << '\n';

std::cout
    << "Crossover      : "
    << config.crossoverRate
    << '\n';

std::cout
    << "Mutacion       : "
    << config.mutationRate
    << '\n';

std::cout
    << "Tournament     : "
    << config.tournamentSize
    << '\n';

std::cout
    << "Threads        : "
    << config.threads
    << "\n\n";

    auto start =
    std::chrono::high_resolution_clock::now();

    Individual best =
        runGeneticAlgorithm(
            cities,
            config);

    auto end =
    std::chrono::high_resolution_clock::now();

    auto duration =
    std::chrono::duration_cast<
        std::chrono::milliseconds>(
            end - start);
    
    std::cout
        << "\n\n===========================\n";

    std::cout
        << "MEJOR SOLUCION\n";

    std::cout
        << "===========================\n\n";

    for (int city : best.route)
    {
        std::cout
            << city
            << " ";
    }

    std::cout
        << "\n\nFitness = "
        << best.fitness
        << '\n';

    std::cout
        << "\nTiempo de ejecucion: "
        << duration.count()
        << " ms\n";
    
    return 0;
}