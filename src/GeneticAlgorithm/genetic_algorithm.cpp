#include "genetic_algorithm.hpp"

namespace GA
{
    GeneticAlgorithm::GeneticAlgorithm() {};

    Population GeneticAlgorithm::Run()
    {
        Initialize(m_values.at("POP_SIZE"), m_values.at("CAPACITY"), m_values.at("DIMENSION"));
        Evaluation();

        this->m_population.PrintPopulation();
        std::cout << "Best Global Fitness: " << this->m_bestFitness << std::endl << std::endl;

        for (int i = 0; i < m_values.at("GENERATIONS"); ++i)
        {
            Evolve(m_values.at("PARENTS_SIZE"), m_values.at("POP_SIZE"), m_values.at("DIMENSION"));
            Evaluation();
            this->m_population.PrintPopulation();
            std::cout << "Best Global Fitness: " << this->m_bestFitness << std::endl << std::endl;
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

            // Create Nodes random order
            std::vector<int> remainingClients(numClients - 1);
            std::iota(remainingClients.begin(), remainingClients.end(), 1);
            std::shuffle(remainingClients.begin(), remainingClients.end(), std::mt19937{std::random_device{}()});

            // dna.push_back(0);

            for (auto &index : remainingClients)
            {
                int demand = this->m_nodes[index].getDemand();

                if (demand > capacity)
                    throw std::runtime_error("Client demand bigger than vehicle cap");

                if (cap + demand > capacity)
                {
                    dna.push_back(-1);
                    cap = 0;
                }

                dna.push_back(index);
                cap += demand;
            }

            chromosome.setDNA(dna);

            individuals.push_back(chromosome);
        }

        population.setIndividuals(individuals);
        population.setGeneration(1);
        this->m_population = population;
        this->setBestFitness(std::numeric_limits<double>::max());
    }

    void GeneticAlgorithm::Evolve(int parentsSize, int populationSize, int numClients)
    {
        std::vector<Chromosome> parents = ParentTournamentSelection(parentsSize, populationSize);

        if (parents.size() % 2 != 0)
            parents.pop_back();

        std::vector<Chromosome> children;
        for (size_t i = 0; i < parents.size(); i += 2)
        {
            std::vector<Chromosome> newChildren = CrossoverOX(parents[i], parents[i + 1]);
            children.insert(children.end(), newChildren.begin(), newChildren.end());
        }

        SwapMutation(children);
        
        for (auto& child : children)
            child.setDNA(AddSeparator(child.getDNA()));

        
        SurviveSelection(children);
    }

    std::vector<Chromosome> GeneticAlgorithm::ParentTournamentSelection(int parentsSize, int populationSize)
    {
        std::vector<Chromosome> aux_population = m_population.getIndividuals();
        std::vector<Chromosome> newIndividuals;
        std::vector<Chromosome> parents;
        int popSize;

        int subsetSize = (m_population.getIndividuals().size() / 3) + 2;

        for (int i = 0; i < parentsSize; ++i)
        {
            popSize = aux_population.size();
            newIndividuals.clear();

            std::vector<Chromosome> temp_population = aux_population;

            for (int j = 0; j < subsetSize; ++j)
            {
                int index = utils::randInteger(0, popSize - 1);
                newIndividuals.push_back(temp_population[index]);
                temp_population.erase(temp_population.begin() + index);
                popSize--;
            }

            Chromosome winner = Tournament(newIndividuals, subsetSize);
            parents.push_back(winner);

            auto it = std::find(aux_population.begin(), aux_population.end(), winner);
            if (it != aux_population.end())
                aux_population.erase(it);
        }

        return parents;
    }

    Chromosome GeneticAlgorithm::Tournament(std::vector<Chromosome> &T, int k)
    {
        Chromosome bestFitness = T[0];

        for (int i = 1; i < k; ++i)
            if (T[i].getFitness() < bestFitness.getFitness())
                bestFitness = T[i];

        return bestFitness;
    }

    std::vector<Chromosome> GeneticAlgorithm::CrossoverOX(Chromosome &parent1, Chromosome &parent2)
    {
        int dnaSize = m_values["DIMENSION"];

        Chromosome child1;
        Chromosome child2;

        std::vector<Chromosome> children;
        std::vector<int> dna(dnaSize - 1, -1);
        std::vector<int> dna2(dnaSize - 1, -1);
        std::vector<int> cleanParent1Dna = RemoveSeparator(parent1.getDNA());
        std::vector<int> cleanParent2Dna = RemoveSeparator(parent2.getDNA());

        int p1 = utils::randInteger(1, dnaSize - 4);
        int p2 = utils::randInteger(p1 + 1, dnaSize - 3);

        for (int i = p1; i <= p2; ++i)
        {
            dna[i] = cleanParent1Dna[i];
            dna2[i] = cleanParent2Dna[i];
        }

        int current = 0;
        for (int i = 0; i < dnaSize && current < dnaSize; ++i)
        {
            int gene = cleanParent2Dna[i];
            if (std::find(dna.begin(), dna.end(), gene) == dna.end())
            {
                dna[current] = gene;
                current++;
            }

            if (current == p1)
                current = p2 + 1;
        }

        current = 0;
        for (int i = 0; i < dnaSize && current < dnaSize; ++i)
        {
            int gene = cleanParent1Dna[i];
            if (std::find(dna2.begin(), dna2.end(), gene) == dna2.end())
            {
                dna2[current] = gene;
                current++;
            }

            if (current == p1)
                current = p2 + 1;
        }

        child1.setDNA(dna);
        child2.setDNA(dna2);

        children.push_back(child1);
        children.push_back(child2);

        return children;
    }

    std::vector<int> GeneticAlgorithm::RemoveSeparator(const std::vector<int> &dna)
    {
        std::vector<int> res = dna;
        res.erase(std::remove(res.begin(), res.end(), -1), res.end());
        return res;
    }

    std::vector<int> GeneticAlgorithm::AddSeparator(const std::vector<int> &dna)
    {
        std::vector<int> res;
        int cap = 0;

        for (size_t i = 0; i < dna.size(); ++i)
        {
            int demand = m_nodes[dna[i]].getDemand();

            if (demand > m_values["CAPACITY"])
                throw std::runtime_error("Client demand bigger than vehicle cap");

            if (cap + demand > m_values["CAPACITY"])
            {
                res.push_back(-1);
                cap = 0;
            }

            res.push_back(dna[i]);
            cap += demand;
        }

        return res;
    }

    void GeneticAlgorithm::SwapMutation(std::vector<Chromosome> &children)
    {
        for (auto &child : children)
        {
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
                }
            }
        }
    }

    void GeneticAlgorithm::SurviveSelection(std::vector<Chromosome> &children)
    {
        std::vector<Chromosome> &population = this->m_population.getIndividuals();

        std::vector<int> index(population.size());
        std::iota(index.begin(), index.end(), 0);

        std::sort(index.begin(), index.end(), [&population](int i1, int i2)
                  { return population[i1].getFitness() > population[i2].getFitness(); });

        for (size_t i = 0; i < children.size(); ++i)
            population[index[i]] = children[i];

        this->m_population.setGeneration(this->m_population.getGeneration() + 1);
    }

    void GeneticAlgorithm::Evaluation()
    {
        double bestFitness = this->getBestFitness();

        if (m_nodes.empty())
        {
            std::cerr << "Error: Nodes is not initialized." << std::endl;
            return;
        }

        if (m_population.getIndividuals().empty())
        {
            std::cerr << "Error: Population is not initialized." << std::endl;
            return;
        }

        for (auto &individual : this->m_population.getIndividuals())
        {
            int lastNode = 0;
            double fitness = 0.0;
            const auto &dna = individual.getDNA();

            if (dna.size() < 2)
            {
                individual.setFitness(0.0);
                continue;
            }

            for (size_t i = 1; i < dna.size(); ++i)
            {
                int currentNode = dna[i];

                if (currentNode == -1)
                {
                    fitness += utils::EuclidianDistance(
                        m_nodes[lastNode].getX(), m_nodes[0].getX(),
                        m_nodes[lastNode].getY(), m_nodes[0].getY());

                    lastNode = 0;
                }
                else
                {
                    if (lastNode == 0)
                        fitness += utils::EuclidianDistance(
                            m_nodes[0].getX(), m_nodes[currentNode].getX(),
                            m_nodes[0].getY(), m_nodes[currentNode].getY());
                    else
                        fitness += utils::EuclidianDistance(
                            m_nodes[lastNode].getX(), m_nodes[currentNode].getX(),
                            m_nodes[lastNode].getY(), m_nodes[currentNode].getY());

                    lastNode = currentNode;
                }
            }

            if (lastNode != 0)
                fitness += utils::EuclidianDistance(
                    m_nodes[lastNode].getX(), m_nodes[0].getX(),
                    m_nodes[lastNode].getY(), m_nodes[0].getY());

            individual.setFitness(fitness);

            if (fitness < bestFitness)
                bestFitness = fitness;
        }

        this->setBestFitness(bestFitness);
    }
}