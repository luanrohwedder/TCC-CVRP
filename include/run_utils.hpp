#ifndef RUN_UTILS_H
#define RUN_UTILS_H

#include <chrono>
#include "../src/GeneticAlgorithm/genetic_algorithm.hpp"

inline void 
RunTests(const std::string &filename, std::vector<int> pop_sizes, std::vector<int> generations_sizes)
{
    for (int generation : generations_sizes)
    {
        for (int pop : pop_sizes)
        {
            std::string name = "results_E_n101/POP_" + std::to_string(pop) + "_GEN_" + 
                                std::to_string(generation) + ".txt";
            std::ofstream file(name);

            for (int i = 0; i < 30; ++i)
            {
                std::unordered_map<std::string, int> values;
                values.insert(std::make_pair("POP_SIZE", pop));
                values.insert(std::make_pair("GENERATIONS", generation));

                values.insert(std::make_pair("PARENTS_SIZE", pop/3));

                std::vector<Node> clientes = utils::ReadNodesFromFile(filename, values);

                GA::GeneticAlgorithm ga;
                ga.setNodes(clientes);
                ga.setValues(values);

                auto start = std::chrono::high_resolution_clock::now();
                ga.Run();
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                file << i + 1 << ": " << ga.getBestFitness() << " - " << duration.count()
                     << "/" << duration.count() / 1000000.0  << std::endl;
            }
        }
    }
}

#endif