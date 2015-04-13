#include<iostream>
//#include"bTreeNode.h"
#include"bTree.h"
using namespace std;

BTreeNode::BTreeNode(int t1, bool leaf1){// Copy the given minimum degree and leaf property
    t = t1;
    leaf = leaf1;
    // Allocate memory for maximum number of possible keys and child pointers
    keys = new string[2*t-1];
    values = new string[2*t-1];
    ChildP = new BTreeNode *[2*t];
    // Initialize the number of keys as 0
    n = 0;
    //existed = true;
}

BTreeNode::~BTreeNode(){
    delete [] keys;
    delete [] values;
}
// A utility function that returns the index of the first key that is
// greater than or equal to k
int BTreeNode::findKey(string k)
{
    int idx=0;
    while (idx<n && keys[idx] < k)
        ++idx;
    return idx;
}

// A function to delete_key the key k from the sub-tree rooted with this node
bool BTreeNode::delete_key(string k){
    bool deleted = false;

    int idx = findKey(k);
    // The key to be removed is present in this node
    if (idx < n && !keys[idx].compare(k)){
        // If the node is a leaf node - removeFromLeaf is called
        // Otherwise, delete_keyFromNonLeaf function is called
        if (leaf){
            removeFromLeaf(idx);
        }
        else {
            removeFromNonLeaf(idx);
        }

        deleted = true;
    }
    else{
        // If this node is a leaf node, then the key is not present in tree
        if (leaf){
            //cout<<existed<<endl;
            //cout << "The key "<< k <<" does not exist in the tree\n";
            return deleted;
        }
        // The key to be removed is present in the sub-tree rooted with this node .
        // The lastC indicates whether the key is present in the sub-tree rooted with the last child of this node
            bool last = false;
            if(idx == n){
                last = true;
            }

            // If the child where the key is supposed to exist has less that t keys, fill that child
            if (ChildP[idx]->n < t){
                fill(idx);
            }
            // If the last child has been merged, it must have merged with the previous child and so we recurse on the (idx-1)th child. Else, we recurse on the
            // (idx)th child which now has at least t keys
            if (last && idx > n)
                deleted = ChildP[idx-1]->delete_key(k);
            else
                deleted = ChildP[idx]->delete_key(k);
        }
    return deleted;
}
// A function to remove the idx-th key from this node - which is a leaf node
void BTreeNode::removeFromLeaf (int idx){
    // Move all the keys after the idx-th pos one place backward
    for (int i=idx+1; i<n; ++i){
        keys[i-1] = keys[i];
        values[i-1] = values[i];
    }
    // Reduce the count of keys
    n--;
    return;
}
// A function to remove the idx-th key from this node - which is a non-leaf node
void BTreeNode::removeFromNonLeaf(int idx){
    string k = keys[idx];
    string value = values[idx];
    // If the child that precedes k (ChildP[idx]) has atleast t keys,
    // find the predecessor 'pred' of k in the subtree rooted at
    // ChildP[idx]. Replace k by pred. Recursively delete pred
    // in ChildP[idx]
    if (ChildP[idx]->n >= t){
        string pred = getPred(idx);
        string preVal = getPreVal(idx);
        keys[idx] = pred;
        values[idx] = preVal;
        ChildP[idx]->delete_key(pred);
    }
    // If the child ChildP[idx] has less that t keys, examine ChildP[idx+1].
    // If ChildP[idx+1] has atleast t keys, find the successor 'succ' of k in
    // the subtree rooted at ChildP[idx+1]
    // Replace k by succ
    // Recursively delete succ in ChildP[idx+1]
    else if  (ChildP[idx+1]->n >= t)
    {
        string succ = getSucc(idx);
        string succVal = getSuccVal(idx);
        keys[idx] = succ;
        ChildP[idx+1]->delete_key(succ);
    }

    // If both ChildP[idx] and ChildP[idx+1] has less that t keys,merge k and all of ChildP[idx+1]
    // into ChildP[idx]
    // Now ChildP[idx] contains 2t-1 keys
    // Free ChildP[idx+1] and recursively delete k from ChildP[idx]
    else
    {
        merge(idx);
        ChildP[idx]->delete_key(k);
    }
    return;
}

// A function to get predecessor of keys[idx]
string BTreeNode::getPred(int idx)
{
    // Keep moving to the right most node until we reach a leaf
    BTreeNode *cur=ChildP[idx];
    while (!cur->leaf)
        cur = cur->ChildP[cur->n];

    // Return the last key of the leaf
    return cur->keys[cur->n-1];
}
string BTreeNode::getPreVal(int idx){
    BTreeNode *cur=ChildP[idx];
    while (!cur->leaf)
        cur = cur->ChildP[cur->n];

    // Return the value of last key of the leaf
    return cur->values[cur->n-1];
}

string BTreeNode::getSucc(int idx)
{

    // Keep moving the left most node starting from ChildP[idx+1] until we reach a leaf
    BTreeNode *cur = ChildP[idx+1];
    while (!cur->leaf)
        cur = cur->ChildP[0];

    // Return the first key of the leaf
    return cur->keys[0];
}

string BTreeNode::getSuccVal(int idx){
    BTreeNode *cur = ChildP[idx+1];
    while (!cur->leaf)
        cur = cur->ChildP[0];

    // Return the first key of the leaf
    return cur->values[0];
}

// A function to fill child ChildP[idx] which has less than t-1 keys
void BTreeNode::fill(int idx){
    // If the previous child(ChildP[idx-1]) has more than t-1 keys, borrow a key from that child
    if (idx!=0 && ChildP[idx-1]->n>=t)
        borrowFromPrev(idx);
    // If the next child(ChildP[idx+1]) has more than t-1 keys, borrow a key from that child
    else if (idx!=n && ChildP[idx+1]->n>=t)
        borrowFromNext(idx);
    // Merge ChildP[idx] with its sibling
    // If ChildP[idx] is the last child, merge it with with its previous sibling
    // Otherwise merge it with its next sibling
    else
    {
        if (idx != n)
            merge(idx);
        else
            merge(idx-1);
    }
    return;
}
// A function to borrow a key from ChildP[idx-1] and insert it
// into ChildP[idx]
void BTreeNode::borrowFromPrev(int idx){
    BTreeNode *child=ChildP[idx];
    BTreeNode *sibling=ChildP[idx-1];
    // The last key from ChildP[idx-1] goes up to the parent and key[idx-1]
    // from parent is inserted as the first key in ChildP[idx]. Thus, the  loses
    // sibling one key and child gains one key

    // Moving all key in ChildP[idx] one step ahead
    for (int i=child->n-1; i>=0; --i){
        child->keys[i+1] = child->keys[i];
        child->values[i+1] = child->values[i];

    }
    // If ChildP[idx] is not a leaf, move all its child pointers one step ahead
    if (!child->leaf){
        for(int i=child->n; i>=0; --i)
            child->ChildP[i+1] = child->ChildP[i];
    }
    // Setting child's first key equal to keys[idx-1] from the current node
    child->keys[0] = keys[idx-1];
    child->values[0] = values[idx-1];
    // Moving sibling's last child as ChildP[idx]'s first child
    if (!leaf)
        child->ChildP[0] = sibling->ChildP[sibling->n];
    // Moving the key from the sibling to the parent
    // This reduces the number of keys in the sibling
    keys[idx-1] = sibling->keys[sibling->n-1];

    child->n += 1;
    sibling->n -= 1;

    return;
}

// A function to borrow a key from the ChildP[idx+1] and place
// it in ChildP[idx]
void BTreeNode::borrowFromNext(int idx){
    BTreeNode *child=ChildP[idx];
    BTreeNode *sibling=ChildP[idx+1];
    // keys[idx] is inserted as the last key in ChildP[idx]
    child->keys[(child->n)] = keys[idx];
    child->values[child->n] = values[idx];

    // Sibling's first child is inserted as the last child into ChildP[idx]
    if (!(child->leaf))
        child->ChildP[(child->n)+1] = sibling->ChildP[0];

    //The first key from sibling is inserted into keys[idx]
    keys[idx] = sibling->keys[0];
    values[idx] = sibling->values[0];
    // Moving all keys in sibling one step behind
    for (int i=1; i<sibling->n; ++i){
        sibling->keys[i-1] = sibling->keys[i];
        sibling->values[i-1] = sibling->values[i];
    }
    // Moving the child pointers one step behind
    if (!sibling->leaf)
    {
        for(int i=1; i<=sibling->n; ++i)
            sibling->ChildP[i-1] = sibling->ChildP[i];
    }
    // Increasing and decreasing the key count of ChildP[idx] and ChildP[idx+1]
    // respectively
    child->n += 1;
    sibling->n -= 1;

    return;
}

// A function to merge ChildP[idx] with ChildP[idx+1]
// ChildP[idx+1] is freed after merging
void BTreeNode::merge(int idx){
    BTreeNode *child = ChildP[idx];
    BTreeNode *sibling = ChildP[idx+1];

    // Pulling a key from the current node and inserting it into (t-1)th position of ChildP[idx]
    child->keys[t-1] = keys[idx];
    child->values[t-1] = values[idx];

    // Copying the keys from ChildP[idx+1] to ChildP[idx] at the end
    for (int i=0; i<sibling->n; ++i)
        child->keys[i+t] = sibling->keys[i];

    // Copying the child pointers from ChildP[idx+1] to ChildP[idx]
    if (!child->leaf)
    {
        for(int i=0; i<=sibling->n; ++i)
            child->ChildP[i+t] = sibling->ChildP[i];
    }

    // Moving all keys after idx in the current node one step before -
    // to fill the gap created by moving keys[idx] to ChildP[idx]
    for (int i=idx+1; i<n; ++i){
        keys[i-1] = keys[i];
        values[i-1] =values[i];
    }

    // Moving the child pointers after (idx+1) in the current node one
    // step before
    for (int i=idx+2; i<=n; ++i)
        ChildP[i-1] = ChildP[i];

    // Updating the key count of child and the current node
    child->n += sibling->n+1;
    n--;

    // Freeing the memory occupied by sibling
    delete(sibling);
    return;
}


// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
void BTreeNode::insertNonFull(string k, string value){
    // Initialize index as index of rightmost element
    int i = n-1;
    // If this is a leaf node
    if (leaf){
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && keys[i] > k)
        {
            keys[i+1] = keys[i];
            values[i+1] = values[i];
            i--;
        }
        // Insert the new key at found location
        keys[i+1] = k;
        values[i+1] = value;
        n = n+1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keys[i] > k)
            i--;

        // See if the found child is full
        if (ChildP[i+1]->n == 2*t-1)
        {
            // If the child is full, then split it
            splitChild(i+1, ChildP[i+1]);
            // After split, the middle key of ChildP[i] goes up and
            // ChildP[i] is splitted into two.  See which of the two
            // is going to have the new key
            if (keys[i+1] < k)
                i++;
        }
        ChildP[i+1]->insertNonFull(k,value);
    }
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
void BTreeNode::splitChild(int i, BTreeNode *y){
    BTreeNode *z = new BTreeNode(y->t, y->leaf);// Create a new node which is going to store (t-1) keys of y
    z->n = t - 1;
    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++){
        z->keys[j] = y->keys[j+t];
        z->values[j] = y->values[j+t];
    }
    // Copy the last t children of y to z
    if (y->leaf == false){
        for (int j = 0; j < t; j++)
            z->ChildP[j] = y->ChildP[j+t];
    }
    // Reduce the number of keys in y
    y->n = t - 1;
    // Since this node is going to have a new child, create space of new child
    for (int j = n; j >= i+1; j--)
        ChildP[j+1] = ChildP[j];

    ChildP[i+1] = z;// Link the new child to this node

    // A key of y will move to this node. Find location of
    // new key and move all greater keys one space ahead
    for (int j = n-1; j >= i; j--){
        keys[j+1] = keys[j];
        values[j+1] = values[j];
    }
    // Copy the middle key of y to this node
    keys[i] = y->keys[t-1];
    values[i] = y->values[t-1];
    // Increment count of keys in this node
    n = n + 1;
}

// Function to traverse all nodes in a subtree rooted with this node
void BTreeNode::traverse(){
    // There are n keys and n+1 children, travers through n keys
    // and first n children
    int i;
    for (i = 0; i < n; i++){
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child ChildP[i].
        if (leaf == false)
            ChildP[i]->traverse();
       // cout << " " << keys[i]<<"\t" <<values[i]<<endl;
        cout<< keys[i]<<endl;
    }

    // Print the subtree rooted with last child
    if (leaf == false)
        ChildP[i]->traverse();
}

// Function to find key k in subtree rooted with this node
bool BTreeNode::find(string k,string *value){
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < n && k > keys[i]){
            i++;
    }
        // If the found key is equal to k, return this node
    if (i!= n && !keys[i].compare(k)){
        *value=values[i];
        return true;
    }
        // If key is not found here and this is a leaf node
    if (leaf == true){
        return false;
    }
    // Go to the appropriate child
    return ChildP[i]->find(k,value);
}
