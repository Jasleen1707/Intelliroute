#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <bits/stdc++.h>
using namespace std;

struct State {
long long cost, time;
};

struct Edge {
int v;
long long cost, time;
};

struct NodeState {
int node;
long long cost, time;
};

#endif