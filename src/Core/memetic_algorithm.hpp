#ifndef MEMETIC_ALGORITHM_H
#define MEMETIC_ALGORITHM_H
#include "genetic_algorithm.hpp"

namespace MA
{
    /**
     * @brief Represents a memetic algorithm that extends a genetic algorithm with local search techniques.
     *
     * A memetic algorithm is a hybrid approach that combines genetic algorithms with local search methods.
     * This class extends the genetic algorithm by incorporating specific local search strategies, such as
     * hill climbing and simulated annealing, to improve individual solutions.
     */
    class MemeticAlgorithm : public GA::GeneticAlgorithm
    {
    public:
        MemeticAlgorithm() {};

        void setLS(std::string LS) { this->LS = LS; }

    private:
        std::string LS;

        /**
         * @brief Applies a local search to a population of chromosomes.
         *
         * @param population A vector of chromosomes to which the local search will be applied.
         */
        void ApplyLocalSeach(std::vector<GA::Chromosome>&) override;

        /**
         * @brief Performs hill climbing on a given chromosome.
         *
         * @param chromosome The chromosome on which hill climbing will be applied.
         */
        void HillClimbing(GA::Chromosome&);

        /**
         * @brief Applies simulated annealing to a given chromosome.
         *
         * @param chromosome The chromosome on which simulated annealing will be applied.
         */
        void SimulatedAnnealing(GA::Chromosome&);

        /**
         * @brief Generates a 2-opt neighbor for a given chromosome.
         *
         * @param chromosome The original chromosome for which a neighboring solution will be generated.
         * 
         * @return A new chromosome representing a 2-opt neighbor of the input chromosome.
         */
        GA::Chromosome GenerateNeighbor2Opt(const GA::Chromosome&);
    };
}

#endif