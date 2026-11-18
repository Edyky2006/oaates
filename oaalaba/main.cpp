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
                if (dist_to_wall < best_dist) {
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
                if (dist_to_wall < best_dist) {
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


void to_upper_case(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
}

bool is_valid_identifier(const string& str) {
    if (str.empty()) return false;

    if (!isalpha(str[0])) {
        return false;
    }

    for (size_t i = 1; i < str.length(); ++i) {
        char c = str[i];
        if (!isalnum(c) && c != '_') {
            return false;
        }
    }
    return true;
}

bool parse_tree_and_point(stringstream& ss, string& name, long long& x, long long& y) {
    char left_paren, comma, right_paren;
    if (!(ss >> name >> left_paren >> x >> comma >> y >> right_paren)) {
        return false;
    }
    if (left_paren != '(' || comma != ',' || right_paren != ')') {
        return false;
    }
    return true;
}

bool parse_point(stringstream& ss, long long& x, long long& y) {
    char lp, comma, rp;
    if (!(ss >> lp >> x >> comma >> y >> rp)) return false;
    return (lp == '(' && comma == ',' && rp == ')');
}

bool parse_two_points(stringstream& ss, long long& x1, long long& y1, long long& x2, long long& y2) {
    char lp1, c1, rp1, comma_mid, lp2, c2, rp2;
    if (!(ss >> lp1 >> x1 >> c1 >> y1 >> rp1 >> comma_mid >> lp2 >> x2 >> c2 >> y2 >> rp2)) return false;
    return (lp1 == '(' && c1 == ',' && rp1 == ')' && comma_mid == ',' &&
            lp2 == '(' && c2 == ',' && rp2 == ')');
}

// =================================
void run_parser() {
    Database db;
    string command_block;

    cout << "KD-Tree Database System. Commands: CREATE, INSERT, CONTAINS, PRINT_TREE, SEARCH(WHERE INSIDE; ABOVE TO; NN)." << endl;

    while (getline(cin, command_block, ';')) {

        string trash;
        getline(cin, trash, '\n');

        stringstream ss(command_block);
        string command;

        if (!(ss >> command)) {
            continue;
        }

        to_upper_case(command);

        if (command == "CREATE") {
            string name;
            if (!(ss >> name)) {
                cout << "Error: Missing name for new set..\n";
                continue;
            }

            if (!is_valid_identifier(name)) {
                cout << "Error: Invalid format. Must match [a-zA-Z][a-zA-Z0-9_]*\n";
                continue;
            }

            db.Create(name);
        }

        else if (command == "INSERT") {
            string name;
            long long x, y;
            if (!parse_tree_and_point(ss, name, x, y)) {
                cout << "Error: Incorrect syntax. INSERT requires name and 2 numbers (x, y).\n";
                continue;
            }
            db.Insert(name, x, y);
        }

        else if (command == "CONTAINS") {
            string name;
            long long x, y;
            if (!parse_tree_and_point(ss, name, x, y)) {
                cout << "Error: Incorrect syntax. CONTAINS requires name and 2 numbers (x, y).\n";
                continue;
            }
            db.Contains(name, x, y);
        }

        else if (command == "PRINT_TREE") {
            string name;
            if (!(ss >> name)) {
                cout << "Error: Missing name for PRINT_TREE command.\n";
                continue;
            }
            db.PrintTree(name);
        }

        else if (command == "SEARCH") {
            string name;
            if (!(ss >> name)) {
                cout << "Error: Missing name for SEARCH command.\n";
                continue;
            }

            string next_word;
            if (ss >> next_word) {
                to_upper_case(next_word);

                if (next_word == "WHERE") {
                    string query_type;

                    if (ss >> query_type) {
                        to_upper_case(query_type);

                        if (query_type == "INSIDE") {
                            long long x_left, y_bottom, x_right, y_top;
                            if (!parse_two_points(ss, x_left, y_bottom, x_right, y_top)) {
                                cout << "Error: Incorrect syntax. Expected: INSIDE (x1,y1), (x2,y2)\n";
                                continue;
                            }
                            db.SearchInside(name, x_left, y_bottom, x_right, y_top);
                        }
                        else if (query_type == "ABOVE_TO") {
                            long long x;
                            if (!(ss >> x)) {
                                cout << "Error: Incorrect syntax. Expected: ABOVE_TO y\n";
                                continue;
                            }
                            db.SearchAboveTo(name, x);
                        }
                        else if (query_type == "NN") {
                            long long x, y;
                            if (!parse_point(ss, x, y)) {
                                cout << "Error: Incorrect syntax. Expected: NN (x, y)\n";
                                continue;
                            }
                            db.SearchNN(name, x, y);
                        }
                        else {
                            cout << "Error: Unknown query type -> " << query_type << "\n";
                        }
                    } else {
                        cout << "Error: Missing query type (INSIDE, ABOVE_TO, NN) after WHERE.\n";
                    }
                } else {
                    cout << "Error: Expected WHERE keyword, found -> " << next_word << "\n";
                }
            }
            else {
                db.Search(name);
            }
        }

        else if (command == "EXIT") {
            break;
        }

        else {
            cout << "Error: Unknown command syntax -> " << command << "\n";
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    run_parser();

    return 0;
}
