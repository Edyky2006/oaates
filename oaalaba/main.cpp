#include <iostream>
#include <string>

using namespace std;

class KDTree {
private:
    string name;

    struct Node {
        long long x;
        long long y;
        Node* left;
        Node* right;

        Node(long long px, long long py ){
            x=px;
            y=py;
            left=nullptr;
            right=nullptr;
        }
    };

    Node* root;

    Node* insert_rec(Node* node, long long x, long long y, int depth) {
        if (node == 0){
            return new Node(x, y);
        }

        int cd = depth % 2;

    if (cd == 0) {
        if (x < node->x)
            node->left = insert_rec(node->left, x, y, depth + 1);
        else
            node->right = insert_rec(node->right, x, y, depth + 1);
        }
        else {
        if (y < node->y)
            node->left = insert_rec(node->left, x, y, depth + 1);
        else
            node->right = insert_rec(node->right, x, y, depth + 1);
        }

        return node;
    }

    bool contains_rec(Node* node, long long x, long long y, int depth){
        if (node == 0 || node == nullptr){
            return false;
        }

        if (node->x == x && node->y == y){
            return true;
        }
        else{
            int cd = depth % 2;

            if (cd == 0){
                if(x < node->x)
                    return contains_rec(node->left, x, y, depth + 1);
                else
                   return contains_rec(node->right, x, y, depth + 1);
            }

            else {
                if(y < node->y)
                    return contains_rec(node->left, x, y, depth + 1);
                else
                    return contains_rec(node->right, x, y, depth + 1);
            }


        }

    }

    void print_rec(Node* node, string prefix, bool is_tail) {
        if (node == nullptr) return;

        cout << prefix << (is_tail ? "└── " : "├── ") << "(" << node->x << ", " << node->y << ")\n";

        string next_prefix = prefix + (is_tail ? "    " : "│   ");

        if (node->left != nullptr) {
            bool left_is_tail = (node->right == nullptr);
            print_rec(node->left, next_prefix, left_is_tail);
        }

        if (node->right != nullptr) {
            print_rec(node->right, next_prefix, true);
        }
    }

public:
    KDTree(string n){
        name=n;
        root=nullptr;
        cout << name << "has been created." << endl;
    }
    void Insert(long long x, long long y){
        root = insert_rec(root, x, y, 0);
        cout << "Point ("<< x << ", " << y << ") added to " << name << "." << endl;
    }

    void Contains(long long x, long long y){
        bool is_there = contains_rec(root, x, y, 0);

        if (is_there){
            cout << "TRUE" << endl;
        }

        else{
            cout << "FALSE" << endl;
        }
    }

    void PrintTree() {
        if (root == nullptr) {
            cout << "Tree " << name << " is empty.\n";
            return;
        }

        cout << "(" << root->x << ", " << root->y << ")\n";

        if (root->left != nullptr) {
            bool left_is_tail = (root->right == nullptr);
            print_rec(root->left, "", left_is_tail);
        }
        if (root->right != nullptr) {
            print_rec(root->right, "", true);
        }
    }

};
