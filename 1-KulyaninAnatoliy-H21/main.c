#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

//A Red-Black tree node structure
struct node
{
    int data;
    char color;
    struct node* left, * right, * parent;
};

// Utility function to init BlakRed tree
struct node** InitTree(void) {
    struct node** tree = malloc(sizeof(struct node*));
    if (tree) {
        *tree = NULL;
        return tree;
    }
    else perror("Malloc failed(");
}

// Left Rotation
void LeftRotate(struct node** root, struct node* x)
{
    if (!x || !x->right)
        return;
    //y stored pointer of right child of x
    struct node* y = x->right;

    //store y's left subtree's pointer as x's right child
    x->right = y->left;

    //update parent pointer of x's right
    if (x->right != NULL)
        x->right->parent = x;

    //update y's parent pointer
    y->parent = x->parent;

    // if x's parent is null make y as root of tree
    if (x->parent == NULL)
        (*root) = y;

    // store y at the place of x
    else if (x == x->parent->left)
        x->parent->left = y;
    else    x->parent->right = y;

    // make x as left child of y
    y->left = x;

    //update parent pointer of x
    x->parent = y;
}

// Right Rotation (Similar to LeftRotate)
void RightRotate(struct node** root, struct node* y)
{
    if (!y || !y->left)
        return;
    struct node* x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (x->parent == NULL)
        (*root) = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

// Utility function to fixup the Red-Black tree after standard BST insertion
void insertFixUp(struct node** root, struct node* z)
{
    // iterate until z is not the root and z's parent color is red
    while (z != *root && z != (*root)->left && z != (*root)->right && z->parent->color == 'R')
    {
        struct node* y;

        // Find uncle and store uncle in y
        if (z->parent && z->parent->parent && z->parent == z->parent->parent->left)
            y = z->parent->parent->right;
        else
            y = z->parent->parent->left;

        // If uncle is RED, do following
        // (i)  Change color of parent and uncle as BLACK
        // (ii) Change color of grandparent as RED
        // (iii) Move z to grandparent
        if (!y)
            z = z->parent->parent;
        else if (y->color == 'R')
        {
            y->color = 'B';
            z->parent->color = 'B';
            z->parent->parent->color = 'R';
            z = z->parent->parent;
        }

        // Uncle is BLACK, there are four cases (LL, LR, RL and RR)
        else
        {
            // Left-Left (LL) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Right Rotate Grandparent
            if (z->parent == z->parent->parent->left &&
                z == z->parent->left)
            {
                char ch = z->parent->color;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                RightRotate(root, z->parent->parent);
            }

            // Left-Right (LR) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Left Rotate Parent
            // (iii) Right Rotate Grand Parent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->left &&
                z == z->parent->right)
            {
                char ch = z->color;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root, z->parent);
                RightRotate(root, z->parent->parent);
            }

            // Right-Right (RR) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Left Rotate Grandparent
            if (z->parent && z->parent->parent &&
                z->parent == z->parent->parent->right &&
                z == z->parent->right)
            {
                char ch = z->parent->color;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root, z->parent->parent);
            }

            // Right-Left (RL) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Right Rotate Parent
            // (iii) Left Rotate Grand Parent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->right &&
                z == z->parent->left)
            {
                char ch = z->color;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                RightRotate(root, z->parent);
                LeftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = 'B'; //keep root always black
}

// Utility function to insert newly node in RedBlack tree
void Insert(struct node** root, int data)
{
    // Allocate memory for new node
    struct node* z = (struct node*)malloc(sizeof(struct node));
    if (z) perror("malloc failed(");
    z->data = data;
    z->left = z->right = z->parent = NULL;

    //if root is null make z as root
    if (*root == NULL)
    {
        z->color = 'B';
        (*root) = z;
    }
    else
    {
        struct node* y = NULL;
        struct node* x = (*root);

        // Follow standard BST insert steps to first insert the node
        while (x != NULL)
        {
            y = x;
            if (z->data == x->data) {
                free(z);
                return 0;
            }
            x = data < x->data ? x->left : x->right;
        }
        z->parent = y;
        if (z->data > y->data)
            y->right = z;
        else
            y->left = z;
        z->color = 'R';

        // call insertFixUp to fix reb-black tree's property if it
        // is voilated due to insertion.
        insertFixUp(root, z);
    }
}

// Utility function to fixup the Red-Black tree after deletion
void DeleteFixup(struct node** tree, struct node* node) {
    while (node != (* tree)->parent && node->color == 'B') {
        if (node->parent->right != NULL && node == node->parent->left) {
            struct node* w = node->parent->right;
            //Case 1
            if (w->color == 'R') {
                w->color = 'B';
                node->parent->color = 'R';
                LeftRotate(tree, node->parent);
                w = node->parent->right;
            }
            //Case 2
            if (w->left != NULL && w->right != NULL && w->left->color == 'B' && w->right->color == 'B') {
                w->color = 'R';
                node = node->parent;
            }
            else {
                //Case 3
                if (w->left != NULL && w->right != NULL && w->right->color == 'B') {
                    w->left->color = 'B';
                    w->color = 'R';
                    RightRotate(tree, w);
                    w = node->parent->right;
                }
                //Case 4
                if (w->right != NULL) {
                    w->color = node->parent->color;
                    node->parent->color = 'B';
                    w->right->color = 'B';
                    LeftRotate(tree, node->parent);
                }
                node = (* tree)->parent;
            }
            continue;
        }
        else if (node->parent->left != NULL) {
            struct node* w = node->parent->left;
            //Case 1
            if (w->color == 'R') {
                w->color = 'B';
                node->parent->color = 'R';
                RightRotate(tree, node->parent);
                w = node->parent->left;
            }
            //Case 2
            if (w->left != NULL && w->right != NULL && w->right->color == 'B' && w->left->color == 'B') {
                w->color = 'R';
                node = node->parent;
            }
            else {
                //Case 3
                if (w->left != NULL && w->right != NULL && w->left->color == 'B') {
                    w->right->color = 'B';
                    w->color = 'R';
                    LeftRotate(tree, w);
                    w = node->parent->left;
                }
                //Case 4
                if (w->left != NULL) {
                    w->color = node->parent->color;
                    node->parent->color = 'B';
                    w->left->color = 'B';
                    RightRotate(tree, node->parent);
                }
                node = (*tree)->parent;
            }
            continue;
        }
        break;
    }
    node->color = 'B';
}

// Utility function to delete node from RedBlack tree
void DeleteNode(struct node** tree_p, int data) {
    struct node* tree = *tree_p;
    struct node* cur = tree;
    struct node* del_node = NULL;

    while (cur != NULL)
        if (data == cur->data) {
            del_node = cur;
            break;
        }
        else
            cur = data < cur->data ? cur->left : cur->right;

    if (del_node == NULL)
        return;

    struct node* swap;

    if (del_node->left == NULL || del_node->right == NULL)
        swap = del_node;
    else {
        swap = del_node->right;

        while (swap->left != NULL)
            swap = swap->left;
    }

    struct node* swap_child;

    if (swap->left != NULL)
        swap_child = swap->left;
    else
        swap_child = swap->right;

    if (swap_child != NULL)
        swap_child->parent = swap->parent;

    if (swap->parent != NULL) {
        if (swap == swap->parent->left)
            swap->parent->left = swap_child;
        else
            swap->parent->right = swap_child;
    }
    else
        tree->parent = swap_child;

    if (swap != del_node)
        del_node->data = swap->data;

    if (swap->color == 'B' && swap_child != NULL)
        DeleteFixup(tree_p, swap_child);

    if (swap == tree) {
        *tree_p = swap_child;
    }

    if (swap != NULL)
        free(swap);
}

// Utility function to find node in RedBlack tree
void FindNode(struct node* tree, int data) {
    struct node* current = tree;
    while (current != NULL) {
        if (data == current->data) {
            puts("yes");
            return;
        }
        else {
            current = data < current->data ? current->left : current->right;
        }
    }
    puts("no");
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
    if (node != NULL)
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
    struct node** tree = InitTree();
    while (scanf("%c %d", &command, &value) >= 1) {
        switch (command) {
        case 'a':
            Insert(tree, value);
            break;
        case 'r':
            DeleteNode(tree, value);
            break;
        case 'f':
            FindNode(*tree, value);
            break;
        case 'q':
            DeleteTree(*tree);
            return 0;
            break;
        }
    }
    
    return 0;
}