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

bool search(TrieNode* node, const string &pattern) {
    size_t i = 0;
    while (i < pattern.length()) {
        int index = charToIndex(pattern[i]);
        if (!node->children[index])
            return false;
        node = node->children[index];
        i++;
    }
    int endIndex = charToIndex('$');
    return node->children[endIndex] != nullptr &&
           node->children[endIndex]->isEndOfWord;
}

bool deleteHelper(TrieNode* node, const string &word, size_t depth) {
    if (!node) return false;

    if (depth == word.length()) {
        int endIndex = charToIndex('$');
        TrieNode* endNode = node->children[endIndex];
        if (!endNode) return false;

        endNode->isEndOfWord = false;

        bool empty = true;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            if (endNode->children[i]) empty = false;
        if (empty) {
            delete endNode;
            node->children[endIndex] = nullptr;
        }

        for (int i = 0; i < ALPHABET_SIZE; i++)
            if (node->children[i]) return false;
        return !node->isEndOfWord;
    }

    int index = charToIndex(word[depth]);
    if (!node->children[index]) return false;

    bool shouldDeleteChild = deleteHelper(node->children[index], word, depth + 1);
    if (shouldDeleteChild) {
        delete node->children[index];
        node->children[index] = nullptr;

        for (int i = 0; i < ALPHABET_SIZE; i++)
            if (node->children[i]) return false;
        return !node->isEndOfWord;
    }
    return false;
}

void deleteSuffix(TrieNode* root, const string &word) {
    deleteHelper(root, word, 0);
}

int main() {
    string text = "banana";
    TrieNode* root = new TrieNode();

    buildSuffixTrie(root, text);

    vector<string> tests;
    tests.push_back("ana");
    tests.push_back("banana");
    tests.push_back("nan");
    tests.push_back("nana");
    tests.push_back("apple");

    for (int i = 0; i < tests.size(); i++) {
        cout << "Searching for '" << tests[i] << ":' it was ";
        if (search(root, tests[i]))
            cout << "found" << endl;
        else
            cout << "not found" << endl;
    }

    cout << "\n After deleting 'ana'...\n";
    deleteSuffix(root, "ana");

    cout << "Searching 'ana' again: ";
    if (search(root, "ana"))
        cout << "found" << endl;
    else
        cout << "not found" << endl;

    return 0;
}