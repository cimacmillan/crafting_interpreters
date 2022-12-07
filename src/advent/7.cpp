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


int main() {
    auto input = readFile("src/advent/7.txt");
    vector<string> lines = split(std::move(input), "\n");

    for (auto line : lines) {
        cout << line << endl;
    }
    
}
