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
                child.setDNA(utils::RemoveSeparator(child.getDNA()));

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
        double bestFitness = child.getFitness();
        
        for (int i = 0; i < maxIterations && noImprovementCounter < maxNoImprovement; ++i)
        {
            GA::Chromosome neighbor = GenerateNeighbor2Opt(bestSolution);
            neighbor.setDNA(utils::AddSeparator(neighbor.getDNA(), this->getNodes(), this->getValues().at("CAPACITY")));
            neighbor.CalculateFitness(this->getNodes());

            if (neighbor.getFitness() < bestFitness)
            {
                neighbor.setDNA(utils::RemoveSeparator(neighbor.getDNA()));
                bestSolution = neighbor;
                bestFitness = neighbor.getFitness();
                noImprovementCounter = 0;
            }
            else
            {
                noImprovementCounter++;
            }
        }

        bestSolution.setDNA(utils::AddSeparator(bestSolution.getDNA(),this->getNodes(), this->getValues().at("CAPACITY")));
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
            neighbor.setDNA(utils::AddSeparator(neighbor.getDNA(), this->getNodes(), this->getValues().at("CAPACITY")));

            neighbor.CalculateFitness(this->getNodes());

            if (neighbor.getFitness() < bestFitness)
            {
                neighbor.setDNA(utils::RemoveSeparator(neighbor.getDNA()));
                bestSolution = neighbor;
                bestFitness = neighbor.getFitness();
            }
            else
            {
                double acceptProbability = exp((bestFitness - neighbor.getFitness()) / temperature);
                double randVal = utils::randDouble(0, 1);

                if (randVal < acceptProbability)
                {
                    neighbor.setDNA(utils::RemoveSeparator(neighbor.getDNA()));
                    bestSolution = neighbor;
                    bestFitness = neighbor.getFitness();
                }
            }
            
            temperature *= cooling;
        }
        
        bestSolution.setDNA(utils::AddSeparator(bestSolution.getDNA(), this->getNodes(), this->getValues().at("CAPACITY")));
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