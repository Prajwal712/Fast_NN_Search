#include "quadtree.h"

double distSq(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void nearestNeighborSearch(QuadNode* node, Point target, Point& best, double& bestDist) {
    if (!node) return;


    double dx = max({0.0, node->x_min - target.x, target.x - node->x_max});
    double dy = max({0.0, node->y_min - target.y, target.y - node->y_max});
    double regionDist = dx * dx + dy * dy;

    if (regionDist > bestDist) return; 


    for (auto& p : node->points) {
        double d = distSq(p, target);
        if (d < bestDist) {
            bestDist = d;
            best = p;
        }
    }


    if (node->divided) {
        nearestNeighborSearch(node->nw, target, best, bestDist);
        nearestNeighborSearch(node->ne, target, best, bestDist);
        nearestNeighborSearch(node->sw, target, best, bestDist);
        nearestNeighborSearch(node->se, target, best, bestDist);
    }
}

Point findNearest(QuadNode* root, Point target) {
    Point best;
    double bestDist = numeric_limits<double>::max();
    nearestNeighborSearch(root, target, best, bestDist);
    return best;
}
