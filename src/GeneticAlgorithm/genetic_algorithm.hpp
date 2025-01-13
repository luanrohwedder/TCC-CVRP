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

        const std::unordered_map<std::string, int> getValues() const { return this->m_values; }
        void setValues(std::unordered_map<std::string, int>& values) { this->m_values = values; }

    private:
        Population m_population;
        std::vector<Node> m_nodes;
        std::unordered_map<std::string, int> m_values;
        double m_mutationRate;

        /**
         * @brief Initialize the genetic algorithm.
         *
         * @param populationSize The size of population.
         * @param capacity The capacity of vehicle.
         * @param numClient Numbers of clients per route, i.e, size of the chromosome.
         */
        void Initialize(int, int, int);

        /**
         * @brief Process the evolution of GA
         * 
         * @param parantSize Size of parents to pick up for crossover.
         * @param populationSize The size of population.
         */
        void Evolve(int, int);

        /**
         * @brief Parent Tournament for selecting parents to crossover.
         * 
         * @param parantSize Size of parents to pick up for crossover.
         * @param populationSize The size of population.
         * 
         * @return Winners chromosomes (parents) for crossover.
         */
        std::vector<Chromosome> ParentTournamentSelection(int, int);

        /**
         * @brief Tournament in a subset of the population (selected parents).
         * 
         * @param T Selected parents.
         * @param k Size of the subset.
         * 
         * @return Winner chromosome (parent).
         */
        Chromosome Tournament(std::vector<Chromosome>&, int);

        /**
         * @brief Performs Crossover and Mutation
         * 
         * @param parents Selected parents for crossover.
         * 
         * @return The resulting children of crossover and mutation.
         */
        std::vector<Chromosome> PerformCrossoverMutation(std::vector<Chromosome>&);

        /**
         * @brief Performs OX Crossover between two parents.
         * 
         * @param parent1 The first parent.
         * @param parent2 The second parent.
         * 
         * @return The resulting children of crossover.
         */
        std::vector<Chromosome> CrossoverOX(Chromosome&, Chromosome&);

        /**
         * @brief Auxiliar method to create child.
         * 
         * @param parent1 The first parent.
         * @param parent2 The second parent.
         * @param start Start index for copy DNA.
         * @param end End index for copy DNA.
         * 
         * @return The resulting child of crossover.
         */
        Chromosome CreateChild(const std::vector<int>&, const std::vector<int>&, int, int);

        /**
         * @brief Auxiliar method to create partial child (Middle DNA).
         * 
         * @param parent Parent to copy middle DNA.
         * @param start Start index for copy DNA.
         * @param end End index for copy DNA.
         * 
         * @return DNA of the partial child.
         */
        std::vector<int> CreatePartialChild(const std::vector<int>&, int, int);

        /**
         * @brief Auxiliar method to complete the remaining genes.
         * 
         * @param child Partial child.
         * @param parent Parent to copy the remaining genes.
         * @param start Start index for copy DNA.
         * @param end End index for copy DNA.
         */
        void InsertRemainingGenes(std::vector<int>&, const std::vector<int> &, int start, int end);

        /**
         * @brief Performs the Swap Mutation in each gene.
         * 
         * @param children Vector of child.
         */
        void SwapMutation(std::vector<Chromosome>&);

        /**
         * @brief Performs the Survive Selection for next generation.
         * 
         * @param children Vector of child.
         */
        void SurviveSelection(std::vector<Chromosome>&);  

        /**
         * @brief Insert random individuals to compete with newly generated children.
         * 
         * @param populationSize The size of population.
         * @param children Vector of child.
         */
        void InsertRandomIndividuals(int, std::vector<Chromosome> &);

        /**
         * @brief Generate an Random Individual.
         * 
         * @return random individual.
         */
        Chromosome GenerateRandomIndividual();

    protected:
        /**
         * @brief Remove the 0 from Chromosome
         * 
         * @param dna vector of DNA.
         * 
         * @return Clean chromosome without separator.
         */
        std::vector<int> RemoveSeparator(const std::vector<int>&);

        /**
         * @brief Insert the 0 in Chromosome
         * 
         * @param dna vector of DNA.
         * 
         * @return Correct chromosome with separator, respecting the capacity
         */
        std::vector<int> AddSeparator(const std::vector<int>&);

        /**
         * @brief Apply local search in GA to become an MA
         * 
         * @param children children to apply local search
         */
        virtual void ApplyLocalSeach(std::vector<Chromosome>&) {};
    };
}

#endif