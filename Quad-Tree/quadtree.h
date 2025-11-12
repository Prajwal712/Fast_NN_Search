#ifndef QUADTREE_H
#define QUADTREE_H

#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
    Point(double _x = 0, double _y = 0) : x(_x), y(_y) {}
};

struct QuadNode {

    double x_min, x_max, y_min, y_max;

    vector<Point> points;

    QuadNode *nw, *ne, *sw, *se;

    int capacity;
    bool divided;

    QuadNode(double xmin, double xmax, double ymin, double ymax, int cap = 1)
        : x_min(xmin), x_max(xmax), y_min(ymin), y_max(ymax),
          capacity(cap), divided(false), nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr) {}
};

// Functions
QuadNode* insert(QuadNode* node, Point p);
bool contains(QuadNode* node, Point p);
void subdivide(QuadNode* node);
Point* search(QuadNode* node, Point target);
void printTree(QuadNode* node, int depth = 0);
void deleteTree(QuadNode* node);
QuadNode* removeNode(QuadNode* node, Point target);
Point findNearest(QuadNode* root, Point target);


#endif
