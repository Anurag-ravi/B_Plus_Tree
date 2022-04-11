#include <bits/stdc++.h>
using namespace std;

class Tree_Node;

class Node{
public:
    int data;
    Tree_Node *left_child;

    Node(){
        left_child = nullptr;
    }
};

class Tree_Node{
public:
    vector<Node *> values;
    bool isDataNode;
    Tree_Node *right_child;
    Tree_Node *parent;

    Tree_Node(){
        right_child = nullptr;
        parent = nullptr;
    }
};

bool compare(Node *a, Node *b){
    return a->data < b->data;
}

class B_Plus_Trees{
public:
    Tree_Node *root;
    int d;
    int t;
    B_Plus_Trees(int d, int t){
        root = nullptr;
        this->d = d;
        this->t = t;
    }

    void insert(int value){
        // initially when root is null
        if (root == nullptr){
            Tree_Node *x = new Tree_Node();
            x->isDataNode = true;

            Node *n = new Node();
            n->data = value;
            x->values.push_back(n);
            root = x;
            return;
        }

        // when the root is datanode
        if (root->isDataNode == true){
            // if root is not getting full
            if (root->values.size() < 2 * d){
                Node *n = new Node();
                n->data = value;
                root->values.push_back(n);
                sort(root->values.begin(), root->values.end(), compare);
            }
            // if root is getting full then create new root and rearrange the pointers
            else{
                Node *n = new Node();
                n->data = value;
                root->values.push_back(n);
                sort(root->values.begin(), root->values.end(), compare);

                Tree_Node *right_Node = new Tree_Node();
                right_Node->isDataNode = true;

                for (int i = d; i < root->values.size(); i++){
                    Node *n = new Node();
                    n->data = root->values[i]->data;
                    right_Node->values.push_back(n);
                }

                for (int i = 0; i < d + 1; i++){
                    root->values.pop_back();
                }

                Tree_Node *new_root = new Tree_Node();
                new_root->isDataNode = false;

                n = new Node();
                n->data = right_Node->values[0]->data;
                n->left_child = root;

                new_root->values.push_back(n);
                new_root->right_child = right_Node;

                root = new_root;
                root->values[0]->left_child->parent = root;
                root->right_child->parent = root;
            }
            return;
        }

        // traverse to leaf node
        Tree_Node *curr = root;
        while (curr->isDataNode == false){
            int count = 0;
            for (int i = 0; i < curr->values.size(); i++){
                if (curr->values[i]->data > value){
                    count = 1;
                    curr = curr->values[i]->left_child;
                    break;
                }
            }
            if (count == 0){
                curr = curr->right_child;
            }
        }
        // check if leaf node has space, if yes then insert and return
        if (curr->values.size() < 2 * d){
            Node *n = new Node();
            n->data = value;
            curr->values.push_back(n);
            sort(curr->values.begin(), curr->values.end(), compare);
            return;
        }

        Node *n = new Node();
        n->data = value;
        curr->values.push_back(n);
        sort(curr->values.begin(), curr->values.end(), compare);

        // splitting leaf Node
        Tree_Node *right_Node = new Tree_Node();
        right_Node->isDataNode = true;

        for (int i = d; i < curr->values.size(); i++){
            Node *n = new Node();
            n->data = curr->values[i]->data;
            right_Node->values.push_back(n);
        }

        for (int i = 0; i < d + 1; i++){
            curr->values.pop_back();
        }

        // Sending index value to parent node
        n = new Node();
        n->data = right_Node->values[0]->data;
        right_Node->parent = curr->parent;

        n->left_child = curr;
        if (n->data > curr->parent->values[curr->parent->values.size() - 1]->data){
            curr->parent->values.push_back(n);
            curr->parent->right_child = right_Node;
        }
        else{
            curr->parent->values.push_back(n);
            Tree_Node *curr_parent = curr->parent;
            sort(curr_parent->values.begin(), curr_parent->values.end(), compare);
            int j = 0;
            for (j = 0; j < curr_parent->values.size(); j++){
                if (curr_parent->values[j]->data == n->data){
                    break;
                }
            }

            curr_parent->values[j + 1]->left_child = right_Node;
        }

        curr = curr->parent;
        if (curr == nullptr){
            return;
        }

        while (curr->values.size() > 2 * t + 1){
            // Special root case

            if (curr == root || curr == nullptr){
                break;
            }

            // Splitting index Node

            Tree_Node *right_Node = new Tree_Node();
            right_Node->isDataNode = false;

            int up_val = curr->values[t]->data;
            for (int i = t + 1; i < curr->values.size(); i++){
                Node *n = new Node();
                n->data = curr->values[i]->data;
                n->left_child = curr->values[i]->left_child;
                right_Node->values.push_back(n);
            }

            right_Node->parent = curr->parent;
            right_Node->right_child = curr->right_child;
            curr->right_child = curr->values[t]->left_child;
            for (int i = 0; i < t + 2; i++){
                curr->values.pop_back();
            }

            for (int i = 0; i < right_Node->values.size(); i++){
                right_Node->values[i]->left_child->parent = right_Node;
            }
            right_Node->right_child->parent = right_Node;

            // Sending index value to parent node
            Node *n = new Node();
            n->data = up_val;
            n->left_child = curr;

            if (n->data > curr->parent->values[curr->parent->values.size() - 1]->data){
                curr->parent->values.push_back(n);
                curr->parent->right_child = right_Node;
            }
            else{
                curr->parent->values.push_back(n);
                Tree_Node *curr_parent = curr->parent;
                sort(curr_parent->values.begin(), curr_parent->values.end(), compare);
                int j = 0;
                for (j = 0; j < curr_parent->values.size(); j++){
                    if (curr_parent->values[j]->data == n->data){
                        break;
                    }
                }

                curr_parent->values[j + 1]->left_child = right_Node;
            }
            curr = curr->parent;
        }

        if (curr == root && curr->values.size() > 2 * t + 1){
            Tree_Node *right_Node = new Tree_Node();
            right_Node->isDataNode = false;

            int up_val = curr->values[t]->data;

            for (int i = t + 1; i < root->values.size(); i++){
                Node *n = new Node();
                n->data = root->values[i]->data;
                n->left_child = root->values[i]->left_child;
                right_Node->values.push_back(n);
            }

            right_Node->right_child = curr->right_child;
            curr->right_child = curr->values[t]->left_child;

            for (int i = 0; i < t + 2; i++){
                root->values.pop_back();
            }
            for (int i = 0; i < right_Node->values.size(); i++){
                right_Node->values[i]->left_child->parent = right_Node;
            }
            right_Node->right_child->parent = right_Node;

            Tree_Node *new_root = new Tree_Node();
            new_root->isDataNode = false;

            n = new Node();
            n->data = up_val;
            n->left_child = root;

            new_root->values.push_back(n);
            new_root->right_child = right_Node;

            root = new_root;
            root->values[0]->left_child->parent = root;
            root->right_child->parent = root;
        }
    }

    int count_index(Tree_Node *root){
        if (root == nullptr) return 0;
        if (root->isDataNode == true) return 0;
        int count = 1;
        for (int i = 0; i < root->values.size(); i++){
            count += count_index(root->values[i]->left_child);
        }
        count += count_index(root->right_child);
        return count;
    }

    int count_data(Tree_Node *root){
        if (root == nullptr) return 0;
        if (root->isDataNode == true) return 1;
        int count = 0;
        for (int i = 0; i < root->values.size(); i++){
            count += count_data(root->values[i]->left_child);
        }
        count += count_data(root->right_child);
        return count;
    }

    void print(){
        if (root == nullptr) return;
        cout << count_index(root) << " " << count_data(root) << " ";
        for (int i = 0; i < root->values.size(); i++){
            cout << root->values[i]->data << " ";
        }
        cout << endl;
        return;
    }
};

int main(){
    int d, t;
    cin >> d >> t;
    B_Plus_Trees tree(d, t);
    int code;
    while (true){
        cin >> code;
        switch (code){
        case 1:
            int val;
            cin >> val;
            tree.insert(val);
            break;

        case 2:
            tree.print();
            break;
        
        default:
            return 0;
            break;
        }
    }
    return 0;
}