#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H
#include "population.hpp"
#include "../include/utils.hpp"
#include "../include/node.hpp"
#include <algorithm>
#include <random>
#include <unordered_map>

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

        const std::unordered_map<std::string, int> getValues() const { return this->m_values; }
        void setValues(std::unordered_map<std::string, int>& values) { this->m_values = values; }

        double getBestFitness() const { return this->m_bestFitness; }
        void setBestFitness(double bestFitness) { this->m_bestFitness = bestFitness; }

    private:
        Population m_population;
        std::vector<Node> m_nodes;
        std::unordered_map<std::string, int> m_values;
        double m_bestFitness;

        void Initialize(int, int, int);

        void Evolve(int, int, int);

        std::vector<Chromosome> ParentTournamentSelection(int, int);

        Chromosome Tournament(std::vector<Chromosome>&, int);

        std::vector<Chromosome> CrossoverOX(Chromosome&, Chromosome&);

        std::vector<int> RemoveSeparator(const std::vector<int>&);

        std::vector<int> AddSeparator(const std::vector<int>&);

        void SwapMutation(std::vector<Chromosome>&);

        void SurviveSelection(std::vector<Chromosome>&);

        void Evaluation();

        //TODO: LER DE DATASETS E INICIALIZAR COM ELE
    };
}

#endif