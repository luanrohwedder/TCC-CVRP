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
     * The populations has a defined mSize and keep track of the current mGeneration and mBestFitness.
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
         * @brief Evaluation of the population.
         * 
         * @param nodes
         */
        inline void Evaluation()
        {
            this->m_lastBestFitness = this->m_bestFitness;

            for (auto &individual : this->getIndividuals())
            {
                if (individual.getFitness() == -1.0)
                {
                    individual.CalculateFitness(this->m_nodes, this->m_capacity);
                }

                if (individual.getFitness() < this->getBestFitness())
                    this->setBestFitness(individual.getFitness());
            }
        }

        /**
         * @brief Performs the Survive Selection for next generation.
         *
         * @param children Vector of child.
         */
        inline void SurviveSelection(std::vector<Chromosome> &children)
        {
            std::vector<Chromosome> &population = this->getIndividuals();

            std::vector<int> index(population.size());
            std::iota(index.begin(), index.end(), 0);

            std::sort(index.begin(), index.end(), [&population](int i1, int i2)
                      { return population[i1].getFitness() > population[i2].getFitness(); });

            for (size_t i = 0; i < children.size(); ++i)
                if (!this->contains(children[i]))
                    population[index[i]] = children[i];

            this->setGeneration(this->getGeneration() + 1);
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

        std::vector<Node> &getNodes() { return this->m_nodes; }
        void setNodes(const std::vector<Node> &nodes) { this->m_nodes = nodes; }

        int getSize() const { return this->m_size; }
        void setSize(int mSize) { this->m_size = mSize; }

        int getGeneration() const { return this->m_generation; }
        void setGeneration(int mGeneration) { this->m_generation = mGeneration; }

        double getCapacity() const { return this->m_capacity; }
        void setCapacity(double mCapacity) { this->m_capacity = mCapacity; }

        double getBestFitness() const { return this->m_bestFitness; }
        void setBestFitness(double mBestFitness) { this->m_bestFitness = mBestFitness; }

        double getLastBestFitness() const { return this->m_lastBestFitness; }
        void setLastBestFitness(double mLastBestFitness) { this->m_lastBestFitness = mLastBestFitness; }

    private:
        std::vector<Chromosome> m_individuals;
        std::vector<Node> m_nodes;

        int m_size;
        int m_generation;
        double m_capacity;
        double m_bestFitness;
        double m_lastBestFitness;
    };
}

#endif