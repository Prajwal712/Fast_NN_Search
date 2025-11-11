#include "kd_tree.h"


void printTree(KDNode* root, int depth) {
    if (!root) return;
    for (int i = 0; i < depth; i++) std::cout << "  ";
    cout << "(" << root->point[0] << ", " << root->point[1] << ")\n";
    printTree(root->left, depth + 1);
    printTree(root->right, depth + 1);
}

int main(){
    int n;
    cin >> n;
    KDNode * root = NULL;
    for(int i = 0 ; i < n ; i++){
        int k;
        cin >> k;
        switch (k){
            case 1 : {
                vector<double> point;
                double p ,q;
                cin >> p >> q;
                point.push_back(p);
                point.push_back(q);
                root = insert(root , point , 0);
                printTree(root , 0);
                break;
            } 
            case 2  : {
                vector<double> point;
                double p ,q;
                cin >> p >> q;
                point.push_back(p);
                point.push_back(q);
                root = removeNode(root , point , 0);
                printTree(root , 0);
                break;
            }
            case 3 : {
                vector<double> point;
                double p ,q;
                cin >> p >> q;
                point.push_back(p);
                point.push_back(q);
                KDNode* temp = findNearest(root , point);
                cout << temp->point[0] << " " << temp->point[1] << endl;
                break;
            } 
        }
    }

    return 0;
}