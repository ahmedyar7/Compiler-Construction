#include <iostream>

using namespace std;

class TreeNode {
   public:
    int data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BinaryTree {
   public:
    TreeNode* root;

    BinaryTree() : root(nullptr) {}

    // Updated insertNode
    TreeNode* insertNode(TreeNode* node, int val) {
        // Base case: If we find a null spot, create the node here
        if (node == nullptr) {
            return new TreeNode(val);
        }

        if (val < node->data) {
            node->left = insertNode(node->left, val);
        }

        else if (val > node->data) {
            node->right = insertNode(node->right, val);
        }

        // Return the (unchanged) node pointer
        return node;
    }

    // Helper to clear the tree and avoid memory leaks
    void clearTree(TreeNode* node) {
        if (node == nullptr) return;

        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

    // Traversal methods remain largely the same, just ensure they are called
    // correctly
    void inOrderTraversal(TreeNode* node) {
        if (node == nullptr) return;

        inOrderTraversal(node->left);
        cout << node->data << " ";
        inOrderTraversal(node->right);
    }

    // Destructor to clean up memory automatically
    ~BinaryTree() { clearTree(root); }
};

int main() {
    BinaryTree tree;

    tree.root = tree.insertNode(tree.root, 50);

    tree.insertNode(tree.root, 30);
    tree.insertNode(tree.root, 70);
    tree.insertNode(tree.root, 20);

    cout << "In-Order Traversal: ";

    tree.inOrderTraversal(tree.root);  // Output: 20 30 50 70

    cout << endl;

    return 0;
}