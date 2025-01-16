#ifndef NEAREST_NEIGHBOR_H
#define NEAREST_NEIGHBOR_H

#include "kmeans.hpp"
#include "../Core/chromosome.hpp"

namespace GEN
{
    class NearestNeighbor
    {
    public:
        NearestNeighbor(const std::vector<Cluster>& clusters)
            : m_clusters(clusters) {}

        inline void run()
        {
            for (auto& cluster : this->m_clusters)
            {
                std::vector<int> dna = createPartialDna(cluster);
                m_dna.insert(this->m_dna.end(), dna.begin(), dna.end());
            }
        }
        
        const std::vector<int>& getDna() const { return this->m_dna; }

    private:
        std::vector<int> m_dna;
        std::vector<Cluster> m_clusters;

        inline std::vector<int> createPartialDna(const Cluster &cluster)
        {
            std::unordered_set<int> unvisited_nodes;
            std::vector<int> dna;

            for (auto &node : cluster.getNodes())
            {
                if (node.getId() != 1)
                    unvisited_nodes.insert(node.getId() - 1);
            }

            if (unvisited_nodes.empty())
                throw std::runtime_error("Error: Cluster is empty. No nodes to visit.");

            int current_node_id = *std::next(unvisited_nodes.begin(), utils::randInteger(0, unvisited_nodes.size() - 1));
            unvisited_nodes.erase(current_node_id);
            dna.push_back(current_node_id);

            while (!unvisited_nodes.empty())
            {
                int nearest_node_id = findNearestNeighbor(current_node_id, unvisited_nodes, cluster.getNodes());
                dna.push_back(nearest_node_id);
                unvisited_nodes.erase(nearest_node_id);
                current_node_id = nearest_node_id;
            }

            return dna;
        }

        inline int
        findNearestNeighbor(int current_node_id, const std::unordered_set<int>& unvisited_nodes, const std::vector<Node>& nodes)
        {
            if (unvisited_nodes.empty())
                throw std::runtime_error("Error: unvisited_nodes is empty.");

            int nearest_node_id = -1;
            double min_distance = std::numeric_limits<double>::max();

            const Node& current_node = getNodeById(current_node_id, nodes);

            for (int node_id : unvisited_nodes)
            {
                const Node& target_node = getNodeById(node_id, nodes);

                double dist = utils::EuclidianDistance(
                    current_node.getX(), target_node.getX(),
                    current_node.getY(), target_node.getY());

                if (dist < min_distance)
                {
                    min_distance = dist;
                    nearest_node_id = node_id;
                }
            }

            if (nearest_node_id == -1)
                throw std::runtime_error("Error: No valid nearest neighbor found.");

            return nearest_node_id;
        }

        inline const Node& getNodeById(int id, const std::vector<Node>& nodes)
        {
            for (const auto& node : nodes)
                if (node.getId() == id + 1)
                    return node;
            throw std::runtime_error("Error: Node ID not found in nodes.");
        }
    };
}

#endif