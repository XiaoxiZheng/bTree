#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED
//#include "bTreeNode.h"
#include <iostream>
#include <string>

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
    ~BTreeNode();   //destructor
    void traverse();// A function to traverse all nodes in a subtree rooted with this node
    bool find(string k, string *value);// a method to find a key in subtree rooted with this node, returns NULL if k is not present.
    void insertNonFull(string k, string value); // A helper method to insert a new key in the subtree rooted with this node. With the assumption that the node is not full
    void splitChild(int i, BTreeNode *y);// A helper method to split the child y of this node. i is index of y in child array C[].Function is called when Child y is full

    bool delete_key(string k);// A wrapper method to remove the key k in subtree rooted with this node.
    int findKey(string k);// A helper function to delete_key() that returns the index of the first key that is greater or equal to k
    void removeFromLeaf(int idx);// A helper method to assist delete_key(string k) so that the key present in idx-th position in this leaf node is removed
    void removeFromNonLeaf(int idx);// A helper method to remove the key present in idx-th position in a non-leaf node

    string getPred(int idx);// A function to get the predecessor of the key- where the key is present in the idx-th position in the node
    string getSucc(int idx);// A function to get the successor of the key- where the key is present in the idx-th position in the node

    string getPreVal(int idx); //A method to get the predecessor of the value- where the value is present in the idx-th position in the node
    string getSuccVal(int idx);
    void fill(int idx);// A method to fill up the child node present in the idx-th position in the ChildP[] array if that child has less than t-1 keys(Process of reshuffling)

    void borrowFromPrev(int idx);// A method to borrow a key from the C[idx-1]-th node and place it in C[idx]th node
    void borrowFromNext(int idx);// A function to borrow a key from the C[idx+1]-th node and place it in C[idx]th node

    void merge(int idx);// A function to merge idx-th child of the node with (idx+1)th child of the node

    friend class bTree; //    // Make BTree friend of the BtreeNode so that we can access private members of this class in BTree functions

};

class bTree{
    BTreeNode *root; // Pointer to root node
    int t;  // Min degree
    int NodeSize; //size of node = 2t
public:
    // Constructor (Initializes tree as empty)
    bTree(int _ns){
        root = NULL;
        NodeSize = _ns;
        if(NodeSize%2==0) t = NodeSize/2;
        else t = (NodeSize-1)/2;
    }
    void toStr(){
        if (root != NULL) root->traverse();
    }
    bool find(string k, string *value){// function to find a key in this tree
        if(root==NULL){
            //cout<<"NULL found"<<endl;
            return false;
        }
        else{
            return root->find(k,value);
        }
    }
    // The main function that inserts a new key in this B-Tree
    void insert(string k,string value){
    // If tree is empty
        if (root == NULL){
            root = new BTreeNode(t, true);// Allocate memory for root
            root->keys[0] = k;  // Insert key
            root->values[0] = value;
            root->n = 1;  // Update number of keys in root
        }
        else{
            // If tree is not empty{
            if (root->n == (2*t)-1){// If root is full, then tree grows in height
                BTreeNode *s = new BTreeNode(t, false);// Allocate memory for new root
                s->ChildP[0] = root;// Make old root as child of new root
                s->splitChild(0, root);// Split the old root and move a key to the new root
                    int i = 0; //create an variable to decide which of the two children is going to have new key base on the 'value' of k.
                    if (s->keys[0] < k)
                        i++;
                        s->ChildP[i]->insertNonFull(k,value);
                        root = s;// Change the root
            }
            else {
                 root->insertNonFull(k,value);
            } // If root is not full, call insertNonFull for root
        }
    }

    bool delete_key(string k){ // The main function that removes a new key in the B-Tree
        bool deleted = false;
        if (root==NULL){
            //cout << "The tree is empty\n";
            return deleted;
        }
        deleted = root->delete_key(k);// Call the remove function for root

        if (root->n==0){// If the root node has 0 keys
            BTreeNode *tmp = root;
            if (root->leaf)  //  if the root is a leaf (have no left and right children) set root as NULL
                root = NULL;
            else
                root = root->ChildP[0];//otherwise(if root is NOT a leaf), then make its first child as the new root
            delete tmp;// Free the old root
        }
        return deleted;
    }
    ~bTree(){
        delete [] root;
    }
};
#endif // BTREE_H_INCLUDED
