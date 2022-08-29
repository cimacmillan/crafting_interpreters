#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <set>

using namespace std;

int main_datastructs() {

    stack<int> stack_numbers;
    stack_numbers.push(1);
    stack_numbers.push(2);
    stack_numbers.push(3);
    for (int i = 0; i < 3; i++) {
        cout << stack_numbers.top() << endl;
        stack_numbers.pop();
    }

    queue<int> queue_numbers;
    queue_numbers.push(1);
    queue_numbers.push(2);
    queue_numbers.push(3);
    for (int i = 0; i < 3; i++) {
        cout << queue_numbers.front() << endl;
        queue_numbers.pop();
    }

    deque<int> deque_numbers;
    deque_numbers.push_front(1);
    deque_numbers.push_front(2);
    deque_numbers.push_back(3);
    cout << deque_numbers.front() << " " << deque_numbers.back() << endl;

    map<string, int> map_counts;
    map_counts["value"] = 0;
    if (map_counts.find("value") != map_counts.end()) {
        cout << "value found" << endl;
    }
    map_counts.erase("value");
    if (map_counts.find("value") == map_counts.end()) {
        cout << "value is missing" << endl;
    }
    map_counts["value"] = 0;
    map_counts["dog"] = 1;
    map_counts["cat"] = 2;
    map_counts["koalla"] = 0;
    for (auto &entry: map_counts) {
        cout << entry.first << " " << entry.second << endl;
    }
    cout << (*map_counts.begin()).first << " " << (*(--map_counts.end())).first << endl;

    // Min priority queue. Greater means smallest out first?
    priority_queue<int, vector<int>, greater<int>> priority;
    priority.push(10);
    priority.push(12);
    priority.push(14);
    cout << priority.top() << endl;

    //set for binary tree? less means smallest first
    set<int, less<int>> binary_tree;
    binary_tree.insert(4);
    binary_tree.insert(8);
    binary_tree.insert(12);
    for (auto &entry: binary_tree) {
        cout << entry << endl;
    }

    set<string, less<string>> alphabetical_sort;
    alphabetical_sort.insert("callum");
    alphabetical_sort.insert("ace");
    alphabetical_sort.insert("ballum");
    alphabetical_sort.insert("allum");
    alphabetical_sort.insert("acronymum");
    for (auto &entry: alphabetical_sort) {
        cout << entry << endl;
    }
}
