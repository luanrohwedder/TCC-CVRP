#include "./GeneticAlgorithm/genetic_algorithm.hpp"
#include "run_utils.hpp"
#include <getopt.h>

int main(int argc, char* argv[])
{
    int pop_size = 0;
    int gen_size = 0;
    bool run_single_test = false;
    std::string alg_choice = "GA";
    std::string ls_choice = "S";

    int opt;
    while ((opt = getopt(argc, argv, "p:g:a:l:")) != -1)
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
        case 'a':
            alg_choice = optarg;
            break;
        case 'l':
            ls_choice = optarg;
            break;
        default:
        std::cerr << "Usage: " << argv[0] << " <file> [-p <pop_size>] [-g <generation_size>] [-a 'GA' or 'MA' | default 'GA'] [-l 'H' or 'S' | default 'S']" << std::endl;
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

        RunSingleTest(file, pop_size, gen_size, alg_choice, ls_choice);
    }
    else
    {
        std::vector<int> pop_sizes = {50};
        std::vector<int> generations_sizes = {5000};

        RunTests(file, pop_sizes, generations_sizes, alg_choice, ls_choice);
        ProcessResults(file, pop_sizes, generations_sizes);
    }
    
    

    return 0;
}
