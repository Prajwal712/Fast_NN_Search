#include "quadtree.h"

bool contains(QuadNode* node, Point p) {
    return (p.x >= node->x_min && p.x <= node->x_max &&
            p.y >= node->y_min && p.y <= node->y_max);
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

QuadNode* insert(QuadNode* node, Point p) {
    if (!contains(node, p))
        return node;

    if (node->points.size() < node->capacity) {
        node->points.push_back(p);
        return node;
    }

    if (!node->divided)
        subdivide(node);

    double midX = (node->x_min + node->x_max) / 2;
    double midY = (node->y_min + node->y_max) / 2;

    if (p.x <= midX && p.y <= midY)
        node->nw = insert(node->nw, p);
    else if (p.x > midX && p.y <= midY)
        node->ne = insert(node->ne, p);
    else if (p.x <= midX && p.y > midY)
        node->sw = insert(node->sw, p);
    else
        node->se = insert(node->se, p);

    return node;
}
