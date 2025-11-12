#include "quadtree.h"

void deleteTree(QuadNode* node) {
    if (!node){
        return;
    } 
    deleteTree(node->nw);
    deleteTree(node->ne);
    deleteTree(node->sw);
    deleteTree(node->se);
    delete node;
}
