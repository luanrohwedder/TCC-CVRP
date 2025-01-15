#ifndef SELECTION_H
#define SELECTION_H

#include "node.hpp"
#include "../Core/chromosome.hpp"
#include "../Core/population.hpp"

namespace {
    /**
     * @brief Tournament in a subset of the population (selected parents).
     *
     * @param T Selected parents.
     * @param k Size of the subset.
     *
     * @return Winner chromosome (parent).
     */
    static GA::Chromosome Tournament(std::vector<GA::Chromosome> &T, int k)
    {
        GA::Chromosome bestFitness = T[0];

        for (int i = 1; i < k; ++i)
            if (T[i].getFitness() < bestFitness.getFitness())
                bestFitness = T[i];

        return bestFitness;
    }
}

namespace OP
{
    /**
     * @brief Parent Tournament for selecting parents to crossover.
     *
     * @param population
     * @param parantSize Size of parents to pick up for crossover.
     *
     * @return Winners chromosomes (parents) for crossover.
     */
    inline std::vector<GA::Chromosome> TournamentSelection(GA::Population &population, int parentsSize)
    {
        std::vector<GA::Chromosome> aux_population = population.getIndividuals();
        std::vector<GA::Chromosome> newIndividuals;
        std::vector<GA::Chromosome> parents;

        int subsetSize = (population.getIndividuals().size() / 3) + 2;

        for (int i = 0; i < parentsSize; ++i)
        {
            newIndividuals.clear();

            std::vector<GA::Chromosome> temp_population = aux_population;

            std::sample(aux_population.begin(), aux_population.end(),
                        std::back_inserter(newIndividuals), subsetSize,
                        std::mt19937{std::random_device{}()});

            GA::Chromosome winner = Tournament(newIndividuals, subsetSize);
            parents.push_back(winner);

            auto it = std::find(aux_population.begin(), aux_population.end(), winner);
            if (it != aux_population.end())
                aux_population.erase(it);
        }

        if (parents.size() % 2 != 0)
            parents.pop_back();

        return parents;
    }
}

#endif