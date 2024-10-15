#include "./GeneticAlgorithm/genetic_algorithm.hpp"
#include "run_utils.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
        return -1;
    }

    std::vector<int> pop_sizes = {10, 50, 100, 500};
    std::vector<int> generations_sizes = {10, 50, 100, 500};

    RunTests(argv[1], pop_sizes, generations_sizes);
    
    ProcessResults(argv[1], pop_sizes, generations_sizes);

    return 0;
}
