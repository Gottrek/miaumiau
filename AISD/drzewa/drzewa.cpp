#include "drzewa.h"

Node* createNode(int val) {
    Node* node = new Node;
    node->key = val;
    node->left = node->right = nullptr;
    node->height = 1;
    return node;
}

int height(Node* node) {
    return node ? node->height : 0;
}

int getBalance(Node* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

Node* insert(Node* root, int key) {
    if (!root) return createNode(key);
    if (key < root->key) root->left = insert(root->left, key);
    else root->right = insert(root->right, key);
    return root;
}

Node* insertAVL(Node* root, int key) {
    if (!root) return createNode(key);
    if (key < root->key) root->left = insertAVL(root->left, key);
    else root->right = insertAVL(root->right, key);

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && key < root->left->key) return rotateRight(root);
    if (balance < -1 && key > root->right->key) return rotateLeft(root);
    if (balance > 1 && key > root->left->key) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

Node* findMin(Node* root) {
    while (root && root->left) root = root->left;
    return root;
}

Node* findMax(Node* root) {
    while (root && root->right) root = root->right;
    return root;
}

Node* remove(Node* root, int key) {
    if (!root) return nullptr;
    if (key < root->key) root->left = remove(root->left, key);
    else if (key > root->key) root->right = remove(root->right, key);
    else {
        if (!root->left) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            Node* temp = root->left;
            delete root;
            return temp;
        } else {
            Node* temp = findMin(root->right);
            root->key = temp->key;
            root->right = remove(root->right, temp->key);
        }
    }
    return root;
}

Node* findNode(Node* root, int key) {
    if (!root || root->key == key) return root;
    if (key < root->key) return findNode(root->left, key);
    return findNode(root->right, key);
}

void inOrder(Node* root) {
    if (!root) return;
    inOrder(root->left);
    cout << root->key << " ";
    inOrder(root->right);
}

void preOrder(Node* root) {
    if (!root) return;
    cout << root->key << " ";
    preOrder(root->left);
    preOrder(root->right);
}

void postOrder(Node* root) {
    if (!root) return;
    postOrder(root->left);
    postOrder(root->right);
    cout << root->key << " ";
    delete root;
}

void printPathTo(Node* root, int key) {
    if (!root) return;
    cout << root->key << " ";
    if (key < root->key) printPathTo(root->left, key);
    else if (key > root->key) printPathTo(root->right, key);
}

void deleteTree(Node*& root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    cout << root->key << " ";
    delete root;
    root = nullptr;
}

void printSubtree(Node* root, int key) {
    Node* node = findNode(root, key);
    if (node) preOrder(node);
    cout << endl;
}