#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <stack>

using namespace std;

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

struct File {
    uint size;
    unordered_map<string, File*> children;
    File *parent;
};


uint get_total_size(File *file, vector<uint> &sizes) {
    uint result = file->size;
    bool is_dir = false;
    for (auto entry : file->children) {
        is_dir = true;
        result += get_total_size(entry.second, sizes);
    }
    if (is_dir) {
        sizes.push_back(result);
    }
    return result;
}

int main() {
    auto input = readFile("src/advent/7.txt");
    vector<string> lines = split(std::move(input), "\n");

    File root = {
        .size = 0,
        .children = {},
        .parent = nullptr
    };

    File *current = &root;

    for (auto line : lines) {
        cout << line << endl;

        // command
        if (line[0] == '$') {
            vector<string> args = split(line, " ");
            if (args[1] == "cd") {
                if (args[2] == "/") continue;
                if (args[2] == "..") {
                    current = current->parent;
                    continue;
                }
                current = current->children.at(args[2]);
            }
        } else {
            vector<string> args = split(line, " ");
            uint size = 0;
            if (args[0] != "dir") {
                size = stoi(args[0]);
            } 
            current->children.insert(std::make_pair(args[1], new File({
                .size = size,
                .children = {},
                .parent = current
            })));
        }
    }

    vector<uint> sizes;
    cout << get_total_size(&root, sizes) << endl;

    uint total_size = 70000000;
    uint update_size = 30000000;
    uint total_left = total_size - get_total_size(&root, sizes);
    uint total_diff = update_size - total_left;

    sort(sizes.begin(), sizes.end());

    uint size_result;
    for (auto size : sizes) {
        // cout << size << endl;
        if (size >= total_diff) {
            size_result = size;
            break;
        }
    }

    cout << size_result << endl;
    
}
