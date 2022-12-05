#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <stack>

using namespace std;

struct AssignmentRange {
    int start;
    int end;
};

vector<string> split(const string &file, const string &delimit) {
    vector<string> lines;
    string contents = file;
    while(contents.size() > 0) {
        auto found = contents.find(delimit);
        if (found == string::npos) {
            lines.push_back(contents);
            break;
        }
        auto sub = contents.substr(0, found);
        contents = contents.substr(found + 1, contents.size());
        lines.push_back(std::move(sub));
    }
    return lines;
}

string readFile(const string &filename) {
    ifstream file(filename);
    string contents((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    file.close();
    return contents;
}

AssignmentRange get_assignment_range_from_part(string part) {
    vector<string> digits = split(part, "-");
    assert(digits.size() == 2);
    int start = stoi(digits[0]);
    int end = stoi(digits[1]);
    AssignmentRange range = {
        .start = start,
        .end = end
    };
    return range;
}

deque<char> stacks[9];

int create_stacks(vector<string> lines) {
    int line_num = 0;
    while (true) {
        int start = 1;
        int jump = 4;
        int curr_pos = start;
        string line = lines[line_num];
        if (line[curr_pos] == '1') {
            break;
        }
        for (int i = 0; i < 9; i++) {
            char value = line[curr_pos];
            curr_pos += jump;
            if (value != ' ') stacks[i].push_front(value);
        }
        line_num++;
    }
    return line_num + 2;
}


void move(string line) {
    int amount, from, to;
    sscanf(line.c_str(), "move %d from %d to %d", &amount, &from, &to);
    printf("%d, %d, %d\n", amount, from, to);
    from--;
    to--;
    stack<char> temp;
    for (int i = 0; i < amount; i++) {
        char item = stacks[from].back();
        stacks[from].pop_back();
        temp.push(item);
    }
    int stack_size = temp.size();
    for (int i = 0; i < stack_size; i++) {
        stacks[to].push_back(temp.top());
        temp.pop();
    }
}


int main() {
    auto input = readFile("src/advent/5.txt");
    vector<string> lines = split(std::move(input), "\n");
    int line = create_stacks(lines);
    for (auto stack : stacks) {
        for (auto letter : stack) {
            printf("%c ", letter);
        }
        printf("\n");
    }

    for (int i = line; i < lines.size(); i++) {
        move(lines[i]);
    }

    for (auto stack : stacks) {
        for (auto letter : stack) {
            printf("%c ", letter);
        }
        printf("\n");
    }

    printf("\n");

    for (auto stack : stacks) {
        printf("%c", stack.back());
    }
  
}
