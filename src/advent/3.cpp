#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

using namespace std;

vector<string> split(const string &file, const string &delimit) {
    vector<string> lines;
    string contents = file;
    while(contents.size() > 1) {
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

int get_priority_from_char(char character) {
    if (character >= 'a' && character <= 'z') {
        return character - 'a' + 1;
    }
    int priority = character - 'A' + 27;
    return priority;
}

unordered_set<char> get_set_from_line(const string& line) {
    unordered_set<char> left;
    int size = line.size();
    for (int i = 0; i < size; i++) {
        left.insert(line[i]);
    }
    return left;
}

char get_common_char(vector<unordered_set<char>> a, int start, int end) {
    unordered_set<char> entries = a[start];
    for (auto entry : entries) {
        bool in = true;
        for (int i = start + 1; i < end; i++) {
            if (a[i].find(entry) == a[i].end()) {
                in = false;
            }
        }
        if (in) {
            return entry;
        }
    }
    return 'a';
}

int get_priority_from_line(const string &line) {
    unordered_set<char> left;
    int size = line.size() / 2;
    for (int i = 0; i < size; i++) {
        left.insert(line[i]);
    }
    for (int i = size; i < line.size(); i++) {
        if (left.find(line[i]) != left.end()) {
            cout << line[i] << endl;
            return get_priority_from_char(line[i]);
        }
    }
    return 0;
}

int main() {
    auto input = readFile("src/advent/3.txt");
    vector<string> lines = split(std::move(input), "\n");
    vector<unordered_set<char>> maps;
    for (auto line : lines) {
        maps.push_back(get_set_from_line(line));
    }
    int priority = 0;
    for (int i = 0; i < lines.size(); i += 3) {
        priority += get_priority_from_char(get_common_char(maps, i, i + 3));
    }
    cout << priority << endl;
}
