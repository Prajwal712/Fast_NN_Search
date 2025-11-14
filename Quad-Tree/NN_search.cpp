#include "quadtree.h"
#include <limits>

double distSq(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void nearestNeighborSearch(QuadNode* node, Point target, Point& best, double& bestDist) {
    if (!node) return;

    double dx = 0.0;
    if (target.x < node->x_min) dx = node->x_min - target.x;
    else if (target.x > node->x_max) dx = target.x - node->x_max;
    
    double dy = 0.0;
    if (target.y < node->y_min) dy = node->y_min - target.y;
    else if (target.y > node->y_max) dy = target.y - node->y_max;
    
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
        double midX = (node->x_min + node->x_max) * 0.5;
        double midY = (node->y_min + node->y_max) * 0.5;

        bool left = target.x <= midX;
        bool top = target.y <= midY;

        if (top && left) {
            nearestNeighborSearch(node->nw, target, best, bestDist);
            nearestNeighborSearch(node->ne, target, best, bestDist);
            nearestNeighborSearch(node->sw, target, best, bestDist);
            nearestNeighborSearch(node->se, target, best, bestDist);
        } else if (top && !left) {
            nearestNeighborSearch(node->ne, target, best, bestDist);
            nearestNeighborSearch(node->nw, target, best, bestDist);
            nearestNeighborSearch(node->se, target, best, bestDist);
            nearestNeighborSearch(node->sw, target, best, bestDist);
        } else if (!top && left) {
            nearestNeighborSearch(node->sw, target, best, bestDist);
            nearestNeighborSearch(node->nw, target, best, bestDist);
            nearestNeighborSearch(node->se, target, best, bestDist);
            nearestNeighborSearch(node->ne, target, best, bestDist);
        } else {
            nearestNeighborSearch(node->se, target, best, bestDist);
            nearestNeighborSearch(node->ne, target, best, bestDist);
            nearestNeighborSearch(node->sw, target, best, bestDist);
            nearestNeighborSearch(node->nw, target, best, bestDist);
        }
    }
}

Point findNearest(QuadNode* root, Point target) {
    Point best;
    double bestDist = numeric_limits<double>::max();
    nearestNeighborSearch(root, target, best, bestDist);
    return best;
}
