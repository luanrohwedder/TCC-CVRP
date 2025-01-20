#include "memetic_algorithm.hpp"

namespace MA
{
    void MemeticAlgorithm::ApplyLocalSeach(std::vector<GA::Chromosome>& children)
    {
        double lsProb = 0.5;

        #pragma omp parallel for
        for (auto& child : children)
        {
            double randProb = utils::randDouble(0, 1);

            if (randProb < lsProb)
            {
                if (LS == "H")
                    HillClimbing(child);
                
                if (LS == "S")
                    SimulatedAnnealing(child);
            }
        }
    }

    void MemeticAlgorithm::HillClimbing(GA::Chromosome& child)
    {
        int maxIterations = 100;
        int maxNoImprovement = 25;
        int noImprovementCounter = 0;
        
        GA::Chromosome bestSolution = child;
        
        for (int i = 0; i < maxIterations && noImprovementCounter < maxNoImprovement; ++i)
        {
            GA::Chromosome neighbor = GenerateNeighbor2Opt(bestSolution);
            neighbor.CalculateFitness(this->getNodes(), this->getValues().at("CAPACITY"));

            if (neighbor.getFitness() < bestSolution.getFitness())
            {
                bestSolution = neighbor;
                noImprovementCounter = 0;
            }
            else
            {
                noImprovementCounter++;
            }
        }

        child = bestSolution;
    }

    void MemeticAlgorithm::SimulatedAnnealing(GA::Chromosome& child)
    {
        double temperature = 10;
        double cooling = 0.95;

        GA::Chromosome bestSolution = child;

        while (temperature > 0.1)
        {
            GA::Chromosome neighbor = GenerateNeighbor2Opt(bestSolution);
            neighbor.CalculateFitness(this->getNodes(), this->getValues().at("CAPACITY"));

            if (neighbor.getFitness() < bestSolution.getFitness())
            {
                bestSolution = neighbor;
            }
            else
            {
                double acceptProbability = exp((bestSolution.getFitness() - neighbor.getFitness()) / temperature);
                double randVal = utils::randDouble(0, 1);

                if (randVal < acceptProbability)
                    bestSolution = neighbor;
            }
            
            temperature *= cooling;
        }

        child = bestSolution;
    }

    GA::Chromosome MemeticAlgorithm::GenerateNeighbor2Opt(const GA::Chromosome& solution)
    {
        GA::Chromosome neighbor = solution;
        int i = utils::randInteger(0, neighbor.getDNA().size() - 1);
        int j = utils::randInteger(0, neighbor.getDNA().size() - 1);

        if (i > j)
            std::swap(i, j);
        
        std::reverse(neighbor.getDNA().begin() + i, neighbor.getDNA().begin() + j);

        return neighbor;
    }
}