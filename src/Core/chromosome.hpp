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

        /**
         * @brief Calculate the firness of an Individual.
         * 
         * @param nodes
         * @param capacity
         */
        inline void CalculateFitness(const std::vector<Node>& nodes, int capacity)
        {
            updateRoutes(nodes, capacity);

            double fitness = 0.0;
            
            for (const auto& route : m_routes)
            {
                double routeCost = 0.0;

                routeCost += utils::Distance(nodes[0], nodes[route[0]]);

                for (size_t i = 0; i < route.size() - 1; ++i)
                    routeCost += utils::Distance(nodes[route[i]], nodes[route[i + 1]]);

                routeCost += utils::Distance(nodes[route.back()], nodes[0]);

                fitness += routeCost;
            }

            this->setFitness(fitness);
        }

        // GETTERS AND SETTERS
        double getFitness() const { return this->m_fitness; }
        void setFitness(double fitness) { this->m_fitness = fitness; }

        std::vector<int> &getDNA() { return this->m_dna; }
        void setDNA(const std::vector<int> &dna) { this->m_dna = dna; }

        std::vector<std::vector<int>> &getRoutes() { return this->m_routes; }
        void setRoutes(const std::vector<std::vector<int>> &routes)
        { 
            this->m_routes = routes;
            this->m_dna.clear();
            for (const auto& route : this->m_routes)
                for (int c : route)
                    this->m_dna.push_back(c);
        }

    private:
        double m_fitness;
        std::vector<int> m_dna;
        std::vector<std::vector<int>> m_routes;

        inline void updateRoutes(const std::vector<Node>& nodes, int capacity)
        {
            Split(nodes, capacity);
        }

        inline void Split(const std::vector<Node>& nodes, int capacity)
        {
            size_t n = m_dna.size();
            std::vector<double> cost(n + 1, std::numeric_limits<double>::max());
            std::vector<int> pred(n + 1, -1);
            cost[0] = 0;

            for (size_t i = 0; i < n; ++i)
            {
                int load = 0;
                double dist = 0;

                for (size_t j = i; j < n; ++j)
                {
                    int customer = m_dna[j];
                    load += nodes[customer].getDemand();
                    if (load > capacity)
                        break;

                    if (j == i)
                        dist = utils::Distance(nodes[0], nodes[customer]) + utils::Distance(nodes[customer], nodes[0]);
                    else
                    {
                        int prevCustomer = m_dna[j - 1];
                        dist += utils::Distance(nodes[prevCustomer], nodes[customer]) 
                                + utils::Distance(nodes[customer], nodes[0])
                                - utils::Distance(nodes[prevCustomer], nodes[0]);

                    }

                    double newCost = cost[i] + dist;

                    if (newCost < cost[j + 1])
                    {
                        cost[j + 1] = newCost;
                        pred[j + 1] = i;
                    }
                }
            }

            m_routes.clear();
            for (int i = static_cast<int>(n); i > 0; i = pred[i])
            {
                int start = pred[i];
                m_routes.emplace_back(m_dna.begin() + start, m_dna.begin() + i);
            }
            std::reverse(m_routes.begin(), m_routes.end());
        }
    };
}

#endif