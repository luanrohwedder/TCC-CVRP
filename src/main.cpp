#include <fstream>
#include <sstream>
#include <chrono>
#include "./GeneticAlgorithm/genetic_algorithm.hpp"

/*
DIMENSION -> NÚMERO DE CLIENTES
CAPACITY -> CAPACIDADE DO VEÍCULO
POP_SIZE -> TAMANHO DA POPULAÇÃO
PARENTS_SIZE -> TAMANHO DA SELEÇÃO DE PARENTES
GENERATIONS -> GERAÇÕES QUE VÃO SER GERADOS
*/

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

std::vector<Node> ReadNodesFromFile(const std::string& filename, std::unordered_map<std::string, int>& values)
{
    std::ifstream infile(filename);
    std::string line;
    std::vector<Node> nodes;

    if (!infile)
    {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return nodes;
    }

    bool reading_coords = false;
    bool reading_demands = false;
    std::vector<double> demands;

    while (std::getline(infile, line))
    {
        line = trim(line);

        std::istringstream iss(line);

        if (line.find("DIMENSION") != std::string::npos || line.find("CAPACITY") != std::string::npos)
        {
            int value;
            char colon;
            std::string key;
            
            iss >> key >> colon >> value;
            values.insert(std::make_pair(key, value));
        }

        if (line == "NODE_COORD_SECTION")
        {
            reading_coords = true;
            continue;
        }

        if (line == "DEMAND_SECTION")
        {
            reading_coords = false;
            reading_demands = true;
            continue;
        }

        if (line == "DEPOT_SECTION" || line == "EOF")
            break;

        if (reading_coords)
        {
            int id;
            double x, y;

            iss >> id >> x >> y;
            nodes.push_back(Node(id, x, y, 0.0));
        }

        if (reading_demands)
        {
            int id;
            double demand;

            iss >> id >> demand;
            nodes[id - 1].setDemand(demand);
        }
    }

    infile.close();
    return nodes;
}

void ParametersVerification(const std::unordered_map<std::string, int> &map)
{
    if (map.at("POP_SIZE") <= 3 
    || map.at("PARENTS_SIZE") <= 1 
    || map.at("GENERATIONS") < 0 
    || map.at("DIMENSION") <= 1 
    || map.at("MUTATION_RATE") <= 0 
    || map.at("MUTATION_RATE") > 100 )
        throw std::invalid_argument("Invalid parameters for initialization.");
}

int main(int argc, char* argv[])
{
    if (argc != 6)
    {
        std::cerr << "Usage: " << argv[0] << " <file> <population_size> <parents_size> <generations> <mutation_rate>" << std::endl;
        return -1;
    }

    std::unordered_map<std::string, int> values;
    values.insert(std::make_pair("POP_SIZE", std::stoi(argv[2])));
    values.insert(std::make_pair("PARENTS_SIZE", std::stoi(argv[3])));
    values.insert(std::make_pair("GENERATIONS", std::stoi(argv[4])));
    values.insert(std::make_pair("MUTATION_RATE", std::stoi(argv[5])));
    std::vector<Node> clientes = ReadNodesFromFile(argv[1], values);

    ParametersVerification(values);

    GA::GeneticAlgorithm ga;
    ga.setNodes(clientes);
    ga.setValues(values);

    auto start = std::chrono::high_resolution_clock::now();
    ga.Run();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    std::cout << "Best fitness: " << ga.getBestFitness() << std::endl << std::endl;

    std::cout << "Duration: " << std::endl 
              << duration.count() << " microseconds" 
              << std::endl << duration.count() / 1000000.0 
              << " seconds" << std::endl;

    return 0;
}
