#include "memetic_algorithm.hpp"

namespace MA
{
    void MemeticAlgorithm::ApplyLocalSeach(std::vector<GA::Chromosome>& children)
    {
        double lsProb = 0.3;

#ifdef _OPENMP
        #pragma omp parallel for
#endif
        for (size_t i = 0; i < children.size(); ++i)
        {
            double randProb = utils::randDouble(0, 1);

            if (randProb < lsProb)
                SimulatedAnnealing(children[i]);
        }
    }

    void MemeticAlgorithm::SimulatedAnnealing(GA::Chromosome& child)
    {
        double temperature = 10;
        double cooling = 0.95;
        double absolute_min_temp = 0.1;

        GA::Chromosome currentSolution = child;
        GA::Chromosome bestSolution = child;

        currentSolution.CalculateFitness(this->getNodes(), this->getParameters()->capacity);
        bestSolution.CalculateFitness(this->getNodes(), this->getParameters()->capacity);

        while (temperature > absolute_min_temp)
        {
            GA::Chromosome neighbor = GenerateNeighborSwapStar(currentSolution);
            neighbor.CalculateFitness(this->getNodes(), this->getParameters()->capacity);

            double currentFitness = currentSolution.getFitness();
            double neighborFitness = neighbor.getFitness();

            if (neighborFitness < currentFitness)
            {
                currentSolution = neighbor;
                if (neighborFitness < bestSolution.getFitness())
                    bestSolution = neighbor;
            }
            else
            {
                double deltaFitness = neighborFitness - currentFitness;
                double acceptProbability = exp(-deltaFitness / temperature);
                double randVal = utils::randDouble(0, 1);

                if (randVal < acceptProbability)
                    currentSolution = neighbor;
            }

            temperature *= cooling;
        }
        child = bestSolution;
    }

    GA::Chromosome MemeticAlgorithm::GenerateNeighborSwapStar(const GA::Chromosome& solution)
    {
        GA::Chromosome neighbor = solution;
        std::vector<std::vector<int>> current_routes = neighbor.getRoutes();

        if (current_routes.size() < 2) {
            return neighbor;
        }

        const auto& nodesRef = this->getNodes();
        int capacity = this->getParameters()->capacity;
        const double epsilon = 1e-9;

        double best_deltaCost = epsilon;
        bool found_improvement = false;
        size_t best_r1_idx = -1, best_r2_idx = -1;
        size_t best_i_pos = -1, best_j_pos = -1;

        for (size_t r1_idx = 0; r1_idx < current_routes.size(); ++r1_idx)
        {
            int original_load1 = 0;
            if (!current_routes[r1_idx].empty()) {
                 for (int nodeIdx : current_routes[r1_idx]) original_load1 += nodesRef[nodeIdx].getDemand();
            } else {
                continue;
            }


            for (size_t r2_idx = r1_idx + 1; r2_idx < current_routes.size(); ++r2_idx)
            {
                const auto& route1_ref = current_routes[r1_idx];
                const auto& route2_ref = current_routes[r2_idx];

                int original_load2 = 0;
                if (!route2_ref.empty()) {
                    for (int nodeIdx : route2_ref) original_load2 += nodesRef[nodeIdx].getDemand();
                } else {
                     continue;
                }


                for (size_t i_pos = 0; i_pos < route1_ref.size(); ++i_pos)
                {
                    for (size_t j_pos = 0; j_pos < route2_ref.size(); ++j_pos)
                    {
                        int node_u = route1_ref[i_pos];
                        int node_v = route2_ref[j_pos];

                        int demand_u = nodesRef[node_u].getDemand();
                        int demand_v = nodesRef[node_v].getDemand();

                        int newLoad1 = original_load1 - demand_u + demand_v;
                        int newLoad2 = original_load2 - demand_v + demand_u;

                        if (newLoad1 > capacity || newLoad2 > capacity) {
                            continue;
                        }

                        int prev_u = (i_pos == 0) ? 0 : route1_ref[i_pos - 1];
                        int next_u = (i_pos == route1_ref.size() - 1) ? 0 : route1_ref[i_pos + 1];
                        int prev_v = (j_pos == 0) ? 0 : route2_ref[j_pos - 1];
                        int next_v = (j_pos == route2_ref.size() - 1) ? 0 : route2_ref[j_pos + 1];

                        double removed_cost = utils::Distance(nodesRef[prev_u], nodesRef[node_u]) +
                                              utils::Distance(nodesRef[node_u], nodesRef[next_u]) +
                                              utils::Distance(nodesRef[prev_v], nodesRef[node_v]) +
                                              utils::Distance(nodesRef[node_v], nodesRef[next_v]);

                        double added_cost = utils::Distance(nodesRef[prev_u], nodesRef[node_v]) + 
                                            utils::Distance(nodesRef[node_v], nodesRef[next_u]) +
                                            utils::Distance(nodesRef[prev_v], nodesRef[node_u]) + 
                                            utils::Distance(nodesRef[node_u], nodesRef[next_v]);

                        double deltaCost = added_cost - removed_cost;

                        if (deltaCost < best_deltaCost)
                        {
                            best_deltaCost = deltaCost;
                            best_r1_idx = r1_idx;
                            best_r2_idx = r2_idx;
                            best_i_pos = i_pos;
                            best_j_pos = j_pos;
                            found_improvement = true;
                        }
                    }
                }
            }
        }

        if (found_improvement)
        {
            std::vector<std::vector<int>> modifiedRoutes = current_routes;

            int node_u_to_move = modifiedRoutes[best_r1_idx][best_i_pos];
            int node_v_to_move = modifiedRoutes[best_r2_idx][best_j_pos];

            modifiedRoutes[best_r1_idx][best_i_pos] = node_v_to_move;
            modifiedRoutes[best_r2_idx][best_j_pos] = node_u_to_move;

            neighbor.setRoutes(modifiedRoutes);
        }

        return neighbor;
    }
}