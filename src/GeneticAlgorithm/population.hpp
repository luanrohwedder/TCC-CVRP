#ifndef POPULATION_H
#define POPULATION_H
#include "chromosome.hpp"
#include <iomanip>
#include <algorithm>

namespace GA
{
    /**
     * @brief Represents a Population in genetic algorithm.
     *
     * A populations is a collection of Individuals, where each individual represents a solution.
     * The populations has a defined mSize and keep track of the current mGeneration.
     *
     */
    class Population
    {
    public:
        Population() : m_individuals(), m_size(0), m_generation(0) {}

        Population(std::vector<Chromosome> &individuals, int mSize, int mGeneration)
            : m_individuals(individuals), m_size(mSize), m_generation(mGeneration) {}

        /**
         * @brief Prints the current population.
         */
        inline void PrintPopulation()
        {
            double bestFitness = this->m_individuals[0].getFitness();

            std::cout << "Generation - " << this->m_generation << std::endl;
            for (auto &individual :this->m_individuals)
            {
                for (const auto& gene : individual.getDNA())
                    std::cout << std::setw(2) << gene << " ";

                std::cout << " -  " << individual.getFitness() << std::endl;

                if (individual.getFitness() < bestFitness)
                    bestFitness = individual.getFitness();
            }
            std::cout << "Best Fitness: " << bestFitness << std::endl << std::endl;
        }

        /**
         * @brief Find if the given Individual is already in population.
         * 
         * @param chromosome Chromosome to search for.
         * 
         * @return boolean.
         */
        inline bool contains(const Chromosome &chromosome) const
        {
            return std::find(m_individuals.begin(), m_individuals.end(), chromosome) != m_individuals.end();
        }

        // GETTERS AND SETTERS
        std::vector<Chromosome> &getIndividuals() { return this->m_individuals; }
        void setIndividuals(const std::vector<Chromosome> &individuals) { this->m_individuals = individuals; }

        int getSize() const { return this->m_size; }
        void setSize(int mSize) { this->m_size = mSize; }

        int getGeneration() const { return this->m_generation; }
        void setGeneration(int mGeneration) { this->m_generation = mGeneration; }

    private:
        std::vector<Chromosome> m_individuals;
        int m_size;
        int m_generation;
    };
}

#endif