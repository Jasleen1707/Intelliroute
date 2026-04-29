#include "structures.h"

void takeInput(int &m, int &n, string &preference, double &alpha, double &beta, vector<vector<Edge>> &graph) {
cin >> m >> n;
cin >> preference;

if (preference == "weighted") {
cin >> alpha >> beta;
}

graph.resize(n + 1);

for (int i = 0; i < m; i++) {
int u, v;
long long cost, time;
cin >> u >> v >> cost >> time;
graph[u].push_back({v, cost, time});
}
}