#include "../include/genetic.h"
#include "../include/tsp.h"
#include "../include/random.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <omp.h>

//--------------------------------------------------
// Crear un individuo aleatorio
//--------------------------------------------------

Individual createRandomIndividual(
    const std::vector<City>& cities)
{
    Individual individual;

    int n = static_cast<int>(cities.size());

    individual.route.resize(n);

    // 0,1,2,3,4,...

    std::iota(
        individual.route.begin(),
        individual.route.end(),
        0);

    // Mezclar aleatoriamente

    std::shuffle(
        individual.route.begin(),
        individual.route.end(),
        getGenerator());

    individual.fitness = 0.0;

    return individual;
}

//--------------------------------------------------
// Generar población
//--------------------------------------------------

Population generatePopulation(
    const std::vector<City>& cities,
    const GAConfig& config)
{
    Population population;

    for (int i = 0; i < config.populationSize; i++)
    {
        population.individuals.push_back(
            createRandomIndividual(cities));
    }

    return population;
}

//--------------------------------------------------

void evaluatePopulation(
    Population& population,
    const std::vector<std::vector<double>>& distanceMatrix)
{
#pragma omp parallel for schedule(static)
    for (int i = 0;
         i < static_cast<int>(population.individuals.size());
         i++)
    {
        population.individuals[i].fitness =
            calculateFitness(
                population.individuals[i],
                distanceMatrix);
    }
}


//--------------------------------------------------
// Tournament Selection
//--------------------------------------------------

Individual selectParent(
    const Population& population,
    const GAConfig& config)
{
    std::uniform_int_distribution<int> dist(
        0,
        static_cast<int>(population.individuals.size()) - 1);

    Individual best =
        population.individuals[
            dist(getGenerator())];

    for (int i = 1;
         i < config.tournamentSize;
         i++)
    {
        Individual candidate =
            population.individuals[
                dist(getGenerator())];

        if (candidate.fitness <
            best.fitness)
        {
            best = candidate;
        }
    }

    return best;
}

Population selection(
    const Population& population,
    const GAConfig& config)
{
    Population parents;

    for (int i = 0; i < config.populationSize; i++)
    {
        parents.individuals.push_back(
            selectParent(
                population,
                config));
    }

    return parents;
}

//--------------------------------------------------
// Ordered Crossover (OX)
//--------------------------------------------------

Individual orderedCrossover(
    const Individual& parent1,
    const Individual& parent2)
{
    int n =
        static_cast<int>(parent1.route.size());

    Individual child;

    child.route.assign(n, -1);

    std::uniform_int_distribution<int> dist(0, n - 1);

    int start = dist(getGenerator());
    int end = dist(getGenerator());

    if (start > end)
        std::swap(start, end);

    //------------------------------------------------
    // Copiar segmento del Padre 1
    //------------------------------------------------

    for (int i = start; i <= end; i++)
    {
        child.route[i] = parent1.route[i];
    }

    //------------------------------------------------
    // Completar usando Padre 2
    //------------------------------------------------

    int current = (end + 1) % n;

    for (int i = 0; i < n; i++)
    {
        int city =
            parent2.route[(end + 1 + i) % n];

        bool exists = false;

        for (int value : child.route)
        {
            if (value == city)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            child.route[current] = city;

            current++;

            if (current == n)
                current = 0;
        }
    }

    child.fitness = 0.0;

    return child;
}

Population crossover(
    const Population& parents,
    const GAConfig& config)
{
    Population children;

    children.individuals.resize(
        config.populationSize);

#pragma omp parallel for schedule(static)
    for (int i = 0;
         i < config.populationSize;
         i += 2)
    {
        const Individual& parent1 =
            parents.individuals[i];

        const Individual& parent2 =
            parents.individuals[
                (i + 1) % config.populationSize];

        children.individuals[i] =
            orderedCrossover(
                parent1,
                parent2);

        children.individuals[i + 1] =
            orderedCrossover(
                parent2,
                parent1);
    }

    return children;
}

//--------------------------------------------------
// Swap Mutation
//--------------------------------------------------

void mutateIndividual(
    Individual& individual,
    const GAConfig& config)
{
    std::uniform_real_distribution<double>
        probability(0.0, 1.0);

    if (probability(getGenerator()) >
        config.mutationRate)
    {
        return;
    }

    int n =
        static_cast<int>(individual.route.size());

    std::uniform_int_distribution<int>
        position(0, n - 1);

    int i =
        position(getGenerator());

    int j =
        position(getGenerator());

    while (i == j)
    {
        j = position(getGenerator());
    }

    std::swap(
        individual.route[i],
        individual.route[j]);
}

void mutation(
    Population& population,
    const GAConfig& config)
{
#pragma omp parallel for schedule(static)
    for (int i = 0;
         i < static_cast<int>(population.individuals.size());
         i++)
    {
        mutateIndividual(
            population.individuals[i],
            config);
    }
}

//--------------------------------------------------
// Mejor individuo
//--------------------------------------------------

Individual getBestIndividual(
    const Population& population)
{
    Individual best =
        population.individuals.front();

    for (const auto& individual :
         population.individuals)
    {
        if (individual.fitness < best.fitness)
        {
            best = individual;
        }
    }

    return best;
}

//--------------------------------------------------
// Estadísticas de la población
//--------------------------------------------------

PopulationStats calculateStatistics(
    const Population& population)
{
    PopulationStats stats;

    stats.bestFitness =
        population.individuals.front().fitness;

    stats.worstFitness =
        population.individuals.front().fitness;

    double sum = 0.0;

    for (const auto& individual :
         population.individuals)
    {
        sum += individual.fitness;

        if (individual.fitness <
            stats.bestFitness)
        {
            stats.bestFitness =
                individual.fitness;
        }

        if (individual.fitness >
            stats.worstFitness)
        {
            stats.worstFitness =
                individual.fitness;
        }
    }

    stats.averageFitness =
        sum / population.individuals.size();

    return stats;
}

//--------------------------------------------------
// Peor individuo
//--------------------------------------------------

Individual getWorstIndividual(
    const Population& population)
{
    Individual worst =
        population.individuals.front();

    for (const auto& individual :
         population.individuals)
    {
        if (individual.fitness >
            worst.fitness)
        {
            worst = individual;
        }
    }

    return worst;
}

//--------------------------------------------------
// Elitismo
//--------------------------------------------------

void applyElitism(
    Population& population,
    const Individual& elite)
{
    int worstIndex = 0;

    for (int i = 1;
         i < population.individuals.size();
         i++)
    {
        if (population.individuals[i].fitness >
            population.individuals[worstIndex].fitness)
        {
            worstIndex = i;
        }
    }

    population.individuals[worstIndex] =
        elite;
}

Individual runGeneticAlgorithm(
    const std::vector<City>& cities,
    const GAConfig& config)
{
    omp_set_num_threads(config.threads);

    //-----------------------------------------
    // Construir matriz
    //-----------------------------------------

    auto distanceMatrix =
        buildDistanceMatrix(cities);

    //-----------------------------------------
    // Generar población inicial
    //-----------------------------------------

    Population population =
        generatePopulation(
            cities,
            config);

    evaluatePopulation(
        population,
        distanceMatrix);

    //Exportamos a csv
    std::ofstream csv("statistics.csv");

    csv << "Generation,"
        << "Best,"
        << "Average,"
        << "Worst\n";

    //-----------------------------------------
    // Evolución
    //-----------------------------------------

    for (int generation = 0;
         generation < config.generations;
         generation++)
    {
        Individual elite =
            getBestIndividual(
                population);

        Population parents =
            selection(
                population,
                config);

        Population children =
            crossover(
                parents,
                config);

        mutation(
            children,
            config);

        evaluatePopulation(
            children,
            distanceMatrix);
        
        applyElitism(
            children,
            elite);

        population = children;

        PopulationStats stats =
            calculateStatistics(population);
        
        csv
        << generation + 1 << ","
        << std::fixed
        << std::setprecision(3)
        << stats.bestFitness << ","
        << stats.averageFitness << ","
        << stats.worstFitness
        << "\n";

        std::cout
            << "Generacion "
            << generation + 1
            << " | Mejor: "
            << stats.bestFitness
            << " | Promedio: "
            << stats.averageFitness
            << " | Peor: "
            << stats.worstFitness
            << '\n';
    }

    csv.close();
    return getBestIndividual(population);
}