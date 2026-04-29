#include "structures.h"

// -------------------- BALANCED ROUTE --------------------
State getBalancedRoute(vector<State> &routes) {

    double avgCost = 0, avgTime = 0;

    for (auto &r : routes) {
        avgCost += r.cost;
        avgTime += r.time;
    }

    avgCost /= routes.size();
    avgTime /= routes.size();

    State best = routes[0];
    double bestDist = 1e18;

    for (auto &r : routes) {
        double dist = sqrt(
            (r.cost - avgCost) * (r.cost - avgCost) +
            (r.time - avgTime) * (r.time - avgTime)
        );

        if (dist < bestDist) {
            bestDist = dist;
            best = r;
        }
    }

    return best;
}

// -------------------- OUTPUT --------------------
void printResult(vector<vector<State>> &dist, int n) {

    cout << "Pareto Optimal Routes:\n";

    for (auto &s : dist[n]) {
        cout << "Cost:" << s.cost
             << ", Time:" << s.time << "\n";
    }

    if (dist[n].empty()) return;

    vector<State> &routes = dist[n];

    // 🔹 Cheapest
    State cheapest = routes[0];
    for (auto &r : routes) {
        if (r.cost < cheapest.cost)
            cheapest = r;
    }

    // 🔹 Fastest
    State fastest = routes[0];
    for (auto &r : routes) {
        if (r.time < fastest.time)
            fastest = r;
    }

    // 🔹 Balanced
    State balanced = getBalancedRoute(routes);

    cout << "\nRecommended Routes:\n";

    cout << "Cheapest Route -> Cost:" << cheapest.cost
         << ", Time:" << cheapest.time << "\n";

    cout << "Fastest Route -> Cost:" << fastest.cost
         << ", Time:" << fastest.time << "\n";

    cout << "Balanced Route -> Cost:" << balanced.cost
         << ", Time:" << balanced.time << "\n";
}