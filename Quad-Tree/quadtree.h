#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <list> 
#include <limits>
#include <cmath>
#include <algorithm> 

using namespace std;

struct QuadNode {
   
    double x_min, x_max, y_min, y_max;
    
   
    int capacity;
    
  
    list<vector<double>> points; 
    
    
    bool divided;

    QuadNode* nw;
    QuadNode* ne;
    QuadNode* sw;
    QuadNode* se;
    QuadNode(double x1, double x2, double y1, double y2, int cap);
};


QuadNode* insert(QuadNode* root, vector<double> point);


void deleteTree(QuadNode* root);


QuadNode* removeNode(QuadNode* root, vector<double>& point_rmv);


vector<double> findNearest(QuadNode* root, vector<double>& target_point, double& bestDist);

#endif 