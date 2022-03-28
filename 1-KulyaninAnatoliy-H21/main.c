#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#define BLACK 'B'
#define RED 'R'


//A Red-Black tree node structure
struct node
{
    int data;
    char color;
    struct node* left, * right, * parent;
};

static struct node* NIL = NULL;

// Utility function to init BlakRed tree
void InitTree(struct node** tree) {
    if (NIL == NULL) {
        NIL = (struct node*)malloc(sizeof(struct node));
        if (NIL == NULL) perror("Malloc failed(");
        NIL->right = NIL;
        NIL->left = NIL;
        NIL->parent = NIL;
        NIL->data = 0;
    }
    *tree = NIL;
}

// Right Rotation
void RightRotate(struct node** root, struct node* y)
{
    struct node* x = y->left;
    y->left = x->right;

    if (x->right != NIL)
        x->right->parent = y;

    if (x != NIL)
        x->parent = y->parent;

    if (y->parent != NIL)
    {
        if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    }
    else {
        (*root) = x;
    }

    x->right = y;

    if (y != NIL)
        y->parent = x;
}

// Left Rotation (Similar to RightRotate)
void LeftRotate(struct node** root, struct node* y)
{
    struct node* x = y->right;
    y->right = x->left;

    if (x->left != NIL)
        x->left->parent = y;

    if (x != NIL)
        x->parent = y->parent;

    if (y->parent != NIL)
    {
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
    }
    else {
        (*root) = x;
    }

    x->left = y;

    if (y != NIL)
        y->parent = x;
}

// Utility function to fixup the Red-Black tree after standard BST insertion
void insertFixUp(struct node** root, struct node* z)
{
    if (!(*root) || !z) return;
    struct node* y = z;
    while (y != *root && y->parent->color == RED) {

        if (y->parent == y->parent->parent->left) {
            struct node* x = y->parent->parent->right;
            if (x->color == RED) {
                y->parent->color = BLACK;
                x->color = BLACK;
                y->parent->parent->color = RED;
                y = y->parent->parent;
            }
            else {
                if (y == y->parent->right) {
                    y = y->parent;
                    LeftRotate(root, y);
                }
                y->parent->color = BLACK;
                y->parent->parent->color = RED;
                RightRotate(root, y->parent->parent);
            }
        }
        else {
            struct node* x = y->parent->parent->left;

            if (x->color == RED)
            {
                y->parent->color = BLACK;
                x->color = BLACK;
                y->parent->parent->color = RED;
                y = y->parent->parent;
            }
            else {
                if (y == y->parent->left) {
                    y = y->parent;
                    RightRotate(root, y);
                }
                y->parent->color = BLACK;
                y->parent->parent->color = RED;
                LeftRotate(root, y->parent->parent);
            }
        }
    }
    (*root)->color = BLACK; //keep root always black
}

// Utility function to insert newly node in RedBlack tree
void Insert(struct node** root, int data)
{
    struct node* y = NIL;
    struct node* x = (*root);

    // Follow standard BST insert steps to first insert the node
    while (x != NIL)
    {
        y = x;
        if (data == x->data) {
            return;
        }
        x = data < x->data ? x->left : x->right;
    }

    // Allocate memory for new node
    struct node* z = (struct node*)malloc(sizeof(struct node));
    if (z == NULL) perror("malloc failed(");
    z->data = data;
    z->left = z->right = NIL;
    z->parent = y;
    if (y != NIL)
        if (data < y->data)
            y->left = z;
        else
            y->right = z;
    else (*root) = z;
    
    z->color = RED;

    // call insertFixUp to fix reb-black tree's property if it
    // is voilated due to insertion.
    insertFixUp(root, z);
}

// Utility function to fixup the Red-Black tree after deletion
void DeleteFixup(struct node** tree, struct node* node) {
    while (node != (*tree)->parent && node->color == BLACK) {
        if (node->parent->right != NIL && node == node->parent->left) {
            struct node* w = node->parent->right;
            //Case 1
            if (w->color == RED) {
                w->color = BLACK;
                node->parent->color = RED;
                LeftRotate(tree, node->parent);
                w = node->parent->right;
            }
            //Case 2
            if (w->left != NIL && w->right != NIL && w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                node = node->parent;
            }
            else {
                //Case 3
                if (w->left != NIL && w->right != NIL && w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(tree, w);
                    w = node->parent->right;
                }
                //Case 4
                if (w->right != NIL) {
                    w->color = node->parent->color;
                    node->parent->color = BLACK;
                    w->right->color = BLACK;
                    LeftRotate(tree, node->parent);
                }
                node = (*tree)->parent;
            }
            continue;
        }
        else if (node->parent->left != NIL) {
            struct node* w = node->parent->left;
            //Case 1
            if (w->color == RED) {
                w->color = BLACK;
                node->parent->color = RED;
                RightRotate(tree, node->parent);
                w = node->parent->left;
            }
            //Case 2
            if (w->left != NIL && w->right != NIL && w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                node = node->parent;
            }
            else {
                //Case 3
                if (w->left != NIL && w->right != NIL && w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(tree, w);
                    w = node->parent->left;
                }
                //Case 4
                if (w->left != NIL) {
                    w->color = node->parent->color;
                    node->parent->color = BLACK;
                    w->left->color = BLACK;
                    RightRotate(tree, node->parent);
                }
                node = (*tree)->parent;
            }
            continue;
        }
        break;
    }
    node->color = BLACK;
}

// Utility function to delete node from RedBlack tree
void DeleteNode(struct node** tree_p, int data) {
    struct node* tree = *tree_p;
    struct node* cur = tree;
    struct node* del_node = NIL;

    while (cur != NIL)
        if (data == cur->data)
        {
            del_node = cur;
            break;
        }
        else
            cur = data < cur->data ? cur->left : cur->right;

    if (del_node == NIL)
        return;

    struct node* swap;

    if (del_node->left == NIL || del_node->right == NIL)
        swap = del_node;
    else
    {
        swap = del_node->right;

        while (swap->left != NIL)
            swap = swap->left;
    }

    struct node* swap_child;

    if (swap->left != NIL)
        swap_child = swap->left;
    else
        swap_child = swap->right;

    if (swap_child != NIL)
        swap_child->parent = swap->parent;

    if (swap->parent != NIL)
    {
        if (swap == swap->parent->left)
            swap->parent->left = swap_child;
        else
            swap->parent->right = swap_child;
    }
    else
        tree->parent = swap_child;

    if (swap != del_node)
        del_node->data = swap->data;

    if (swap->color == BLACK && swap_child != NIL)
        DeleteFixup(tree_p, swap_child);

    if (swap == tree)
    {
        *tree_p = swap_child;
    }

    if (swap != NIL)
        free(swap);
}

// Utility function to find node in RedBlack tree
int FindNode(struct node* tree, int data) {
    struct node* current = tree;
    while (current != NIL) {
        if (current->data > data) current = current->left;
        else
            if (current->data < data) current = current->right;
            else return 1;
    }
    return 0;
}

// Utility function to merge 2 trees
void Merge(struct node** r, struct node* l) {
    if (l->left) Merge(r, l->left);
    Insert(r, l->data);
    if (l->right) Merge(r, l->right);
}

// Utility function to pass ex
void MergePass(struct node** r, struct node* l, int x) {
    Insert(r, x);
    Merge(r, l);
}

// Utility function to delete tree
void DeleteTree(struct node* node)
{
    if (node != NIL)
    {
        DeleteTree(node->left);
        DeleteTree(node->right);
        free(node);
    }
}

int main()
{
    char command;
    int value;
    struct node* tree = NULL;
    InitTree(&tree);
    while (scanf("%c %d", &command, &value) >= 1) {
        switch (command) {
        case 'a':
            Insert(&tree, value);
            break;
        case 'r':
            DeleteNode(&tree, value);
            break;
        case 'f':
            if (FindNode(tree, value)) puts("yes");
            else puts("no");
            break;
        case 'q':
            DeleteTree(tree);
            return 0;
            break;
        }
    }

    return 0;
}