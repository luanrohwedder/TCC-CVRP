#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H
#include "population.hpp"
#include <random>
#include <unordered_map>
#include <fstream>

namespace GA
{
    /**
     * @brief Represents a genetic algorithm.
     *
     * This class encapsulates the functionalities and operations of a genetic algorithm,
     * including initialization, selection, crossover, mutation, and evaluation of individuals.
     * It manages the population of individuals and drives the evolution process across generations.
     */
    class GeneticAlgorithm
    {
    public:
        GeneticAlgorithm();

        /**
         * @brief Runs the genetic algorithm.
         *
         * This method orchestrates the process of evolution, calling other methods
         * to initialize the population, perform selection, crossover, mutation,
         * and evaluation.
         *
         * @return The final population after running the algorithm.
         */
        Population Run();

        //GETTERS AND SETTERS
        Population getPopulation() const { return this->m_population; }

        const std::vector<Node> getNodes() const { return this->m_nodes; }
        void setNodes(std::vector<Node>& nodes) { this->m_nodes = nodes; }

        const Parameters* getParameters() const { return this->m_param; }
        void setParameters(Parameters* param) { this->m_param = param; }

    private:
        Population m_population;
        std::vector<Node> m_nodes;
        Parameters* m_param;

        int m_generations_no_improvements;

        /**
         * @brief Initialize the genetic algorithm.
         */
        void Initialize();

        /**
         * @brief Process the evolution of GA.
         */
        void Evolve();

    protected:
        /**
         * @brief Apply local search in GA to become an MA
         * 
         * @param children children to apply local search
         */
        virtual void ApplyLocalSeach(std::vector<Chromosome>&) {};
    };
}

#endif