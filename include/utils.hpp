#ifndef UTILS_H
#define UTILS_H
#include <random>
#include <iostream>

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

}

#endif