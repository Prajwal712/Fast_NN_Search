#include "quadtree.h"

void printTree(QuadNode* node, int depth = 0) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << "Region: [(" << node->x_min << "," << node->y_min << ") → ("
         << node->x_max << "," << node->y_max << ")] Points: ";
    for (auto& p : node->points)
        cout << "(" << p.x << "," << p.y << ") ";
    cout << "\n";
    if (node->divided) {
        printTree(node->nw, depth + 1);
        printTree(node->ne, depth + 1);
        printTree(node->sw, depth + 1);
        printTree(node->se, depth + 1);
    }
}


int main() {
    // Create root Quadtree covering an 8x8 area (change as needed)
    QuadNode* root = new QuadNode(0, 8, 0, 8, 1);

    cout << "Operations:\n"
         << "1 x y  -> Insert point (x, y)\n"
         << "2 x y  -> Remove point (x, y)\n"
         << "3 x y  -> Find nearest neighbor to (x, y)\n"
         << "0      -> Exit\n";

    while (true) {
        int op;
        cout << "\nEnter operation: ";
        if (!(cin >> op)) break;
        if (op == 0) break;

        double x, y;
        if (op == 1 || op == 2 || op == 3)
            cin >> x >> y;

        switch (op) {
            case 1: {
                root = insert(root, Point(x, y));
                cout << "Inserted (" << x << ", " << y << ")\n";
                printTree(root, 0);
                break;
            }
            case 2: {
                root = removeNode(root, Point(x, y));
                cout << "Removed (" << x << ", " << y << ")\n";
                printTree(root, 0);
                break;
            }
            case 3: {
                Point nearest = findNearest(root, Point(x, y));
                cout << "Nearest neighbor to (" << x << ", " << y << ") → ("
                     << nearest.x << ", " << nearest.y << ")\n";
                break;
            }
            default:
                cout << "Invalid operation.\n";
        }
    }

    deleteTree(root);
    cout << "Quadtree cleared.\n";
    return 0;
}
