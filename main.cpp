#include "structures.h"

void takeInput(int&, int&, string&, double&, double&, vector<vector<Edge>>&);
vector<vector<State>> paretoDijkstra(int, vector<vector<Edge>>&, string, double, double);
void printResult(vector<vector<State>>&, int);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int m, n, destination;
    string preference;
    double alpha = 1.0, beta = 1.0;

    vector<vector<Edge>> graph;

    takeInput(m, n, preference, alpha, beta, graph);
    
    cin >> destination;

    auto dist = paretoDijkstra(n, graph, preference, alpha, beta);

    printResult(dist, destination);

    return 0;
}