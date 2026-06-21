#ifndef GENETIC_H
#define GENETIC_H

#include <vector>
#include <string>
#include <algorithm>
#include <random>

/*--------------------------------------------------
    Representa una ciudad del TSP
---------------------------------------------------*/
struct City
{
    int id;
    double x;
    double y;
};

/*--------------------------------------------------
    Representa un individuo del algoritmo genético
---------------------------------------------------*/
struct Individual
{
    std::vector<int> route;
    double fitness;
};

/*--------------------------------------------------
    Población
---------------------------------------------------*/
struct Population
{
    std::vector<Individual> individuals;
};

/*--------------------------------------------------
    Parámetros del algoritmo
---------------------------------------------------*/
struct GAConfig
{
    int populationSize;

    int generations;

    int tournamentSize;

    int threads;

    double crossoverRate;

    double mutationRate;
};

/*--------------------------------------------------
    Estadísticas de una generación
---------------------------------------------------*/
struct PopulationStats
{
    double bestFitness;
    double averageFitness;
    double worstFitness;
};

Individual createRandomIndividual(
    const std::vector<City>& cities);

Population generatePopulation(
    const std::vector<City>& cities,
    const GAConfig& config);

/*--------------------------------------------------
    Funciones principales
---------------------------------------------------*/

Population generatePopulation(
    const std::vector<City>& cities,
    const GAConfig& config);

void evaluatePopulation(
    Population& population,
    const std::vector<std::vector<double>>& distanceMatrix);


Individual selectParent(
    const Population& population,
    const GAConfig& config);
    
Population selection(
    const Population& population,
    const GAConfig& config);

Individual orderedCrossover(
    const Individual& parent1,
    const Individual& parent2);

Population crossover(
    const Population& parents,
    const GAConfig& config);

void mutateIndividual(
    Individual& individual,
    const GAConfig& config);

void mutation(
    Population& population,
    const GAConfig& config);

Individual getBestIndividual(
    const Population& population);

PopulationStats calculateStatistics(
    const Population& population);

Individual getWorstIndividual(
    const Population& population);

void applyElitism(
    Population& population,
    const Individual& elite);

Individual runGeneticAlgorithm(
    const std::vector<City>& cities,
    const GAConfig& config);

#endif