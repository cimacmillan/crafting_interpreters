#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

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

pair<AssignmentRange, AssignmentRange> get_assignment_range_from_line(string line) {
    vector<string> splat = split(line, ",");
    assert(splat.size() == 2);
    auto first = get_assignment_range_from_part(splat[0]);
    auto second = get_assignment_range_from_part(splat[1]);
    return make_pair(first, second);
}

vector<pair<AssignmentRange, AssignmentRange>> get_assignment_ranges(vector<string> lines) {
    vector<pair<AssignmentRange, AssignmentRange>> results;
    for (auto line : lines) {
        auto pair = get_assignment_range_from_line(line);
        results.push_back(pair);
    }
    return results;
}

bool is_range_a_within_b(AssignmentRange a, AssignmentRange b) {
    return a.start >= b.start && a.end <= b.end;
}

bool does_range_overlap(AssignmentRange a, AssignmentRange b) {
    bool does_overlap = true;
    if (a.end < b.start) {
        does_overlap = false;
    }
    if (a.start > b.end) {
        does_overlap = false;
    }
    return does_overlap;
}


int main() {
    auto input = readFile("src/advent/4.txt");
    vector<string> lines = split(std::move(input), "\n");
    vector<pair<AssignmentRange, AssignmentRange>> ranges = get_assignment_ranges(lines);
    for (auto range : ranges) {
        printf("%d %d, %d %d\n", range.first.start, range.first.end, range.second.start, range.second.end);
    }
    int number = 0;

    for (auto range : ranges) {
        if (does_range_overlap(range.first, range.second)) {
            number++;
        }
    }

    printf("%d\n", number);
}
