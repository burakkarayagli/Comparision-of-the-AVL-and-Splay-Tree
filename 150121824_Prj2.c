//
// Created by hazew on 24.12.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int max(int a, int b) {
    return (a > b) ? a : b;
}

typedef struct Node {
    int data;
    int height;
    struct Node *left;
    struct Node *right;
}Node;

Node* newNode(int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int getHeight(Node *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}


int getBalanceFactor(Node *node) {
    if (node == NULL) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

/*

  a                                b
 /  \                            /   \
T1   b     Left Rotate(a)       a      c
    /  \   - - - - - - - ->    / \    / \
   T2   c                     T1  T2 T3  T4
       / \
     T3  T4

 */

//Rotation function
Node* leftRotate(Node* node) {
    //node is a
    Node* right = node->right;  //b
    Node* leftOfRight = right->left; //T2

    //After this operation, the right node will be the root node
    //And the node will be the left node of the right node
    right->left = node;
    node->right = leftOfRight;

    //Updating the height of the nodes
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    right->height = 1 + max(getHeight(right->left), getHeight(right->right));

    return right;
}
/*
         a                                      b
        / \                                   /   \
       b   T4      Right Rotate (a)          c      a
      / \          - - - - - - - - ->      /  \    /  \
     c   T3                               T1  T2  T3  T4
    / \
  T1   T2

*/

//Rotation function
Node* rightRotate(Node *node) {
    //node is a
    Node *left = node->left; //b
    Node *rightOfLeft = left->right; //T3

    //After this operation, the left node will be the root node
    //And the node will be the right node of the left node
    left->right = node;
    node->left = rightOfLeft;


    //Updating the height of the nodes
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    left->height = 1 + max(getHeight(left->left), getHeight(left->right));

    return left;
}





int main() {
    printf("Hello, World!");
    return 0;
}