#include "quadtree.h"

int main() {
    QuadNode* root = new QuadNode(0, 8, 0, 8, 1);

    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int k;
        cin >> k;
        switch (k) {
            case 1: {
                double x, y;
                cin >> x >> y;
                root = insert(root, Point(x, y));
                printTree(root, 0);
                break;
            }
            case 2: {
                double x, y;
                cin >> x >> y;
                Point* res = search(root, Point(x, y));
                if (res) cout << "Found (" << res->x << "," << res->y << ")\n";
                else cout << "Not found\n";
                break;
            }
        }
    }

    deleteTree(root);
    return 0;
}
