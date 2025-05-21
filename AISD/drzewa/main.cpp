#include "drzewa.h"

int main() {
    Node* bst = nullptr;
    Node* avl = nullptr;
    //Node* balancedBst = nullptr;
    //Node* min = 0;
    //int n = 256*pow(2,i);
    //vector<double> wyniki = {n};
    vector<int> data = {10, 20, 5, 15, 30, 25, 35, 1, 8, 12};
    //vector<int> data = generate_random_vector(n,1,10000,0);

    for (int val : data) bst = insert(bst, val);
    for (int val : data) avl = insertAVL(avl, val);

    //auto start = high_resolution_clock::now(); // START CZAS

    //min = findMin(avl);     // OPERACJA

    //auto stop = high_resolution_clock::now(); // STOP CZAS
    //auto duration = duration_cast<milliseconds>(stop - start);
    //double czas = duration.count();
    //wyniki.push_back(czas);
    //save_to_file(wyniki, "drzewowe_pomiary.txt");

    cout << "BST in-order: ";
    inOrder(bst); cout << endl;

    cout << "BST min path: ";
    printPathTo(bst, findMin(bst)->key); cout << endl;

    cout << "AVL pre-order: ";
    preOrder(avl); cout << endl;

    remove(avl, 25);

    cout << "AVL pre-order: ";
    preOrder(avl); cout << endl;

    deleteTree(bst);
    deleteTree(avl);

    return 0;
}