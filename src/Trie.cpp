#include <iostream>
#include <string>
#include <stdlib.h>

#include "vector/Vector.h"
#include "c/cprogramming.h"
#include "cpp/cpp_book.h"
#include "window/Window.h"
#include "ocl/ocl.h"

using namespace std;

const int TRIE_ENTRIES = (1 << sizeof(char) * 8) - 1; // 255

class TrieNode {
private:
    bool isWord;
    TrieNode *children[TRIE_ENTRIES];

    int getNodeIndexFromChar(char entry) {
        return entry;
    }

    int getNextNodeIndex(string word) {
        int index = this->getNodeIndexFromChar(word[0]);
        return index;
    }

public:
    TrieNode(bool isWord): isWord(isWord) {
        for (int i = 0; i < TRIE_ENTRIES; i++) {
            this->children[i] = nullptr;
        }
    }

    void addWord(string word) {
        if (word.length() == 0) {
            return;
        }

        int index = this->getNextNodeIndex(word);

        if (index < 0) {
            // TODO return error 
            return;
        }

        bool isLastCharacter = word.length() == 1;
        if (children[index] == nullptr) {
            children[index] = new TrieNode(isLastCharacter);
        }

        children[index]->addWord(word.substr(1));
    }

    bool isPresent(string word) {
        if (word.length() == 0) {
            return this->isWord;
        }

        int index = this->getNextNodeIndex(word);

        if (index < 0) {
            // TODO return error 
            return false;
        }

        if (children[index] == nullptr) {
            return false;
        }

        return children[index]->isPresent(word.substr(1));
    }

    void removeWord(string word) {
        if (word.length() == 0) {
            this->isWord = false;
            return;
        }

        int index = this->getNextNodeIndex(word);

        if (index < 0) {
            return;
        }

        if (children[index] == nullptr) {
            return;
        }

        children[index]->removeWord(word.substr(1));
    }
};


int trie_main() {

    TrieNode dictionary(false);
    dictionary.addWord("camel hand");
    dictionary.addWord("horse");

    cout << dictionary.isPresent("camel hand") << endl;
    cout << dictionary.isPresent("horse") << endl;
    cout << dictionary.isPresent("dog") << endl;

    dictionary.removeWord("dog");
    dictionary.removeWord("horse");

    cout << dictionary.isPresent("camel") << endl;
    cout << dictionary.isPresent("horse") << endl;
    cout << dictionary.isPresent("dog") << endl;
}

