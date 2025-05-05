#include "drzewa.h"

int main() {
    Node* bst = nullptr;
    Node* avl = nullptr;
    for (int i = 0; i < 15; i++) {
        int n = 256*pow(2,i);
        vector<double> wyniki = {n};
        //vector<int> data = {10, 20, 5, 15, 30, 25, 35, 1, 8, 12};
        vector<int> data = generate_random_vector(n,1,10000,2);

        auto start = high_resolution_clock::now();
        
        for (int val : data) bst = insert(bst, val);    // OPERACJA

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        double czas = duration.count();
        wyniki.push_back(czas);
        save_to_file(wyniki, "drzewowe_pomiary.txt");
        //for (int val : data) avl = insertAVL(avl, val);

        //cout << "BST in-order: ";
        //inOrder(bst); cout << endl;

        //remove(avl, 8);

        //cout << "BST min path: ";
        //printPathTo(bst, findMin(bst)->key); cout << endl;

        //cout << "AVL in-order: ";
        //inOrder(avl); cout << endl;

        deleteTree(bst);
        deleteTree(avl);
    }

    return 0;
}