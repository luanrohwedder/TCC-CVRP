#include <fstream>
#include <sstream>
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

int main()
{
    GA::GeneticAlgorithm ga;
    std::unordered_map<std::string, int> values;
    values.insert(std::make_pair("POP_SIZE", 10));
    values.insert(std::make_pair("PARENTS_SIZE", 4));
    values.insert(std::make_pair("GENERATIONS", 3000));
    std::vector<Node> clientes = ReadNodesFromFile("./dataset/E-n22-k4.txt", values);

    ga.setNodes(clientes);
    ga.setValues(values);
    ga.Run();

    return 0;
}
