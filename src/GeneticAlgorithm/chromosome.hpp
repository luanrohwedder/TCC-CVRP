#ifndef CHROMOSOME_H
#define CHROMOSOME_H
#include <iostream>
#include <vector>
#include "node.hpp"
#include "utils.hpp"

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

        /**
         * @brief Calculate the firness of an Individual.
         * 
         * @param nodes
         * @param capacity
         */
        inline void CalculateFitness(const std::vector<Node>& nodes, int capacity)
        {
            double fitness = 0.0;
            //double currentCapacity = 0.0;
            const auto &dna = AddSeparator(this->m_dna, nodes, capacity);

            if (dna.size() < 2)
            {
                this->setFitness(fitness);
                return;
            }

            
            for (size_t i = 1; i < dna.size(); ++i)
                fitness += utils::EuclidianDistance(
                    nodes[dna[i - 1]].getX(), nodes[dna[i]].getX(),
                    nodes[dna[i - 1]].getY(), nodes[dna[i]].getY());

            this->setFitness(fitness);
        }

        // GETTERS AND SETTERS
        double getFitness() const { return this->m_fitness; }
        void setFitness(double fitness) { this->m_fitness = fitness; }

        std::vector<int> &getDNA() { return this->m_dna; }
        void setDNA(const std::vector<int> &dna) { this->m_dna = dna; }

    private:
        double m_fitness;
        std::vector<int> m_dna;

        inline std::vector<int> AddSeparator(const std::vector<int>& dna, const std::vector<Node>& nodes, int capacity)
        {
            std::vector<int> res;
            int cap = 0;

            res.push_back(0);
            for (size_t i = 0; i < dna.size(); ++i)
            {
                int demand = nodes[dna[i]].getDemand();

                if (demand > capacity)
                    throw std::runtime_error("Client demand bigger than vehicle cap");

                if (cap + demand > capacity)
                {
                    res.push_back(0);
                    cap = 0;
                }

                res.push_back(dna[i]);
                cap += demand;
            }
            res.push_back(0);

            return res;
        }
    };
}

#endif