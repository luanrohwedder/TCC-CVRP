#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include "node.hpp"

namespace GEN
{
    class Cluster
    {
    public:
        Cluster(double centroid_x, double centroid_y)
            : m_centroid_x(centroid_x), m_centroid_y(centroid_y) {}

        inline void addNode(const Node& node)
        {
            this->m_nodes.push_back(node);
            updateCentroid();
        }

        inline void clear()
        {
            this->m_nodes.clear();
        }

        inline void updateCentroid()
        {
            if (this->m_nodes.empty()) return;

            double sum_x = 0;
            double sum_y = 0;

            for (const auto& node : this->m_nodes)
            {
                sum_x += node.getX();
                sum_y += node.getY();
            }

            this->m_centroid_x = sum_x / this->m_nodes.size();
            this->m_centroid_y = sum_y / this->m_nodes.size();
        }

        void setCentroid(double centroid_x, double centroid_y) 
        { 
            this->m_centroid_x = centroid_x;
            this->m_centroid_y = centroid_y;
        }

        double getCentroidX() const { return this->m_centroid_x; }
        double getCentroidY() const { return this->m_centroid_y; }

        const std::vector<Node>& getNodes() const { return this->m_nodes; }

    private:
        std::vector<Node> m_nodes;
        double m_centroid_x = 0;
        double m_centroid_y = 0;
    };
}

#endif 