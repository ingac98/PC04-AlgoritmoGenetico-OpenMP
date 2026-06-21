#include "../include/config.h"

#include <fstream>
#include <iostream>
#include <sstream>

GAConfig loadConfiguration(
    const std::string& filename)
{
    GAConfig config;

    config.populationSize = 100;
    config.generations = 50;
    config.crossoverRate = 0.80;
    config.mutationRate = 0.10;
    config.tournamentSize = 3;
    config.threads = 4;

    std::ifstream file(filename);

    if (!file)
    {
        std::cout
            << "No se pudo abrir "
            << filename
            << std::endl;

        return config;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        std::stringstream ss(line);

        std::string key;
        std::string value;

        std::getline(ss, key, '=');
        std::getline(ss, value);

        if (key == "PopulationSize")
            config.populationSize = std::stoi(value);

        else if (key == "Generations")
            config.generations = std::stoi(value);

        else if (key == "CrossoverRate")
            config.crossoverRate = std::stod(value);

        else if (key == "MutationRate")
            config.mutationRate = std::stod(value);

        else if (key == "TournamentSize")
            config.tournamentSize = std::stoi(value);

        else if (key == "Threads")
            config.threads = std::stoi(value);
    }

    return config;
}