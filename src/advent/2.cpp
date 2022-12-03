#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

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

enum Hand {
    ROCK = 0,
    PAPER = 1,
    SCISSORS = 2,
    WIN = 1,
    DRAW = 0,
    LOSE = 2
};

Hand get_hand(char letter) {
    switch (letter) {
        case 'A':
            return Hand::ROCK;
        case 'B':
            return Hand::PAPER;
        case 'C':
            return Hand::SCISSORS;
        case 'X':
            return Hand::LOSE;
        case 'Y':
            return Hand::DRAW;
        case 'Z':
            return Hand::WIN;
    }
    cout << "no hand" << endl;
    exit(1);
}


int get_score(Hand a, Hand desired) {
    int b = (a + desired) % 3;
    int play_score = b + 1;
    int outcome_score = 0;
    if (a == b) {
        outcome_score = 3;
    }
    if ((b + 1) % 3 == a) {
        // lost
    }
    if ((b + 2) % 3 == a) {
        outcome_score = 6;
    }
    return play_score + outcome_score;
}

int main_2() {
    auto input = readFile("src/advent/2.txt");
    vector<string> lines = split(std::move(input), "\n");
    int score = 0;
    for (auto vec : lines) {
        auto hand_a = get_hand(vec[0]);
        auto hand_b = get_hand(vec[2]);
        score += get_score(hand_a, hand_b);
    }   
    cout << score << endl;
}
