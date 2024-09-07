#ifndef NODE_H
#define NODE_H

/**
 * @brief Represents a node in the VRP (Vehicle Routing Problem).
 *
 * This class contains information about a node, including their location and any other relevant data.
 */
class Node
{
public:
    Node(int id, double x, double y, double demand)
        : m_id(id), m_x(x), m_y(y), m_demand(demand) {}

    // GETTERS AND SETTER
    int getId() const { return m_id; }
    void setId(int id) { m_id = id; }

    double getX() const { return m_x; }
    void setX(double x) { m_x = x; }

    double getY() const { return m_y; }
    void setY(double y) { m_y = y; }

    double getDemand() const { return m_demand; }
    void setDemand(double demand) { m_demand = demand; }

private:
    int m_id;
    double m_x;
    double m_y;
    double m_demand;
};

#endif