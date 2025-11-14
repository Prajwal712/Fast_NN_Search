#include "quadtree.h"
QuadNode::QuadNode(double x1, double x2, double y1, double y2, int cap)
    : x_min(x1), x_max(x2), y_min(y1), y_max(y2), capacity(cap),
      divided(false), nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr) {}

bool samePoint(vector<double>& a, vector<double>& b, double eps = 1e-9) {
    if (a.size() < 2 || b.size() < 2) return false;
    return (fabs(a[0] - b[0]) < eps && fabs(a[1] - b[1]) < eps);
}


double distSq(vector<double>& a, vector<double>& b) {
    if (a.size() < 2 || b.size() < 2) return numeric_limits<double>::max();
    double dx = a[0] - b[0];
    double dy = a[1] - b[1];
    return dx * dx + dy * dy;
}


bool contains(QuadNode* node, vector<double>& p) {
    if (p.size() < 2) return false;
    return (p[0] >= node->x_min && p[0] <= node->x_max &&
            p[1] >= node->y_min && p[1] <= node->y_max);
}


void subdivide(QuadNode* node) {
    double midX = (node->x_min + node->x_max) / 2;
    double midY = (node->y_min + node->y_max) / 2;

    node->nw = new QuadNode(node->x_min, midX, node->y_min, midY, node->capacity);
    node->ne = new QuadNode(midX, node->x_max, node->y_min, midY, node->capacity);
    node->sw = new QuadNode(node->x_min, midX, midY, node->y_max, node->capacity);
    node->se = new QuadNode(midX, node->x_max, midY, node->y_max, node->capacity);

    node->divided = true;
}



QuadNode* insert(QuadNode* node, vector<double> point) {
    if (point.size() < 2 || !contains(node, point)) {
        return node; 
    }

    if (node->points.size() < node->capacity) {
        node->points.push_back(point); 
        return node;
    }

    
    if (!node->divided) {
        subdivide(node);
    }

    
    double midX = (node->x_min + node->x_max) / 2;
    double midY = (node->y_min + node->y_max) / 2;

    if (point[0] <= midX && point[1] <= midY)
        node->nw = insert(node->nw, point);
    else if (point[0] > midX && point[1] <= midY)
        node->ne = insert(node->ne, point);
    else if (point[0] <= midX && point[1] > midY)
        node->sw = insert(node->sw, point);
    else
        node->se = insert(node->se, point);

    return node;
}


void deleteTree(QuadNode* node) {
    if (!node) {
        return;
    }
    deleteTree(node->nw);
    deleteTree(node->ne);
    deleteTree(node->sw);
    deleteTree(node->se);
    delete node;
}


QuadNode* removeNode(QuadNode* root, vector<double>& point_rmv) {
    if (!root || point_rmv.size() < 2) return nullptr;


    if (!contains(root, point_rmv)) {
        return root;
    }

    for (auto it = root->points.begin(); it != root->points.end(); ++it) {
        if (samePoint(*it, point_rmv)) {
            root->points.erase(it);
            return root; 
        }
    }

    if (root->divided) {
        root->nw = removeNode(root->nw, point_rmv);
        root->ne = removeNode(root->ne, point_rmv);
        root->sw = removeNode(root->sw, point_rmv);
        root->se = removeNode(root->se, point_rmv);
    }

    return root;
}

void nearestPoint(QuadNode* node, vector<double>& target, vector<double>& best, double& bestDist) {
    if (!node) return;

    double dx = max({0.0, node->x_min - target[0], target[0] - node->x_max});
    double dy = max({0.0, node->y_min - target[1], target[1] - node->y_max});
    

    double regionDist = dx * dx + dy * dy;


    if (regionDist > bestDist) {
        return; 
    }


    for (auto& p : node->points) {
        double d = distSq(p, target);
        if (d < bestDist) {
            bestDist = d;
            best = p;
        }
    }
    if (node->divided) {
        nearestPoint(node->nw, target, best, bestDist);
        nearestPoint(node->ne, target, best, bestDist);
        nearestPoint(node->sw, target, best, bestDist);
        nearestPoint(node->se, target, best, bestDist);
    }
}


vector<double> findNearest(QuadNode* root, vector<double>& target_point, double& bestDist) {
    vector<double> nearest_point;
    bestDist = numeric_limits<double>::max();
    
    if (root == nullptr || target_point.size() < 2) {
        return nearest_point;
    }

    
    nearestPoint(root, target_point, nearest_point, bestDist);
    
    return nearest_point;
}