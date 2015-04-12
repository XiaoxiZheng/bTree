#ifndef BTREENODE_H_INCLUDED
#define BTREENODE_H_INCLUDED
#include <string>
//#include"bTree.h"
using namespace std;

class BTreeNode{// A BTree node structure I created to assist the functionally of my bTree program
    string *keys;  // An array of keys
    string *values;
    int t;      // Minimum degree of tree--the range for number of keys)
    BTreeNode **ChildP; // An array of child pointers
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf-if left&&right child is NULL
public:
    BTreeNode(int _t, bool _leaf);   // Constructor
    void traverse();// A function to traverse all nodes in a subtree rooted with this node
    bool find(string k,string *value);// a method to find a key in subtree rooted with this node, returns NULL if k is not present.
    void insertNonFull(string k); // A helper method to insert a new key in the subtree rooted with this node. With the assumption that the node is not full
    void splitChild(int i, BTreeNode *y);// A helper method to split the child y of this node. i is index of y in child array C[].Function is called when Child y is full

    void delete_key(string k);// A wrapper method to remove the key k in subtree rooted with this node.
    int findKey(string k);// A helper function to delete_key() that returns the index of the first key that is greater or equal to k
    void removeFromLeaf(int idx);// A helper method to assist delete_key(string k) so that the key present in idx-th position in this leaf node is removed
    void removeFromNonLeaf(int idx);// A helper method to remove the key present in idx-th position in a non-leaf node

    string getPred(int idx);// A function to get the predecessor of the key- where the key is present in the idx-th position in the node
    string getSucc(int idx);// A function to get the successor of the key- where the key is present in the idx-th position in the node
    void fill(int idx);// A method to fill up the child node present in the idx-th position in the ChildP[] array if that child has less than t-1 keys(Process of reshuffling)

    void borrowFromPrev(int idx);// A method to borrow a key from the C[idx-1]-th node and place it in C[idx]th node
    void borrowFromNext(int idx);// A function to borrow a key from the C[idx+1]-th node and place it in C[idx]th node

    void merge(int idx);// A function to merge idx-th child of the node with (idx+1)th child of the node

    friend class BTree; //    // Make BTree friend of the BtreeNode so that we can access private members of this class in BTree functions

};


#endif // BTREENODE_H_INCLUDED
