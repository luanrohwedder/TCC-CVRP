#ifndef MEMETIC_ALGORITHM_H
#define MEMETIC_ALGORITHM_H
#include "../GeneticAlgorithm/genetic_algorithm.hpp"

namespace MA
{
    class MemeticAlgorithm : public GA::GeneticAlgorithm
    {
    public:
        MemeticAlgorithm() {};

    private:
        void ApplyLocalSeach(std::vector<GA::Chromosome>&) override;
    };
}

#endif