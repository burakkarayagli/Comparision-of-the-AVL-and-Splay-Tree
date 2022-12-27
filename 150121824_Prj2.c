//
// Created by hazew on 24.12.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareCounterAVL = 0;
int rotateCounterAVL = 0;

int *compareCounterAVLptr = &compareCounterAVL;
int *rotateCounterAVLptr = &rotateCounterAVL;

int max(int a, int b) {
    return (a > b) ? a : b;
}

void increase(int *counter) {
    *counter += 1;
}

typedef struct Node{
    int data;
    int height;
    int frequency;
    struct Node *left;
    struct Node *right;
}Node;

Node* newNodeAVL(int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->height = 1;
    node->frequency = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

//Getting height of the node --AVL Tree
int getHeight(Node *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

//Calculating balance factor --AVL Tree
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
Node* leftRotateAVL(Node* node) {
    //node is a

    //If we call the leftRotate function, so this means there is a right child.
    Node* right = node->right;  //b
    //But we don't know if there is a left child of right.
    //If there is we attach the left of right to the node.
    //If there isn't it means it is NULL. So we attach the node's right to the null.
    //So we don't need to check if it is null or not.
    Node* leftOfRight = right->left; //T2

    //After this operation, the right node will be the root node
    //And the node will be the left node of the right node
    //Or node's right will be NULL
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
Node* rightRotateAVL(Node *node) {
    //node is a

    //Reverse of the left rotate. Same logic
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

//TODO Make rotate function checks which function to call and call it
//TODO Make another function for binary search tree insertion

Node* insertAVL(Node *node, int data) {
    //If tree is empty
    if (node == NULL) {
        return newNodeAVL(data);
    }
    //Binary search tree inserting
    if (data < node->data) {
        increase(compareCounterAVLptr);
        node->left = insertAVL(node->left, data);
    }
    else if (data > node->data) {
        increase(compareCounterAVLptr);
        node->right = insertAVL(node->right, data);
    }
    //If node already exists
    else {
        increase(compareCounterAVLptr);
        node->frequency += 1;
        return node;
    }

    //Updating height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    //Calculating balance factor
    //If factor is greater than 1, this means we should rotate to the right side. Left-Left or Left-Right
    //If it is less than -1, this means we should rotate to the left side. Right-Right or Right-Left
    int balanceFactor = getBalanceFactor(node);


    //Left Left node=x
    /*

             x
            /
           y
          /
         z

    */

    if (balanceFactor > 1 && data < node->left->data) {
        increase(rotateCounterAVLptr);
        return rightRotateAVL(node);
    }

    //Right Right node=x
    /*

        x
         \
          y
           \
            z

     */
    if (balanceFactor < -1 && data > node->right->data) {
        increase(rotateCounterAVLptr);
        return leftRotateAVL(node);
    }

    //Left Right node=x
    /*

        x
       /
      y
       \
        z

     */
    if (balanceFactor > 1 && data > node->left->data) {
        node->left = leftRotateAVL(node->left);increase(rotateCounterAVLptr);
        increase(rotateCounterAVLptr);
        return rightRotateAVL(node);
    }

    //Right Left node=x
    /*

           x
            \
             y
            /
           z


     */
    if (balanceFactor < -1 && data < node->right->data) {
        node->right = rightRotateAVL(node->right);increase(rotateCounterAVLptr);
        increase(rotateCounterAVLptr);
        return leftRotateAVL(node);
    }

    return node;
}

//Preorder traversal
void preorder(Node *node) {
    if (node != NULL) {
        printf("%d ", node->data);
        preorder(node->left);
        preorder(node->right);
    }
}





int main() {
    printf("Program started\n");
    //File reading
    FILE *file;
    file = fopen("../input1.txt", "r");
    if (file == NULL) {
        printf("File not found");
        return 0;
    }
    int data;
    fscanf(file, "%d", &data);
    Node *node = newNodeAVL(data);
    while (fscanf(file, "%d", &data) != EOF) {
        node = insertAVL(node, data);
    }
    fclose(file);
    //print the tree

    preorder(node);
    printf("\n");
    printf("Compare counter: %d\n", compareCounterAVL);
    printf("Rotate counter: %d\n", rotateCounterAVL);
    //Sum
    printf("Sum: %d\n", compareCounterAVL + rotateCounterAVL);
    printf("Program finished");

    return 0;
}