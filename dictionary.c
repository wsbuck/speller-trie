// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node {
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary) {
    // Initialize trie
    root = calloc(1, sizeof(node));
    if (root == NULL) {
        return false;
    }
    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL) {
        unload();
        return false;
    }
    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    node *current_node = root;
    int current_char;
    while (fscanf(file, "%s", word) != EOF) {
        for (int i = 0; i < strlen(word); i++) {
            current_char = word[i] == '\'' ? 26 : (tolower(word[i]) - 'a');
            if (current_node->children[current_char]) {
                current_node = current_node->children[current_char];
            } else {
                node *n = calloc(1, sizeof(node));
                if (!n) {
                    return 1;
                }
                current_node->children[current_char] = n;
                current_node = n;
            }
        }
        current_node->is_word = true;
        current_node = root;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

unsigned int count_node_word(node *n) {
    int count = n->is_word ? 1 : 0;
    for (int i = 0; i < N; i++) {
        if (n->children[i]) {
            count = count + count_node_word(n->children[i]);
        }
    }
    return count;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void) {
    node *current_node = root;
    return count_node_word(root);
}

// Returns true if word is in dictionary else false
bool check(const char *word) {
    node *current_node = root;
    int current_char;
    for (int i = 0; i < strlen(word); i++) {
        current_char = word[i] == '\'' ? 26 : tolower(word[i]) - 'a';
        if (current_node->children[current_char]) {
            current_node = current_node->children[current_char];
        } else {
            return false;
        }
    }
    // printf("checking: %s, result: %s\n", word, current_node->is_word ? "true" : "false");
    return current_node->is_word;
}

bool free_node(node *n) {
    for (int i = 0; i < N; i++) {
        if (n->children[i]) {
            free_node(n->children[i]);
        }
    }
    free(n);
    return true;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void) {
    bool result = free_node(root);
    return result;
}
