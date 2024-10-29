#include "./GeneticAlgorithm/genetic_algorithm.hpp"
#include "run_utils.hpp"
#include <getopt.h>

int main(int argc, char* argv[])
{
    int pop_size = 0;
    int gen_size = 0;
    bool run_single_test = false;

    int opt;
    while ((opt = getopt(argc, argv, "p:g:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            pop_size = std::stoi(optarg);
            run_single_test = true;
            break;
        case 'g':
            gen_size = std::stoi(optarg);
            run_single_test = true;
            break;
        default:
        std::cerr << "Usage: " << argv[0] << " <file> [-p <pop_size>] [-g <generation_size>]" << std::endl;
            break;
        }
    }

    if (optind >= argc)
    {
        std::cerr << "Expected argument after options" << std::endl;
        return -1;
    }

    std::string file = argv[optind];
    
    if (run_single_test)
    {
        if (pop_size == 0 || gen_size == 0) 
        {
            std::cerr << "Both -p <pop_size> and -g <generation_size> must be provided for a single test." << std::endl;
            return -1;
        }

        RunSingleTest(file, pop_size, gen_size);
    }
    else
    {
        std::vector<int> pop_sizes = {50};
        std::vector<int> generations_sizes = {5000};

        RunTests(file, pop_sizes, generations_sizes);
        ProcessResults(file, pop_sizes, generations_sizes);
    }
    
    

    return 0;
}
