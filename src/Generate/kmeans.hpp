#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include <limits>
#include <unordered_set>
#include "node.hpp"
#include "utils.hpp"
#include "cluster.hpp"

namespace GEN 
{
    class KMeans
    {
    public:
        KMeans(int k, const std::vector<Node>& nodes, int max_x, int max_y)
            : k(k), m_nodes(nodes), m_max_x(max_x), m_max_y(max_y) {}

        inline void run(int max_iters = 100)
        {
            initializeClusters();

            for (int iter = 0; iter < max_iters; ++iter)
            {
                createClusters();

                for (auto& cluster : this->m_clusters)
                {
                    if (cluster.getNodes().empty())
                    {
                        cluster.clear();
                        double rand_x = utils::randDouble(0, this->m_max_x);
                        double rand_y = utils::randDouble(0, this->m_max_y);
                        cluster.setCentroid(rand_x, rand_y);
                    }
                    else
                    {
                        cluster.updateCentroid();
                    }
                }

                if (isConverged())
                    break;
            }
        }

        const std::vector<Cluster>& getClusters() const { return this->m_clusters; }

    private:
        int k;
        std::vector<Node> m_nodes;
        std::vector<Cluster> m_clusters;
        int m_max_x = 0;
        int m_max_y = 0;

        inline void initializeClusters()
        {
           this->m_clusters.clear();

            for (int i = 0; i < k; ++i)
            {
                double rand_x = utils::randDouble(0, this->m_max_x);
                double rand_y = utils::randDouble(0, this->m_max_y);
                Cluster cluster(rand_x, rand_y);
                this->m_clusters.push_back(cluster);
            }
        }

        inline void createClusters()
        {
            for (auto& cluster: this->m_clusters)
                cluster.clear();

            for (const auto& node : this->m_nodes)
            {
                if (node.getId() == 1) continue;

                int best_cluster_index = 0;
                double min_distance = std::numeric_limits<double>::infinity();

                for (int i = 0; i < k; ++i)
                {
                    double dist = utils::EuclidianDistance(
                        node.getX(), this->m_clusters[i].getCentroidX(),
                        node.getY(), this->m_clusters[i].getCentroidY());

                    if (dist < min_distance)
                    {
                        min_distance = dist;
                        best_cluster_index = i;
                    }
                }

                this->m_clusters[best_cluster_index].addNode(node);
            }
        }

        inline bool isConverged()
        {
            for (int i = 1; i < k; ++i)
            {
                if (this->m_clusters[i].getCentroidX() != this->m_clusters[i - 1].getCentroidX() ||
                    this->m_clusters[i].getCentroidY() != this->m_clusters[i - 1].getCentroidY()) {
                    return false;
                }
            }
            return true;
        }
    };
}

#endif