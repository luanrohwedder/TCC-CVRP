#include "genetic_algorithm.hpp"
#include "../Operators/selection.hpp"
#include "../Operators/crossover_mutation.hpp"
#include <unordered_set>

namespace GA
{
    GeneticAlgorithm::GeneticAlgorithm() {};

    Population GeneticAlgorithm::Run()
    {
        Initialize();
        this->m_population.Evaluation();

        for (int i = 0; i < this->m_values.at("GENERATIONS"); ++i)
        {
            Evolve();
            this->m_population.Evaluation();
        }

        return this->m_population;
    }

    void GeneticAlgorithm::Initialize()
    {
        Population population;
        population.setNodes(this->getNodes());
        population.setSize(this->m_values.at("POP_SIZE"));
        population.setCapacity(this->m_values.at("CAPACITY"));

        std::vector<Chromosome> individuals;

        for (int i = 0; i < this->m_values.at("POP_SIZE"); ++i)
        {
            Chromosome chromosome;

            std::vector<int> dna(this->m_values.at("DIMENSION") - 1);
            std::iota(dna.begin(), dna.end(), 1);
            std::shuffle(dna.begin(), dna.end(), std::mt19937{std::random_device{}()});

            chromosome.setDNA(dna);
            chromosome.setFitness(-1.0);

            individuals.push_back(chromosome);
        }

        population.setIndividuals(individuals);
        population.setGeneration(1);
        this->m_population = population;
        this->m_population.setBestFitness(std::numeric_limits<double>::max());
    }

    void GeneticAlgorithm::Evolve()
    {
        std::vector<Chromosome> parents = OP::TournamentSelection(this->m_population, this->m_values.at("PARENTS_SIZE"));
        std::vector<Chromosome> children = OP::CrossoverMutation(parents, this->m_population);

        ApplyLocalSeach(children);
        
        int genInterval = 100;
        if (this->m_population.getGeneration() % genInterval == 0)
        {
            InsertRandomIndividuals(children);
        }

        this->m_population.SurviveSelection(children);
    }

    void GeneticAlgorithm::InsertRandomIndividuals(std::vector<Chromosome> &children)
    {
        int numNewIndividuals = static_cast<int>(this->m_values.at("POP_SIZE") * 0.05);

        for (int i = 0; i < numNewIndividuals; ++i)
        {
            Chromosome newIndividual = GenerateRandomIndividual();

            newIndividual.CalculateFitness(this->m_nodes, this->m_values.at("CAPACITY"));

            if (newIndividual.getFitness() < this->m_population.getBestFitness() * 1.5)
                children.push_back(newIndividual);
        }
    }

    Chromosome GeneticAlgorithm::GenerateRandomIndividual()
    {
        Chromosome newChromosome;

        std::vector<int> dna(this->m_values.at("DIMENSION") - 1);
        std::iota(dna.begin(), dna.end(), 1);
        std::shuffle(dna.begin(), dna.end(), std::mt19937{std::random_device{}()});

        newChromosome.setDNA(dna);
        newChromosome.setFitness(-1.0);
        return newChromosome;
    }
}