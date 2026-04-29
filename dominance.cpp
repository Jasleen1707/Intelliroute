#include "structures.h"

bool dominates(State a, State b) {
    return (a.cost <= b.cost && a.time <= b.time) &&
           (a.cost < b.cost || a.time < b.time);
}