#include "drzewa.h"
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    Node* bst = nullptr;
    Node* avl = nullptr;

    vector<int> data = {10, 20, 5, 15, 30, 25, 35, 1, 8, 12};

    for (int val : data) bst = insert(bst, val);
    for (int val : data) avl = insertAVL(avl, val);

    cout << "BST in-order: ";
    inOrder(bst); cout << endl;

    cout << "BST min path: ";
    printPathTo(bst, findMin(bst)->key); cout << endl;

    cout << "AVL in-order: ";
    inOrder(avl); cout << endl;

    deleteTree(bst);
    deleteTree(avl);

    return 0;
}