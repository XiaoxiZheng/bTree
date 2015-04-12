#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED
#include "bTreeNode.h"
#include <iostream>
#include <string>
class BTree{
    BTreeNode *root; // Pointer to root node
    int t;  // Min degree
    int NodeSize; //size of node = 2t
public:
    // Constructor (Initializes tree as empty)
    BTree(int _ns){
        root = NULL;
        NodeSize = _ns;
        if(NodeSize%2==0) t = NodeSize/2;
        else t = (NodeSize-1)/2;
    }
    void traverse(){
        if (root != NULL) root->traverse();
    }
    bool find(string k, string *value){// function to find a key in this tree
        if(root== NULL) {
            //cout<<"NULL found"<<endl;
            return false;
        }
        else{
            return root->find(k,value);
        }
    }
    // The main function that inserts a new key in this B-Tree
    void insert(string k){
    // If tree is empty
        if (root == NULL){
        root = new BTreeNode(t, true);// Allocate memory for root
        root->keys[0] = k;  // Insert key
        root->n = 1;  // Update number of keys in root
        }
        else // If tree is not empty{
            if (root->n == 2*t-1){// If root is full, then tree grows in height
                BTreeNode *s = new BTreeNode(t, false);// Allocate memory for new root
                s->ChildP[0] = root;// Make old root as child of new root
                s->splitChild(0, root);// Split the old root and move a key to the new root
                int i = 0; //create an variable to decide which of the two children is going to have new key base on the 'value' of k.
                if (s->keys[0] < k)
                    i++;
                    s->ChildP[i]->insertNonFull(k);
                    root = s;// Change the root
            }
            else  // If root is not full, call insertNonFull for root
                root->insertNonFull(k);
    }

    void delete_key(string k){ // The main function that removes a new key in the B-Tree
        if (!root){
            cout << "The tree is empty\n";
            return;
        }
        root->delete_key(k);// Call the remove function for root
        if (root->n==0){// If the root node has 0 keys
            BTreeNode *tmp = root;
            if (root->leaf)  //  if the root is a leaft(have no left and right children) set root as NULL
                root = NULL;
            else
                root = root->ChildP[0];//otherwise(if root is NOT a leaf), then make its first child as the new root
            delete tmp;// Free the old root
        }
        return;
    }
};

#endif // BTREE_H_INCLUDED
