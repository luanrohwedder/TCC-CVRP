#include "memetic_algorithm.hpp"

namespace MA
{
    void MemeticAlgorithm::ApplyLocalSeach(std::vector<GA::Chromosome>& children)
    {
        double lsProb = 0.5;

        for (auto& child : children)
        {
            double randProb = utils::randDouble(0, 1);

            if (randProb < lsProb)
            {
                child.setDNA(RemoveSeparator(child.getDNA()));
                SimulatedAnnealing(child);
            }
        }
    }

    void MemeticAlgorithm::HillClimbing(GA::Chromosome& child)
    {
        int maxIterations = 100;
        
        GA::Chromosome bestSolution = child;
        double bestFitness = child.getFitness();
        
        for (int i = 0; i < maxIterations; ++i)
        {
            GA::Chromosome neighbor = GenerateNeighbor2Opt(bestSolution);
            neighbor.setDNA(AddSeparator(neighbor.getDNA()));

            double neighborFitness = CalculateFitness(neighbor);

            if (neighborFitness < bestFitness)
            {
                neighbor.setDNA(RemoveSeparator(neighbor.getDNA()));
                bestSolution = neighbor;
                bestFitness = neighborFitness;
            }
        }

        bestSolution.setDNA(AddSeparator(bestSolution.getDNA()));
        bestSolution.setFitness(bestFitness);

        child = bestSolution;
    }

    void MemeticAlgorithm::SimulatedAnnealing(GA::Chromosome& child)
    {
        double temperature = 10;
        double cooling = 0.95;

        GA::Chromosome bestSolution = child;
        double bestFitness = child.getFitness();

        while (temperature > 0.1)
        {
            GA::Chromosome neighbor = GenerateNeighbor2Opt(bestSolution);
            neighbor.setDNA(AddSeparator(neighbor.getDNA()));

            double neighborFitness = CalculateFitness(neighbor);

            if (neighborFitness < bestFitness)
            {
                neighbor.setDNA(RemoveSeparator(neighbor.getDNA()));
                bestSolution = neighbor;
                bestFitness = neighborFitness;
            }
            else
            {
                double acceptProbability = exp((bestFitness - neighborFitness) / temperature);
                double randVal = utils::randDouble(0, 1);

                if (randVal < acceptProbability)
                {
                    neighbor.setDNA(RemoveSeparator(neighbor.getDNA()));
                    bestSolution = neighbor;
                    bestFitness = neighborFitness;
                }
            }
            
            temperature *= cooling;
        }
        
        bestSolution.setDNA(AddSeparator(bestSolution.getDNA()));
        bestSolution.setFitness(bestFitness);

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