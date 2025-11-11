#ifndef KD_TREE_H
#define KD_TREE_H

#include <bits/stdc++.h>
using namespace std;

struct KDNode{
    vector<double> point;
    KDNode * left;
    KDNode * right;

    KDNode(const vector<double>&pt) : point(pt) , left(nullptr) , right(nullptr){}
};

KDNode* insert(KDNode* root , vector<double> point , int depth = 0);

void deleteTree(KDNode* root);

KDNode* removeNode(KDNode* root , vector<double>&point_rmv , int depth = 0);

KDNode* findNearest(KDNode* root , vector<double> &target_point);

//testing
void printTree(KDNode* root, int depth = 0);

#endif
