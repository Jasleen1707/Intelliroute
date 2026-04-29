#include "structures.h"

extern bool dominates(State, State);

struct Compare {
    string preference;
    double alpha, beta;

    Compare(string p, double a, double b)
        : preference(p), alpha(a), beta(b) {}

    bool operator()(const NodeState &a, const NodeState &b) {
        if (preference == "cost") return a.cost > b.cost;
        if (preference == "time") return a.time > b.time;
        return (alpha*a.cost + beta*a.time) >
               (alpha*b.cost + beta*b.time);
    }
};

vector<vector<State> > paretoDijkstra(int n, vector<vector<Edge> > &graph, string preference, double alpha, double beta) {

    vector<vector<State> > dist(n + 1);
    dist[1].push_back({0, 0});

    priority_queue<NodeState, vector<NodeState>, Compare> pq(
        Compare(preference, alpha, beta));

    pq.push({1, 0, 0});

    while (!pq.empty()) {
        auto curr = pq.top(); pq.pop();

        State newState = {curr.cost, curr.time};
        int u = curr.node;

        bool dominated_flag = false;
        for (auto &s : dist[u])
            if (dominates(s, newState)) dominated_flag = true;

        if (dominated_flag) continue;

        vector<State> temp;
        for (auto &s : dist[u])
            if (!dominates(newState, s)) temp.push_back(s);

        temp.push_back(newState);
        dist[u] = temp;

        for (auto &e : graph[u]) {
            pq.push({e.v,
                     curr.cost + e.cost,
                     curr.time + e.time});
        }
    }

    return dist;
}