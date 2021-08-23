#ifndef __TRIE__
#define __TRIE__

#include <string.h>
#include <iostream>
using std::string;


/
class TrieNode
{
public:
    TrieNode *next[26];
    bool isword;
    TrieNode()
    {
        memset(next, NULL, sizeof(next));
        isword = false;
    }
    ~TrieNode()
    {
        for (int i = 0; i < 26; i++)
            if (next[i])
                delete next[i];
    }
};

class Trie
{
    TrieNode *root;

public:
    /** Initialize your data structure here. */
    Trie() { root = new TrieNode(); }

    /** Inserts a word into the Trie. */
    void insert(string word)
    {
        TrieNode *p = root;
        for (int i = 0; i < (int) word.size(); i++) {
            if (p->next[word[i] - 'a'] == NULL)
                p->next[word[i] - 'a'] = new TrieNode();
            p = p->next[word[i] - 'a'];
        }
        p->isword = true;
    }

    /** Returns if the word is in the Trie. */
    bool search(string word)
    {
        TrieNode *p = root;
        for (int i = 0; i < (int) word.size() && p; i++) {
            p = p->next[word[i] - 'a'];
        }
        return p && p->isword;
    }

    /** Returns if there is any word in the Trie that starts with the given
     * prefix. */
    bool startsWith(string prefix)
    {
        TrieNode *p = root;
        for (int i = 0; i < (int) prefix.size() && p; i++) {
            p = p->next[prefix[i] - 'a'];
        }
        return p;
    }
    ~Trie() { delete root; }
};
#endif