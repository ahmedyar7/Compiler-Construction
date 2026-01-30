#include <iostream>

using namespace std;

class TreeNode {
   public:
    int data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int data) {
        this->data = data;
        left = nullptr;
        right = nullptr;
    }
};

class BinaryTree {
   public:
    TreeNode* root;

    BinaryTree() { root = nullptr; }

    TreeNode* insertNode(TreeNode* node, int val) {
        if (node == nullptr) {
            return new TreeNode(val);
        }

        else if (val > node->data) {
            node->right = insertNode(node->right, val);
        }

        else {
            node->left = insertNode(node->left, val);
        }

        return node;
    }

    void clearTree(TreeNode* node) {
        if (node == nullptr) {
            return;
        }

        clearTree(node->left);
        clearTree(node->right);

        return;
    }

    void preOrderTraversal(TreeNode* node) {
        if (node == nullptr) {
            return;
        }

        cout << node->data << " ";

        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }

    void postOrderTraversal(TreeNode* node) {
        if (node == nullptr) {
            return;
        }

        postOrderTraversal(node->left);
        postOrderTraversal(node->right);

        cout << node->data << " ";
    }

    void inOrderTraversal(TreeNode* node) {
        if (node == nullptr) {
            return;
        }

        inOrderTraversal(node->left);
        cout << node->data << " ";
        inOrderTraversal(node->right);
    }

    ~BinaryTree() { clearTree(root); }
};

int main() {
    BinaryTree tree;

    tree.root = tree.insertNode(tree.root, 5);

    tree.insertNode(tree.root, 3);
    tree.insertNode(tree.root, 7);
    tree.insertNode(tree.root, 2);
    tree.insertNode(tree.root, 4);
    tree.insertNode(tree.root, 1);

    cout << "PRE-Order Traversal: \n";
    tree.preOrderTraversal(tree.root);  // Output: 20 30 50 70

    cout << "In-Order Traversal: ";
    tree.inOrderTraversal(tree.root);  // Output: 20 30 50 70
    
    cout << "POST-Order Traversal: ";
    tree.postOrderTraversal(tree.root);  // Output: 20 30 50 70

    cout << endl;

    return 0;
}