#include "genetic_algorithm.hpp"
#include "../Operators/selection.hpp"
#include "../Operators/crossover_mutation.hpp"
#include <unordered_set>

namespace GA
{
    GeneticAlgorithm::GeneticAlgorithm() {};

    Population GeneticAlgorithm::Run()
    {
        Initialize(m_values.at("POP_SIZE"), m_values.at("CAPACITY"), m_values.at("DIMENSION"));
        this->m_population.Evaluation(this->m_nodes);

        for (int i = 0; i < m_values.at("GENERATIONS"); ++i)
        {
            Evolve(m_values.at("POP_SIZE"));
            this->m_population.Evaluation(this->m_nodes);
        }

        return this->m_population;
    }

    void GeneticAlgorithm::Initialize(int populationSize, int capacity, int numClients)
    {
        Population population;
        population.setSize(populationSize);
        std::vector<Chromosome> individuals;

        for (int i = 0; i < populationSize; ++i)
        {
            Chromosome chromosome;
            std::vector<int> dna;
            int cap = 0;

            std::vector<int> remainingClients(numClients - 1);
            std::iota(remainingClients.begin(), remainingClients.end(), 1);
            std::shuffle(remainingClients.begin(), remainingClients.end(), std::mt19937{std::random_device{}()});

            dna.push_back(0);
            for (auto &index : remainingClients)
            {
                int demand = this->m_nodes[index].getDemand();

                if (demand > capacity)
                    throw std::runtime_error("Client demand bigger than vehicle cap");

                if (cap + demand > capacity)
                {
                    dna.push_back(0);
                    cap = 0;
                }

                dna.push_back(index);
                cap += demand;
            }
            dna.push_back(0);

            chromosome.setDNA(dna);
            chromosome.setFitness(-1.0);

            individuals.push_back(chromosome);
        }

        population.setIndividuals(individuals);
        population.setGeneration(1);
        this->m_population = population;
        this->m_population.setBestFitness(std::numeric_limits<double>::max());
    }

    void GeneticAlgorithm::Evolve(int populationSize)
    {
        std::vector<Chromosome> parents = OP::TournamentSelection(this->m_population, this->m_values.at("PARENTS_SIZE"));

        std::vector<Chromosome> children = OP::CrossoverMutation(parents, this->m_population, this->m_nodes, this->m_values.at("CAPACITY"));

        ApplyLocalSeach(children);
        
        int genInterval = 100;
        if (this->m_population.getGeneration() % genInterval == 0)
        {
            InsertRandomIndividuals(populationSize, children);
        }

        this->m_population.SurviveSelection(children);
    }

    void GeneticAlgorithm::InsertRandomIndividuals(int populationSize, std::vector<Chromosome> &children)
    {
        int numNewIndividuals = static_cast<int>(populationSize * 0.05);

        for (int i = 0; i < numNewIndividuals; ++i)
        {
            Chromosome newIndividual = GenerateRandomIndividual();

            newIndividual.CalculateFitness(this->m_nodes);

            if (newIndividual.getFitness() < this->m_population.getBestFitness() * 1.5)
                children.push_back(newIndividual);
        }
    }

    Chromosome GeneticAlgorithm::GenerateRandomIndividual()
    {
        Chromosome newChromosome;
        std::vector<int> dna;
        int cap = 0;

        std::vector<int> remainingClients(m_values.at("DIMENSION") - 1);
        std::iota(remainingClients.begin(), remainingClients.end(), 1);
        std::shuffle(remainingClients.begin(), remainingClients.end(), std::mt19937{std::random_device{}()});

        dna.push_back(0);
        for (auto &index : remainingClients)
        {
            int demand = this->m_nodes[index].getDemand();

            if (demand > m_values.at("CAPACITY"))
                throw std::runtime_error("Client demand bigger than vehicle cap");

            if (cap + demand > m_values.at("CAPACITY"))
            {
                dna.push_back(0);
                cap = 0;
            }

            dna.push_back(index);
            cap += demand;
        }
        dna.push_back(0);
        newChromosome.setDNA(dna);
        newChromosome.setFitness(-1.0);
        return newChromosome;
    }
}