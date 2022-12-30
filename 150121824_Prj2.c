//
// Created by hazew on 24.12.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT 10

int compareCounterAVL = 0;
int rotateCounterAVL = 0;

int *compareCounterAVLptr = &compareCounterAVL;
int *rotateCounterAVLptr = &rotateCounterAVL;

int compareCounterSPLAY = 0;
int rotateCounterSPLAY = 0;

int *compareCounterSPLAYptr = &compareCounterSPLAY;
int *rotateCounterSPLAYptr = &rotateCounterSPLAY;

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

Node *rootOfSplayTree;

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


Node* splay(Node *root, int target) {



    if (root->data == target) {
        return root;
    }

    //target sag taraftaysa
    if (root->data < target) {

        //target sagin sol tarafindaysa
        if (target < root->right->data && target != root->right->left->data) {
            root->right = splay(root->right, target);
            return root;
        }

        //target sagin sag tarafindaysa
        if (root->right->data < target && target != root->right->right->data) {
            root->right = splay(root->right, target);
            return root;
        }
        //target sag cocuk
        if (target == root->right->data && root == rootOfSplayTree) {
            root = leftRotate(root);
            return root;
        }


        //target sagin soluysa
        if (root->right->left != NULL && target == root->right->left->data) {
            root->right = rightRotate(root->right);
            root = leftRotate(root);
            return root;
        }

        //target sagin sagiysa
        if (root->right->right != NULL && target == root->right->right->data) {
            root = leftRotate(root);
            root = leftRotate(root);
            return root;
        }

    }
    else {
        //target solun sol tarafindaysa
        if (target < root->left->data && target != root->left->left->data) {
            root->left = splay(root->left, target);
            return root;
        }

        //target solun sag tarafindaysa
        if (root->left->data < target && target != root->left->right->data) {
            root->left = splay(root->left, target);
            return root;
        }
        //target sol cocuk
        if (target == root->left->data && root == rootOfSplayTree) {
            root = rightRotate(root);
            return root;
        }

        //target solun soluysa
        if (root->left->left != NULL && target == root->left->left->data) {
            root = rightRotate(root);
            root = rightRotate(root);
            return root;
        }

        //target solun sagiysa
        if (target == root->left->right->data) {
            root->left = leftRotate(root->left);
            root = rightRotate(root);
            return root;
        }



    }

}


//Preorder traversal
void preorder(Node *node) {
    if (node != NULL) {
        printf("%d ", node->data);
        preorder(node->left);
        preorder(node->right);
    }
}

void print2DUtil(struct Node* root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->data);

    // Process left child
    print2DUtil(root->left, space);
}

// Wrapper over print2DUtil()
void print2D(struct Node* root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}




//Binary Search Tree insertion
Node* insertBST(Node *node, int data) {
    //If tree is empty
    if (node == NULL) {
        return newNode(data);
    }
    //Binary search tree inserting
    if (data < node->data) {
        increase(compareCounterSPLAYptr);
        node->left = insertBST(node->left, data);
    }
    else if (data > node->data) {
        increase(compareCounterSPLAYptr);
        node->right = insertBST(node->right, data);
    }
    //If node already exists
    else {
        increase(compareCounterSPLAYptr);
        node->frequency += 1;
        return node;
    }
    return node;
}

Node* insertSplay(Node *node, int data) {
    insertBST(node, data);
    return splay(node, data);
}

void printPreOrder(Node *node) {
    if (node == NULL) {
        return;
    }
    if (node->frequency > 1) {
        printf("%d(%d) ", node->data, node->frequency);
    }
    printPreOrder(node->left);
    printPreOrder(node->right);
}


int main() {
    //printf("Program started\n");
    //Node *deneme = newNode(215);
    //printf("%d", deneme->left->data);
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
    //printf("CHECKPOINT 1\n");

    rootOfSplayTree = node2;


    while (fscanf(file, "%d", &data) != EOF) {
        //printf("CHECKPOINT 2\n");
        node = insertAVL(node, data);
        node2 = insertSplay(node2, data);
        rootOfSplayTree = node2;

        //print2D(node2);
        //printf("======================\n");
    }
    fclose(file);
    //print the tree

    printf("AVL Tree:\n");
    preorder(node);
    printf("\n");
    //printf("Compare counter: %d\n", compareCounterAVL);
    //printf("Rotate counter: %d\n", rotateCounterAVL);
    //Sum
    printf("Total Cost: %d\n", compareCounterAVL + rotateCounterAVL);

    //printPreOrder(node);
    printf("\n");

    printf("Splay Tree:\n");
    preorder(node2);
    printf("\n");
    //printf("Compare counter: %d\n", compareCounterSPLAY);
    //printf("Rotate counter: %d\n", rotateCounterSPLAY);
    //Sum
    printf("Total Cost: %d\n", compareCounterSPLAY + rotateCounterSPLAY);
    //printf("Program finished");

    printf("===================================================================================\n");

    Node *node3 = newNode(10);
    insertBST(node3,5);
    insertBST(node3, 2);


    /*
    print2D(node3);
    printf("===================================================================================\n");
    node3 = rightRotate(node3);
    printf("-----%d-----\n",node3->data);


    print2D(node3);
    */
    return 0;
}