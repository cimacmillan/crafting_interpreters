#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;

enum BinaryTreeError {
    InitializationError = 0
};

template <typename T> struct BinaryTreeNode {
    T value;
    BinaryTreeNode *left = nullptr;
    BinaryTreeNode *right = nullptr;

    BinaryTreeNode(T value): value(value) {

    }

    string toString() {
        string output = to_string(this->value);
        if (this->left) {
            output += ", left(" + this->left->toString() + ")";
        }
        if (this->right) {
            output += ", right(" + this->right->toString() + ")";
        }
        return output;
    }

    static BinaryTreeNode* from_vector(vector<T> values, size_t from, size_t to) {
        if (values.size() == 0) {
            throw BinaryTreeError::InitializationError;
        }

        if (from >= values.size()) {
            return nullptr;
        }

        // 1, 9, 3, 1, 4, 5 -> (1) and [9, 3, 1, 4, 5 ]
        BinaryTreeNode *result = new BinaryTreeNode(values[from]);
        size_t new_from = from + 1;        
        size_t midpoint = (new_from + to) / 2;

        cout << new_from << " " << midpoint << " " << to << endl;

        if (from >= values.size() || midpoint >= values.size() || from >= to) {
            return result;
        }

        result->left = BinaryTreeNode<T>::from_vector(values, new_from, midpoint);
        result->right = BinaryTreeNode<T>::from_vector(values, midpoint, to);

        return result;
    }


};

int main_binary() {
    vector<int> list_of_ints = {1, 9, 3, 1, 4, 5};

    auto integer_tree = BinaryTreeNode<int>::from_vector(list_of_ints, 0, list_of_ints.size() - 1);

    cout << integer_tree->toString() << endl;
}