#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
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



int main() {
    auto input = readFile("src/advent/6.txt");
    vector<string> lines = split(std::move(input), "\n");
    
    int length = 14;
    unordered_map<char, int> occurrences;
    for (int i = 0; i < length; i++) {
        occurrences[lines[0][i]]++;
    }

    for (int i = length; i < lines[0].size(); i++) {
        char leaving_char = lines[0][i - length];
        char arriving_char = lines[0][i];

        occurrences[leaving_char]--;
        if (occurrences[leaving_char] == 0) {
            occurrences.erase(leaving_char);
        }

        occurrences[arriving_char]++;

        printf("%d\n", occurrences.size());
        if (occurrences.size() == length) {
            for (int b = i - length + 1; b <= i; b++) {
                printf("%c", lines[0][b]);
            }
            printf("\n");
            printf("%d\n", i + 1);
            break;
        }
    }
  
}
