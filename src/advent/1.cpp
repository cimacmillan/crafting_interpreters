#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

string readFile(string filename) {
    ifstream file(filename);
    string contents((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    file.close();
    return contents;
}



int main_1() {
    auto input = readFile("src/advent/1.txt");
    int group = 0;
    int running_sum = 0;
    int current_num = 0;
    vector<int> sums = {};
    for (int i = 0; i < input.size(); i++) {
        char c = input[i];
        if (c == '\n') {
            running_sum += current_num;
            current_num = 0;
            if (i < input.size() - 1 && input[i + 1] == '\n') {
                group++;
                sums.push_back(running_sum);
                running_sum = 0;    
                i++;
            }
            continue;
        }   
        int digit = c - '0';
        current_num = (current_num * 10) + digit;
    }
    sort(sums.begin(), sums.end());
    cout << sums[sums.size() - 1] + sums[sums.size() - 2] + sums[sums.size() - 3] << endl;
}
