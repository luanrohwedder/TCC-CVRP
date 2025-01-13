#include "genetic_algorithm.hpp"
#include "../Operators/selection.hpp"
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
        this->m_mutationRate = 0.05;
    }

    void GeneticAlgorithm::Evolve(int populationSize)
    {
        std::vector<Chromosome> parents = OP::TournamentSelection(this->m_population, this->m_values.at("PARENTS_SIZE"));

        std::vector<Chromosome> children = PerformCrossoverMutation(parents);

        ApplyLocalSeach(children);
        
        int genInterval = 100;
        if (this->m_population.getGeneration() % genInterval == 0)
        {
            InsertRandomIndividuals(populationSize, children);
        }

        this->m_population.SurviveSelection(children);
    }

    std::vector<Chromosome> GeneticAlgorithm::PerformCrossoverMutation(std::vector<Chromosome>& parents)
    {
        std::vector<Chromosome> children;

        for (size_t i = 0; i < parents.size(); i += 2)
        {
            std::vector<Chromosome> newChildren = CrossoverOX(parents[i], parents[i + 1]);
            children.insert(children.end(), newChildren.begin(), newChildren.end());
        }

        SwapMutation(children);

        for (auto &child : children)
        {
            child.setDNA(AddSeparator(child.getDNA()));
            child.CalculateFitness(this->m_nodes);
        }

        return children;
    }

    std::vector<Chromosome> GeneticAlgorithm::CrossoverOX(Chromosome &parent1, Chromosome &parent2)
    {
        std::vector<int> cleanParent1Dna = RemoveSeparator(parent1.getDNA());
        std::vector<int> cleanParent2Dna = RemoveSeparator(parent2.getDNA());

        int p1 = utils::randInteger(1, cleanParent1Dna.size() - 3);
        int p2 = utils::randInteger(p1 + 1, cleanParent2Dna.size() - 2);

        Chromosome child1 = CreateChild(cleanParent1Dna, cleanParent2Dna, p1, p2);
        Chromosome child2 = CreateChild(cleanParent2Dna, cleanParent1Dna, p1, p2);

        std::vector<Chromosome> children;
        if (!m_population.contains(child1))
            children.push_back(child1);

        if (!m_population.contains(child2))
            children.push_back(child2);

        return children;
    }

    Chromosome GeneticAlgorithm::CreateChild(const std::vector<int> &parent1, const std::vector<int> &parent2, int start, int end)
    {
        Chromosome child;
        std::vector<int> dna = CreatePartialChild(parent1, start, end);
        InsertRemainingGenes(dna, parent2, start, end);
        child.setDNA(dna);
        return child;
    }

    std::vector<int> GeneticAlgorithm::CreatePartialChild(const std::vector<int> &parent, int start, int end)
    {
        std::vector<int> child(parent.size(), -1);

        for (int i = start; i <= end; ++i)
            child[i] = parent[i];

        return child;
    }

    void GeneticAlgorithm::InsertRemainingGenes(std::vector<int> &child, const std::vector<int> &parent, int start, int end)
    {
        std::unordered_set<int> genesInChild(child.begin() + start, child.begin() + end + 1);
        size_t current = end + 1;
        size_t parentIndex = end + 1;
        size_t genesInserted = end - start + 1;

        while (genesInserted < child.size())
        {
            if (parentIndex >= parent.size())
                parentIndex = 0;

            int gene = parent[parentIndex];

            if (genesInChild.find(gene) == genesInChild.end())
            {
                if (current >= child.size())
                    current = 0;

                if (child[current] == -1)
                {
                    child[current] = gene;
                    genesInChild.insert(gene);
                    genesInserted++;
                    current++;
                }
            }
            parentIndex++;
        }
    }

    std::vector<int> GeneticAlgorithm::RemoveSeparator(const std::vector<int> &dna)
    {
        std::vector<int> res = dna;
        res.erase(std::remove(res.begin(), res.end(), 0), res.end());
        return res;
    }

    std::vector<int> GeneticAlgorithm::AddSeparator(const std::vector<int> &dna)
    {
        std::vector<int> res;
        int cap = 0;

        res.push_back(0);
        for (size_t i = 0; i < dna.size(); ++i)
        {
            int demand = m_nodes[dna[i]].getDemand();

            if (demand > m_values["CAPACITY"])
                throw std::runtime_error("Client demand bigger than vehicle cap");

            if (cap + demand > m_values["CAPACITY"])
            {
                res.push_back(0);
                cap = 0;
            }

            res.push_back(dna[i]);
            cap += demand;
        }
        res.push_back(0);

        return res;
    }

    void GeneticAlgorithm::SwapMutation(std::vector<Chromosome> &children)
    {
        for (auto &child : children)
        {
            bool mutated = false;
            std::vector<int> originalDNA = child.getDNA();

            for (size_t j = 1; j < child.getDNA().size() - 1; ++j)
            {
                if (utils::randDouble(0, 1) < m_mutationRate)
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

            if (mutated && m_population.contains(child))
                child.setDNA(originalDNA);
        }
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