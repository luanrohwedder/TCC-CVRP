#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <string>
#include <getopt.h>
#include <iostream>

#define POPULATION 50
#define GENERATION 5000

class Parameters {
public:
    Parameters() = default;

    Parameters(int argc, char *argv[])
    {
        int opt;
        while ((opt = getopt(argc, argv, "p:g:a:l:o")) != -1)
        {
            switch (opt)
            {
            case 'p':
                population = std::stoi(optarg);
                parent_size = population/3;
                run_single_test = true;
                break;
            case 'g':
                generation = std::stoi(optarg);
                run_single_test = true;
                break;
            case 'a':
                algorithm = optarg;
                break;
            case 'l':
                ls_Choice = optarg;
                break;
            default:
                std::cerr << "Usage: " << argv[0]
                          << " <file> [-p pop_size] [-g generation_size] "
                             "[-a GA|MA] [-l S|H] [-o (enable OpenMP)]"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }

        if (optind >= argc)
        {
            std::cerr << "Expected argument after options (file path)" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        input_file = argv[optind];
    }

    int parent_size = POPULATION/3;
    int population = POPULATION;
    int generation = GENERATION;
    int capacity = 0;
    int dimension = 0;
    int max_x = 0;
    int max_y = 0;

    bool run_single_test = false;

    std::string algorithm = "GA";
    std::string ls_Choice = "S";
    std::string input_file;
};

#endif
