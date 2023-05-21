#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// AVL tree:
struct AVLNode {
    int key;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

int getHeight(AVLNode* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

int getBalanceFactor(AVLNode* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

AVLNode* insert(AVLNode* root, int key) {
    if (root == nullptr)
        return new AVLNode(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else
        return root; // duplicate keys are not allowed

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getBalanceFactor(root);

    if (balance > 1 && key < root->left->key)
        return rotateRight(root);

    if (balance < -1 && key > root->right->key)
        return rotateLeft(root);

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

void inorderTraversal(AVLNode* root, ofstream& outputFile) {
    if (root == nullptr)
        return;

    inorderTraversal(root->left, outputFile);
    outputFile << root->key << " ";
    inorderTraversal(root->right, outputFile);
}

// Red Black tree:
enum Color { RED, BLACK };

struct RedBlackNode {
    int key;
    Color color;
    RedBlackNode* left;
    RedBlackNode* right;
    RedBlackNode* parent;

    RedBlackNode(int k) : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    RedBlackNode* root;

    void rotateLeft(RedBlackNode* node) {
        RedBlackNode* rightChild = node->right;
        node->right = rightChild->left;

        if (rightChild->left != nullptr)
            rightChild->left->parent = node;

        rightChild->parent = node->parent;

        if (node->parent == nullptr)
            root = rightChild;
        else if (node == node->parent->left)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;

        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(RedBlackNode* node) {
        RedBlackNode* leftChild = node->left;
        node->left = leftChild->right;

        if (leftChild->right != nullptr)
            leftChild->right->parent = node;

        leftChild->parent = node->parent;

        if (node->parent == nullptr)
            root = leftChild;
        else if (node == node->parent->left)
            node->parent->left = leftChild;
        else
            node->parent->right = leftChild;

        leftChild->right = node;
        node->parent = leftChild;
    }

    void fixInsertion(RedBlackNode* node) {
        while (node != root && node->parent->color == RED) {
            if (node->parent == node->parent->parent->left) {
                RedBlackNode* uncle = node->parent->parent->right;

                if (uncle != nullptr && uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        rotateLeft(node);
                    }

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateRight(node->parent->parent);
                }
            } else {
                RedBlackNode* uncle = node->parent->parent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rotateRight(node);
                    }

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateLeft(node->parent->parent);
                }
            }
        }

        root->color = BLACK;
    }

    void inorderTraversal(RedBlackNode* node, ofstream& outputFile) {
        if (node == nullptr)
            return;

        inorderTraversal(node->left, outputFile);
        outputFile << node->key << " ";
        inorderTraversal(node->right, outputFile);
    }

public:
    RedBlackTree() : root(nullptr) {}

    void insert(int key) {
        RedBlackNode* newNode = new RedBlackNode(key);
        RedBlackNode* parent = nullptr;
        RedBlackNode* current = root;

        while (current != nullptr) {
            parent = current;

            if (newNode->key < current->key)
                current = current->left;
            else
                current = current->right;
        }

        newNode->parent = parent;

        if (parent == nullptr)
            root = newNode;
        else if (newNode->key < parent->key)
            parent->left = newNode;
        else
            parent->right = newNode;

        fixInsertion(newNode);
    }

    void printInorderTraversal(ofstream& outputFile) {
        outputFile << "Traversal inorder : ";
        inorderTraversal(root, outputFile);
        outputFile << endl;
    }

    int getHeight(RedBlackNode* node) {
        if (node == nullptr)
            return 0;

        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);

        return std::max(leftHeight, rightHeight) + 1;
    }

    int getHeight() {
        return getHeight(root);
    }
};

int main() {
    const int numFiles = 10;
    const int numIntegers = 1000000;

    for (int i = 1; i <= numFiles; ++i) {
        string inputFilename = "test_file_" + to_string(i) + ".txt";
        ifstream inputFile(inputFilename);

        if (!inputFile) {
            cout << "Failed to open input file: " << inputFilename << endl;
            continue;
        }

        string outputFilename = "output_file_" + to_string(i) + ".txt";
        ofstream outputFile(outputFilename);

        if (!outputFile) {
            cout << "Failed to create output file: " << outputFilename << endl;
            inputFile.close();
            continue;
        }

        int num;
        AVLNode* rootAVLTree = nullptr;
        RedBlackTree rbTree;

        while (inputFile >> num) {
            rootAVLTree = insert(rootAVLTree, num);
            rbTree.insert(num);
        }

        inputFile.close();

        outputFile << "File: " << inputFilename << endl;
        outputFile << "Height of AVL tree: " << getHeight(rootAVLTree) << endl;
        outputFile << "Height of Red Black tree: " << rbTree.getHeight() << endl;
        outputFile << endl;

        outputFile.close();
    }

    return 0;
}
