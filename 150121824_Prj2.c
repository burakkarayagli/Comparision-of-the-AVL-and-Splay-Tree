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

Node* newNode(int data) {
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
Node* leftRotate(Node* node) {
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
Node* rightRotate(Node *node) {
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
        return newNode(data);
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
        return rightRotate(node);
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
        return leftRotate(node);
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
        node->left = leftRotate(node->left);increase(rotateCounterAVLptr);
        increase(rotateCounterAVLptr);
        return rightRotate(node);
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
        node->right = rightRotate(node->right);increase(rotateCounterAVLptr);
        increase(rotateCounterAVLptr);
        return leftRotate(node);
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


//Splay Tree
/*
 * 1- target is the root, then return target
 * 2- target is the left child of root and target does not have grand parent, RightRotate(root)
 * 3- target is the right child of root and target does not have grand parent, LeftRotate(root)
 * 4- target is the left of left of root so first RotateRight(root), the new root is the root's left child, then RotateRight(root) again
 * 5- target is the right of right of root so first LeftRotate(root), the new root is the root's right child, then RotateLeft(root) again
 * 6- target is right of left of root so first LeftRotate(root->left), after rotation root's left child is target, then RotateRight(root)
 * 7- target is the
 */

Node* splay(Node *root, int data) {

    //Target is root - CASE 1
    if (root->data == data) {
        return root;
    }

    //Target is in the left of tree - CASE 2,4,6
    if (data < root->data) {

        //If target is left of the root - CASE 2
        if (data == root->left->data) {
            return rightRotate(root);
        }
        //If target is left left - CASE 4
        else if (data == root->left->left->data) {
            return rightRotate(root);
        }
        //left right - CASE 6
        else if (data == root->left->right->data) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        else {
            splay(root->left->left, data);
        }
    }
    //Target is in the right of tree - CASE 3,5,7
    else {
        //If target is right of the root - CASE 3
        if (data == root->right->data) {
            return leftRotate(root);
        }
        //If target is right right - CASE 5
        else if (data == root->right->right->data) {
            return leftRotate(root);
        }
        //right left - CASE 7
        else if (data == root->right->left->data) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        else {
            splay(root->right->right, data);
        }
    }
}

//Binary Search Tree insertion
Node* insertBST(Node *node, int data) {
    //If tree is empty
    if (node == NULL) {
        return newNode(data);
    }
    //Binary search tree inserting
    if (data < node->data) {
        node->left = insertBST(node->left, data);
    }
    else if (data > node->data) {
        node->right = insertBST(node->right, data);
    }
    //If node already exists
    else {
        node->frequency += 1;
        return node;
    }
    return node;
}

Node* insertSplay(Node *node, int data) {
    insertBST(node, data);
    return splay(node, data);
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
    Node *node = newNode(data);
    Node *node2 = newNode(data);
    printf("CHECKPOINT 1\n");
    while (fscanf(file, "%d", &data) != EOF) {
        node = insertAVL(node, data);
        node2 = insertSplay(node2, data);
    }
    fclose(file);
    //print the tree

    preorder(node);
    printf("\n");
    //printf("Compare counter: %d\n", compareCounterAVL);
    //printf("Rotate counter: %d\n", rotateCounterAVL);
    //Sum
    printf("AVL: %d\n", compareCounterAVL + rotateCounterAVL);

    printf("Program finished");

    return 0;
}