#ifndef UTILS_H
#define UTILS_H
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include "node.hpp"
#include "../src/parameters.hpp"

namespace utils {

/**
 * @brief Generate a random number between min and max (inclusive)
 * 
 * This function generates a random integer within the specified range, including both
 * the minimun and maximun values.
 * 
 * @param min The minimun value (inclusive) of the range.
 * @param max The maximun value (inclusive) of the range.
 * @return A int between min and max (inclusive)
 */
inline int randInteger(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);

    return distr(gen);
}

/**
 * @brief Generate a random number between min and max (inclusive)
 * 
 * This function generates a random double within the specified range, including both
 * the minimun and maximun values.
 * 
 * @param min The minimun value (inclusive) of the range.
 * @param max The maximun value (inclusive) of the range.
 * @return A double between min and max (inclusive)
 */
inline double randDouble(double min, double max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(min, max);

    return distr(gen);
}

/**
 * @brief Computes the Euclidean distance between two points.
 * 
 * This function calculates the distance between two points in a 2D space given their coordinates.
 * 
 * @param x1 X-coordinate of the first point.
 * @param x2 X-coordinate of the second point.
 * @param y1 Y-coordinate of the first point.
 * @param y2 Y-coordinate of the second point.
 * @return The Euclidean distance between the two points.
 */
inline double EuclidianDistance(double x1, double x2, double y1, double y2)
{
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

inline double Distance(const Node& a, const Node& b)
{
    return utils::EuclidianDistance(
        a.getX(), b.getX(),
        a.getY(), b.getY());
}

/**
 * @brief Remove leading and trailing whitespace characters from a string.
 * 
 * This function removes any leading (beginning) and trailing (end) spaces, tabs, newlines, 
 * and carriage return characters from the input string.
 * 
 * @param str The input string to be trimmed.
 * 
 * @return A new string with leading and trailing whitespace removed.
 */
inline std::string Trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

/**
 * @brief Reads nodes and associated data from a file.
 * 
 * This function reads node coordinates and demands from a file in a specific format.
 * It populates a vector of Node objects, each containing an ID, coordinates, and demand.
 * 
 * @param filename The name of the file to read.
 * @param values A reference to an unordered_map that will store additional parameters 
 * (e.g., DIMENSION, CAPACITY) found in the file.
 * 
 * @return A vector of Node objects populated with data from the file.
 */
inline std::vector<Node> ReadNodesFromFile(Parameters* param)
{
    std::ifstream infile(param->input_file);
    std::string line;
    std::vector<Node> nodes;

    if (!infile)
    {
        std::cerr << "Error: Cannot open file " << param->input_file << std::endl;
        return nodes;
    }

    int max_x = 0;
    int max_y = 0;
    bool reading_coords = false;
    bool reading_demands = false;
    std::vector<double> demands;

    while (std::getline(infile, line))
    {
        line = Trim(line);

        std::istringstream iss(line);

        if (line.find("DIMENSION") != std::string::npos || line.find("CAPACITY") != std::string::npos)
        {
            int value;
            char colon;
            std::string key;
            
            iss >> key >> colon >> value;
            
            if (key == "DIMENSION")
                param->dimension = value;
            if (key == "CAPACITY")
                param->capacity = value;
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

            if (x > max_x) max_x = x;
            if (y > max_y) max_y = y;
        }

        if (reading_demands)
        {
            int id;
            double demand;

            iss >> id >> demand;
            nodes[id - 1].setDemand(demand);
        }
    }

    param->max_x = max_x;
    param->max_y = max_y;

    infile.close();
    return nodes;
}

}

#endif