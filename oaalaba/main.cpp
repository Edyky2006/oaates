#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include <windows.h>

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
        if (node == nullptr){
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

    void clear_rec(Node* node) {
        if (node != nullptr) {
            clear_rec(node->left);
            clear_rec(node->right);
            delete node;
        }
    }

    void search_inside_rec(Node* node, long long x_left, long long y_bottom, long long x_right, long long y_top, int depth) {
        if (node == nullptr) return;

        if (node->x >= x_left && node->x <= x_right && node->y >= y_bottom && node->y <= y_top) {
            cout << "Found point: (" << node->x << ", " << node->y << ")\n";
        }

        int cd = depth % 2;

        if (cd == 0) {
            if (x_left < node->x) {
                search_inside_rec(node->left, x_left, y_bottom, x_right, y_top, depth + 1);
            }
            if (x_right >= node->x) {
                search_inside_rec(node->right, x_left, y_bottom, x_right, y_top, depth + 1);
            }
        }
        else {
            if (y_bottom < node->y) {
                search_inside_rec(node->left, x_left, y_bottom, x_right, y_top, depth + 1);
            }
            if (y_top >= node->y) {
                search_inside_rec(node->right, x_left, y_bottom, x_right, y_top, depth + 1);
            }
        }
    }

    void search_above_to_y_rec(Node* node, long long y_limit, int depth) {
        if (node == nullptr) return;

        if (node->y > y_limit) {
            cout << "Found point: (" << node->x << ", " << node->y << ") is above " << y_limit << "\n";
        }

        int cd = depth % 2;

        if (cd == 0) {
            search_above_to_y_rec(node->left, y_limit, depth + 1);
            search_above_to_y_rec(node->right, y_limit, depth + 1);
        }
        else {
            if (y_limit < node->y) {
                search_above_to_y_rec(node->left, y_limit, depth + 1);
            }
            search_above_to_y_rec(node->right, y_limit, depth + 1);
        }
    }

    void search_nn_rec(Node* node, long long target_x, long long target_y, int depth, vector<Node*>& best_nodes, long long& best_dist) {
        if (node == nullptr) return;

        long long dx = target_x - node->x;
        long long dy = target_y - node->y;
        long long current_dist = dx * dx + dy * dy;

        if (best_nodes.empty() || current_dist < best_dist) {
            best_nodes.clear();
            best_nodes.push_back(node);
            best_dist = current_dist;
        }
        else if(current_dist==best_dist){
            best_nodes.push_back(node);
        }

        int cd = depth % 2;

        if (cd == 0) {
            if (target_x < node->x) {
                search_nn_rec(node->left, target_x, target_y, depth + 1, best_nodes, best_dist);

                long long dist_to_wall = (target_x - node->x) * (target_x - node->x);
                if (dist_to_wall <= best_dist) {
                    search_nn_rec(node->right, target_x, target_y, depth + 1, best_nodes, best_dist);
                }
            }
            else {
                search_nn_rec(node->right, target_x, target_y, depth + 1, best_nodes, best_dist);

                long long dist_to_wall = (target_x - node->x) * (target_x - node->x);
                if (dist_to_wall <= best_dist) {
                    search_nn_rec(node->left, target_x, target_y, depth + 1, best_nodes, best_dist);
                }
            }
        }
        else {
            if (target_y < node->y) {
                search_nn_rec(node->left, target_x, target_y, depth + 1, best_nodes, best_dist);

                long long dist_to_wall = (target_y - node->y) * (target_y - node->y);
                if (dist_to_wall <= best_dist) {
                    search_nn_rec(node->right, target_x, target_y, depth + 1, best_nodes, best_dist);
                }
            }
            else {
                search_nn_rec(node->right, target_x, target_y, depth + 1, best_nodes, best_dist);

                long long dist_to_wall = (target_y - node->y) * (target_y - node->y);
                if (dist_to_wall <= best_dist) {
                    search_nn_rec(node->left, target_x, target_y, depth + 1, best_nodes, best_dist);
                }
            }
        }
    }


public:
    KDTree(string n){
        name=n;
        root=nullptr;
        cout << name << " has been created." << endl;
    }

    ~KDTree() {
        clear_rec(root);
    }

    void Insert(long long x, long long y){
        if (contains_rec(root, x, y, 0)) {
            cout << "Point (" << x << ", " << y << ") already exists in " << name << "\n";
            return;
        }

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

    void SearchInside(long long x_left, long long y_bottom, long long x_right, long long y_top) {
        cout << "Points inside [" << x_left << ", " << x_right << "] x [" << y_bottom << ", " << y_top << "]:\n";
        search_inside_rec(root, x_left, y_bottom, x_right, y_top, 0);
    }

    void SearchAboveTO(long long y) {
        cout << "Points with Y > " << y << ":\n";
        search_above_to_y_rec(root, y, 0);
    }

    void SearchNN(long long x, long long y) {
        if (root == nullptr) {
            cout << "Tree " << name << " is empty.\n";
            return;
        }

        vector<Node*> best_nodes;
        long long best_dist = -1;

        search_nn_rec(root, x, y, 0, best_nodes, best_dist);

        for (Node* n: best_nodes) {
            cout << "Found nearest point: (" << n->x << ", " << n->y << ")\n";
        }
    }

};

class Database {
private:
    map<string, KDTree*> tables;

public:
    ~Database() {
        for (auto const& pair : tables) {
            delete pair.second;
        }
    }

    void Create(string name) {
        if (tables.count(name) > 0) {
            cout << "Error: Structure '" << name << "' already exists!\n";
            return;
        }
        tables[name] = new KDTree(name);
    }

    void Insert(string name, long long x, long long y) {
        if (tables.count(name) == 0) {
            cout << "Error: Target structure '" << name << "' does not exist.\n";
            return;
        }
        tables[name]->Insert(x, y);
    }

    void Contains(string name, long long x, long long y) {
        if (tables.count(name) == 0) {
            cout << "Error: Target structure '" << name << "' does not exist.\n";
            return;
        }
        tables[name]->Contains(x, y);
    }

    void PrintTree(string name){
        if (tables.count(name)==0){
            cout << "Error: Target structure " << name << " does not exist.\n";
            return;
        }

        tables[name]->PrintTree();
    }

    void Search(string name){
        if(tables.count(name)==0){
            cout << "Search result: Tree " << name << " does NOT exist.\n";
        }
        else{
            cout << "Search result: Tree " << name << " exist.\n";
            tables[name]->PrintTree();

        }
    }

    void SearchInside(string name, long long x_left, long long y_bottom, long long x_right, long long y_top) {
        if (tables.count(name) == 0) {
            cout << "Error: Target structure '" << name << "' does not exist.\n";
            return;
        }

        tables[name]->SearchInside(x_left, y_bottom, x_right, y_top);
    }

    void SearchAboveTo(string name, long long y) {
        if(tables.count(name)==0){
            cout<< "Error: Target structure " << name << " does not exist.\n";
            return;
        }
        tables[name]->SearchAboveTO(y);
    }

    void SearchNN(string name, long long x, long long y) {
        if (tables.count(name) == 0) {
            cout << "Error: Target structure " << name << " does not exist.\n";
            return;
        }

    tables[name]->SearchNN(x,y);
    }
};
