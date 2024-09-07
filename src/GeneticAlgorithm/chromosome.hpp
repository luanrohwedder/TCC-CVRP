#ifndef CHROMOSOME_H
#define CHROMOSOME_H
#include <iostream>
#include <vector>

namespace GA
{
    /**
     * @brief Represents a Chromosome in a genetic algorithm.
     *
     * A chromosome is composed of a fitness value and a DNA sequence represented
     * as a vector of integers. The fitness value indicates how well this chromosome
     * performs with respect to a particular fitness function, while the DNA vector
     * represents the sequence of genes that define the chromosome's characteristics.
     */
    class Chromosome
    {
    public:
        Chromosome() : m_fitness(0.0), m_dna() {}

        Chromosome(double fitness, const std::vector<int> &dna)
            : m_fitness(fitness), m_dna(dna) {}

        bool operator == (const Chromosome &other) const { return this->m_dna == other.m_dna; }

        bool operator < (const Chromosome &other) const { return this->m_fitness == other.m_fitness; }

        // GETTERS AND SETTERS
        double getFitness() const { return this->m_fitness; }
        void setFitness(double fitness) { this->m_fitness = fitness; }

        std::vector<int> &getDNA() { return this->m_dna; }
        void setDNA(const std::vector<int> &dna) { this->m_dna = dna; }

    private:
        double m_fitness;
        std::vector<int> m_dna;
        // TODO: HASHMAP RESTRICTIONS
    };
}

#endif