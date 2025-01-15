#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "../Core/chromosome.hpp"
#include "../Core/population.hpp"
#include <unordered_set>

namespace {

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
    inline GA::Chromosome
    CreateChild(const std::vector<int> &parent1, const std::vector<int> &parent2, int start, int end)
    {
        std::vector<int> dna(parent1.size(), -1);
        std::unordered_set<int> genesInChild;

        for (int i = start; i <= end; ++i)
        {
            dna[i] = parent1[i];
            genesInChild.insert(parent1[i]);
        }

        size_t current = end + 1;
        size_t parentIndex = end + 1;

        while (genesInChild.size() < dna.size())
        {
            if (parentIndex >= parent2.size())
                parentIndex = 0;

            int gene = parent2[parentIndex];

            if (genesInChild.find(gene) == genesInChild.end())
            {
                if (current >= dna.size())
                    current = 0;

                if (dna[current] == -1)
                {
                    dna[current] = gene;
                    genesInChild.insert(gene);
                    current++;
                }
            }
            parentIndex++;
        }
        
        GA::Chromosome child;
        child.setDNA(dna);
        return child;
    }

    /**
     * @brief Performs OX Crossover between two parents.
     *
     * @param parent1 The first parent.
     * @param parent2 The second parent.
     * @param population
     *
     * @return The resulting children of crossover.
     */
    inline std::vector<GA::Chromosome>
    CrossoverOX(GA::Chromosome &parent1, GA::Chromosome &parent2, GA::Population& population)
    {
        std::vector<int> cleanParent1Dna = parent1.getDNA();
        std::vector<int> cleanParent2Dna = parent2.getDNA();

        int p1 = utils::randInteger(1, cleanParent1Dna.size() - 3);
        int p2 = utils::randInteger(p1 + 1, cleanParent2Dna.size() - 2);

        GA::Chromosome child1 = CreateChild(cleanParent1Dna, cleanParent2Dna, p1, p2);
        GA::Chromosome child2 = CreateChild(cleanParent2Dna, cleanParent1Dna, p1, p2);

        std::vector<GA::Chromosome> children;
        if (!population.contains(child1))
            children.push_back(child1);

        if (!population.contains(child2))
            children.push_back(child2);

        return children;
    }

    /**
     * @brief Performs the Swap Mutation in each gene.
     *
     * @param population
     * @param children Vector of child.
     */
    void
    SwapMutation(GA::Population& population, std::vector<GA::Chromosome> &children)
    {
        for (auto &child : children)
        {
            bool mutated = false;
            std::vector<int> originalDNA = child.getDNA();

            for (size_t j = 1; j < child.getDNA().size() - 1; ++j)
            {
                if (utils::randDouble(0, 1) < 0.05)
                {
                    size_t n1;

                    do
                    {
                        n1 = utils::randInteger(1, child.getDNA().size() - 2);
                    } while (n1 == j);

                    std::swap(child.getDNA()[j], child.getDNA()[n1]);
                    mutated = true;
                }
            }

            if (mutated && population.contains(child))
                child.setDNA(originalDNA);
        }
    }
}

namespace OP
{
    /**
     * @brief Performs Crossover and Mutation
     *
     * @param parents Selected parents for crossover.
     * @param population
     *
     * @return The resulting children of crossover and mutation.
     */
    inline 
    std::vector<GA::Chromosome> CrossoverMutation(std::vector<GA::Chromosome>& parents, GA::Population& population)
    {
        std::vector<GA::Chromosome> children;

        for (size_t i = 0; i < parents.size(); i += 2)
        {
            std::vector<GA::Chromosome> newChildren = CrossoverOX(parents[i], parents[i + 1], population);
            children.insert(children.end(), newChildren.begin(), newChildren.end());
        }

        SwapMutation(population, children);

        for (auto &child : children)
        {
            child.CalculateFitness(population.getNodes(), population.getCapacity());
        }

        return children;
    }
}

#endif