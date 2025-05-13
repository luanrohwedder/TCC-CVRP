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
#include "../src/Core/genetic_algorithm.hpp"
#include "../src/Core/memetic_algorithm.hpp"

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
RunSingleTest(Parameters* param)
{
    std::vector<Node> clientes = utils::ReadNodesFromFile(param);

    if (param->algorithm == "GA")
    {
        GA::GeneticAlgorithm ga;
        ga.setNodes(clientes);
        ga.setParameters(param);

        auto start = std::chrono::high_resolution_clock::now();
        ga.Run();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Dataset: " << getFileName(param->input_file) << std::endl
                  << "Best Fitness: " << ga.getPopulation().getBestFitness() << std::endl
                  << "Generations: " << ga.getPopulation().getGeneration() << std::endl
                  << "Duration (ms/s): " << duration.count() << "ms/" << duration.count() / 1000000.0 << "s" << std::endl;
    }
    else if (param->algorithm == "MA")
    {
        MA::MemeticAlgorithm ma;
        ma.setNodes(clientes);
        ma.setParameters(param);

        auto start = std::chrono::high_resolution_clock::now();
        ma.Run();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Dataset: " << getFileName(param->input_file) << std::endl
                  << "Best Fitness: " << ma.getPopulation().getBestFitness() << std::endl
                  << "Generations: " << ma.getPopulation().getGeneration() << std::endl
                  << "Duration (ms/s): " << duration.count() << "ms/" << duration.count() / 1000000.0 << "s" << std::endl;
    }
    else
    {
        std::cerr << "Invalid algorithm choice. Please specify either 'GA' or 'MA'." << std::endl;
    }
}

inline void 
RunTests(Parameters* param)
{
    SetupFolders(getFileName(param->input_file));

    std::string folder_name = getFileName(param->input_file);
    std::string name = "TEST_" + folder_name + ".txt";
    std::ofstream file("results/" + folder_name + "/" + name);

    if (!file.is_open())
    {
        std::cerr << "Failed to create file: " << name << std::endl;
        return;
    }

    std::vector<Node> clientes = utils::ReadNodesFromFile(param);

    for (int i = 0; i < 10; ++i)
    {
        if (param->algorithm == "GA")
        {
            GA::GeneticAlgorithm ga;
            ga.setNodes(clientes);
            ga.setParameters(param);

            auto start = std::chrono::high_resolution_clock::now();
            ga.Run();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            file << i + 1 << ": " << ga.getPopulation().getBestFitness() << " - " << duration.count()
                 << "/" << duration.count() / 1000000.0 << std::endl;
        }
        else if (param->algorithm == "MA")
        {
            MA::MemeticAlgorithm ma;
            ma.setNodes(clientes);
            ma.setParameters(param);

            auto start = std::chrono::high_resolution_clock::now();
            ma.Run();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            file << i + 1 << ": " << ma.getPopulation().getBestFitness() << " - " << duration.count()
                 << "/" << duration.count() / 1000000.0 << std::endl;
        }
        else
        {
            std::cerr << "Invalid algorithm choice. Please specify either 'GA' or 'MA'." << std::endl;
            break;
        }
    }
}

inline void
ProcessResults (const std::string& filename)
{
    std::string folder = getFileName(filename);
    std::string text_filename = "TEST_" + folder + ".txt";
    std::ifstream infile("results/" + folder + "/" + text_filename);
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

    for (const auto &result : results)
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

    for (const auto &res : results)
        variance_fitness += std::pow(res.fitness - avg_fitness, 2);

    double stddev_fitness = std::sqrt(variance_fitness / results.size() - 1);

    std::string out_filename = "RES_" + folder;
    std::ofstream outfile("results/" + folder + "/" + out_filename);

    if (!outfile.is_open())
    {
        std::cerr << "Failed to create file: " << out_filename << std::endl;
        return;
    }

    outfile << std::fixed << std::setprecision(6);
    outfile << "Média do fitness: " << avg_fitness << std::endl;
    outfile << "Desvio padrão do fitness: " << stddev_fitness << std::endl;
    outfile << "Melhor fitness: " << best_fitness << std::endl;
    outfile << "Média do tempo (ms): " << avg_time_ms << std::endl;
    outfile << "Média do tempo (s): " << avg_time_sec << std::endl;

    outfile.close();
}

#endif