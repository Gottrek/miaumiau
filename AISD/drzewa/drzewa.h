#ifndef DRZEWA_H
#define DRZEWA_H

#include <iostream>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
};

Node* createNode(int val);
Node* insert(Node* root, int key);
Node* insertAVL(Node* root, int key);
Node* findMin(Node* root);
Node* findMax(Node* root);
Node* remove(Node* root, int key);
Node* findNode(Node* root, int key);
void inOrder(Node* root);
void preOrder(Node* root);
void postOrder(Node* root);
void printPathTo(Node* root, int key);
void deleteTree(Node*& root);
void printSubtree(Node* root, int key);
int height(Node* node);
int getBalance(Node* node);
Node* rotateRight(Node* y);
Node* rotateLeft(Node* x);

#endif
