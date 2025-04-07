#include "./Core/genetic_algorithm.hpp"
#include "run_utils.hpp"
#include "parameters.hpp"

#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char* argv[])
{
    Parameters* param = new Parameters(argc, argv);

#ifdef _OPENMP
    int numThreads = omp_get_num_procs() / 2;
    omp_set_num_threads(numThreads);
#endif

    if (param->run_single_test)
    {
        if (param->population == 0 || param->generation == 0) 
        {
            std::cerr << "Both -p <pop_size> and -g <generation_size> must be provided for a single test." << std::endl;
            return -1;
        }

        RunSingleTest(param);
    }
    else
    {
        param->population = 50;
        param->generation = 5000;

        RunTests(param);
        ProcessResults(param->input_file);
    }
    
    

    return 0;
}
