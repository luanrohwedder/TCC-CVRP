#include "genetic_algorithm.hpp"
#include "../Operators/selection.hpp"
#include "../Operators/crossover_mutation.hpp"
#include "../Generate/kmeans.hpp"
#include "../Generate/nearest_neighbor.hpp"
#include <unordered_set>

namespace GA
{
    GeneticAlgorithm::GeneticAlgorithm() {};

    Population GeneticAlgorithm::Run()
    {
        Initialize();
        this->m_population.Evaluation();

        for (int i = 0; i < this->m_param->generation; ++i)
        {
            this->m_population.setGeneration(i + 1);
            Evolve();
            this->m_population.Evaluation();

            if (this->m_population.getBestFitness() < this->m_population.getLastBestFitness())
                this->m_generations_no_improvements = 0;
            else
                this->m_generations_no_improvements++;

            if (this->m_generations_no_improvements >= this->m_param->stagnation_limit)
                break;
        }

        return this->m_population;
    }

    void GeneticAlgorithm::Initialize()
    {
        Population population;
        population.setNodes(this->getNodes());
        population.setSize(this->m_param->population);
        population.setCapacity(this->m_param->capacity);

        std::vector<Chromosome> individuals;

        for (int i = 0; i < this->m_param->population/2; ++i)
        {
            Chromosome chromosome;

            GEN::KMeans kmeans(4, this->m_nodes, this->m_param->max_x, this->m_param->max_y);
            kmeans.run();
            GEN::NearestNeighbor NN(kmeans.getClusters());
            NN.run();

            chromosome.setDNA(NN.getDna());
            chromosome.setFitness(-1.0);

            individuals.push_back(chromosome);
        }

        for (int i = 0; i < this->m_param->population/2; ++i)
        {
            Chromosome chromosome;

            std::vector<int> dna(this->m_param->dimension - 1);
            std::iota(dna.begin(), dna.end(), 1);
            std::shuffle(dna.begin(), dna.end(), std::mt19937{std::random_device{}()});

            chromosome.setDNA(dna);
            chromosome.setFitness(-1.0);

            individuals.push_back(chromosome);
        }

        population.setIndividuals(individuals);
        population.setGeneration(1);
        this->m_population = population;
        this->m_generations_no_improvements = 0;
        this->m_population.setBestFitness(std::numeric_limits<double>::max());
        this->m_population.setLastBestFitness(std::numeric_limits<double>::max());
    }

    void GeneticAlgorithm::Evolve()
    {
        std::vector<Chromosome> parents = OP::TournamentSelection(this->m_population, this->m_param->parent_size);
        std::vector<Chromosome> children = OP::CrossoverMutation(parents, this->m_population);

        ApplyLocalSeach(children);

        this->m_population.SurviveSelection(children);
    }
}