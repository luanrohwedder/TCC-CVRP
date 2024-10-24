#ifndef RUN_UTILS_H
#define RUN_UTILS_H

#ifdef _WIN32
    #include <sys/stat.h>
    #define mkdir _mkdir
#else
    #include <unistd.h>
    #include <sys/stat.h>
#endif

#include <chrono>
#include "../src/GeneticAlgorithm/genetic_algorithm.hpp"

struct TestResult {
    double fitness;
    double time_ms;
    double time_sec;
};

inline void
SetupFolders(const std::string& folder)
{
std::string folderName;

#ifdef _WIN32
    if (!mkdir("results") != 0 && errno != EEXIST)
        std::cerr << "Failed to create folder results" << std::endl;

    if (!mkdir("results/" + folder + "/avg") != 0 && errno != EEXIST)
        std::cerr << "Failed to create folder results/avg" << std::endl;

    if (!mkdir("results/" + folder + "/test") != 0 && errno != EEXIST)
        std::cerr << "Failed to create folder results/test" << std::endl;
#else
    struct stat info;

    if (stat("results", &info) != 0)
    {
        if (mkdir("results", 0777) != 0)
            std::cerr << "Failed to create folder results" << std::endl;
    }

    folderName = "results/" + folder;
    if (stat(folderName.c_str(), &info) != 0)
    {
        if (mkdir(folderName.c_str(), 0777) != 0)
            std::cerr << "Failed to create folder results/" << folder << std::endl;
    }

    folderName = "results/" + folder + "/avg";
    if (stat(folderName.c_str(), &info) != 0)
    {
        if (mkdir(folderName.c_str(), 0777) != 0)
            std::cerr << "Failed to create folder results/" << folder << "/avg" << std::endl;
    }

    folderName = "results/" + folder + "/test";
    if (stat(folderName.c_str(), &info) != 0)
    {
        if (mkdir(folderName.c_str(), 0777) != 0)
            std::cerr << "Failed to create folder results/" << folder << "/test" << std::endl;
    }
#endif
}

inline std::string 
getFileName(const std::string& filePath) {
    size_t lastSlash = filePath.find_last_of("/\\");
    
    std::string fileName = (lastSlash == std::string::npos) ? filePath : filePath.substr(lastSlash + 1);

    size_t lastDot = fileName.find_last_of(".");

    if (lastDot != std::string::npos)
        return fileName.substr(0, lastDot);

    return fileName;
}

inline void 
RunTests(const std::string &filename, std::vector<int> pop_sizes, std::vector<int> generations_sizes)
{
    SetupFolders(getFileName(filename));

    for (int generation : generations_sizes)
    {
        for (int pop : pop_sizes)
        {
            std::string folder_name = getFileName(filename);
            std::string name =  "POP_" + std::to_string(pop) + "_GEN_" + 
                                std::to_string(generation) + ".txt";
            std::ofstream file("results/" + folder_name+ "/test/" + name);

            if (!file.is_open())
            {
                std::cerr << "Failed to create file: " << name << std::endl;
                return;
            }

            std::unordered_map<std::string, int> values;
            values.insert(std::make_pair("POP_SIZE", pop));
            values.insert(std::make_pair("GENERATIONS", generation));
            values.insert(std::make_pair("PARENTS_SIZE", pop/3));

            std::vector<Node> clientes = utils::ReadNodesFromFile(filename, values);

            for (int i = 0; i < 30; ++i)
            {
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

inline void
ProcessResults (const std::string& filename, std::vector<int> pop_sizes, std::vector<int> generations_sizes)
{
    for (int generation : generations_sizes)
    {
        for (int pop : pop_sizes)
        {
            std::string folder = getFileName(filename);
            std::string text_filename = "POP_" + std::to_string(pop) +
                                        "_GEN_" + std::to_string(generation) + ".txt";
            std::ifstream infile("results/" + folder + "/test/" + text_filename);
            std::vector<TestResult> results;
            
            if (!infile.is_open())
            {
                std::cerr << "Failed to open file: " << text_filename << std::endl;
                return;
            }

            std::string line;

            while (std::getline(infile, line))
            {
                std::stringstream ss(line);
                std::string ignore;
                TestResult result;

                ss >> ignore >> result.fitness >> ignore >> result.time_ms;
                result.time_sec = result.time_ms / 1000000.0;

                results.push_back(result);
            }

            infile.close();

            double sum_fitness = 0, sum_time_ms = 0, sum_time_sec = 0;
            double best_fitness = results[0].fitness;

            for (const auto& result : results)
            {
                sum_fitness += result.fitness;
                sum_time_ms += result.time_ms;
                sum_time_sec += result.time_sec;

                if (result.fitness < best_fitness)
                    best_fitness = result.fitness;
            }

            double avg_fitness = sum_fitness / results.size();
            double avg_time_ms = sum_time_ms / results.size();
            double avg_time_sec = sum_time_sec / results.size();

            double variance_fitness = 0;

            for (const auto& res : results)
                variance_fitness += std::pow(res.fitness - avg_fitness, 2);

            double stddev_fitness = std::sqrt(variance_fitness / results.size());

            std::string out_filename = "RES_" + text_filename;
            std::ofstream outfile("results/" + folder + "/avg/" + out_filename);

            if (!outfile.is_open())
            {
                std::cerr << "Failed to create file: " << out_filename << std::endl;
                return;
            }

            outfile << std::fixed << std::setprecision(6);
            outfile << "Média do fitness: " << avg_fitness << stddev_fitness << std::endl;
            outfile << "Desvio padrão do fitness: " << stddev_fitness << std::endl;
            outfile << "Melhor fitness: " << best_fitness << std::endl;
            outfile << "Média do tempo (ms): " << avg_time_ms << std::endl;
            outfile << "Média do tempo (s): " << avg_time_sec << std::endl;

            outfile.close();
        }
    }
}

#endif