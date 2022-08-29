#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>

using namespace std;

bool is_character_valid(char character) {
    auto alphabet_index = character - 'A';
    return alphabet_index >= 0 && alphabet_index < 26;
}

bool are_options_valid(unordered_map<char, int> options) {
    for (auto &entry : options) {
        if (!is_character_valid(entry.first)) {
            return false;
        }
    }
    return true;
}

bool can_create_sentence(string sentence, unordered_map<char, int> options) {
    if (!are_options_valid(options)) 
        throw runtime_error("Options are invalid. Only capitalised letters are supported");

    for (auto &character_in_sentence : sentence) {
        auto uppercase_character = toupper(character_in_sentence);
        if (!is_character_valid(uppercase_character)) {
            continue;
        }

        auto character_count = options[uppercase_character];

        if (character_count <= 0) {
            return false;
        }

        options[uppercase_character] = character_count - 1;
    }

    return true;
}

void test_basic_text() {
    string target_result = "DOG";
    unordered_map<char, int> options = {
        {'D', 1},
        {'O', 1},
        {'G', 1},
    };

    assert(can_create_sentence(target_result, options) == true);
}

void test_missing_letters() {
    string target_result = "DOG";
    unordered_map<char, int> options = {
        {'D', 1},
        {'O', 1},
    };

    assert(can_create_sentence(target_result, options) == false);
}

void test_multiple_letters() {
    string target_result = "DDOG";
    unordered_map<char, int> options = {
        {'D', 2},
        {'O', 1},
        {'G', 1},
    };

    assert(can_create_sentence(target_result, options) == true);
}

void test_text_with_space() {
    string target_result = "HE LLO";
    unordered_map<char, int> options = {
        {'H', 1},
        {'E', 1},
        {'L', 2},
        {'O', 1},
    };

    assert(can_create_sentence(target_result, options) == true);
}

void test_text_with_punctuation() {
    string target_result = "HELO.";
    unordered_map<char, int> options = {
        {'H', 1},
        {'E', 1},
        {'L', 2},
        {'O', 1},
    };

    assert(can_create_sentence(target_result, options) == true);
}

void test_text_with_upper_lower_case_mix() {
    string target_result = "dog";
    unordered_map<char, int> options = {
        {'D', 1},
        {'O', 1},
        {'G', 1},
    };

    assert(can_create_sentence(target_result, options) == true);    
}

void test_throws_error_when_given_lowercase_options() {
    string target_result = "dog";
    unordered_map<char, int> options = {
        {'d', 1},
        {'o', 1},
        {'g', 1},
    };

    try {
        can_create_sentence(target_result, options);
        assert(0); // This should never be reached
    } catch(runtime_error e) {}
}

void test_throws_error_when_given_punctuation_options() {
    string target_result = "dog";
    unordered_map<char, int> options = {
        {'d', 1},
        {'o', 1},
        {'g', 1},
        {'.', 1},
    };

    try {
        can_create_sentence(target_result, options);
        assert(0); // This should never be reached
    } catch(runtime_error e) {}
}

int main() {

    test_basic_text();
    test_missing_letters();
    test_multiple_letters();
    test_text_with_space();
    test_text_with_punctuation();
    test_text_with_upper_lower_case_mix();
    test_throws_error_when_given_lowercase_options();
    test_throws_error_when_given_punctuation_options();

    cout << "Tests succeeded" << endl;
}

