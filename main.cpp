#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define ALPHABET_SIZE 27

int charToIndex(char c) {
    return (c == '$') ? 26 : c - 'a';
}

struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            children[i] = nullptr;
    }
};

void insert(TrieNode* node, const string &word) {
    size_t i = 0;
    while (i < word.length()) {
        char c = word[i];
        int index = charToIndex(c);
        if (!node->children[index])
            node->children[index] = new TrieNode();
        node = node->children[index];
        i++;
    }
    int endIndex = charToIndex('$');
    if (!node->children[endIndex])
        node->children[endIndex] = new TrieNode();
    node->children[endIndex]->isEndOfWord = true;
}

void buildSuffixTrie(TrieNode* root, const string &text) {
    size_t i = 0;
    while (i < text.length()) {
        insert(root, text.substr(i));
        i++;
    }
}