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
    struct node* y = x->right;
    x->right = y->left;

    if (x->right != NULL)
        x->right->parent = x;

    y->parent = x->parent;
    if (x->parent == NULL)
        (*root) = y;

    else if (x == x->parent->left)
        x->parent->left = y;
    else    x->parent->right = y;

    y->left = x;
    x->parent = y;
}

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

        if (z->parent && z->parent->parent && z->parent == z->parent->parent->left)
            y = z->parent->parent->right;
        else
            y = z->parent->parent->left;
        if (!y)
            z = z->parent->parent;
        else if (y->color == 'R')
        {
            y->color = 'B';
            z->parent->color = 'B';
            z->parent->parent->color = 'R';
            z = z->parent->parent;
        }

        else
        {
            if (z->parent == z->parent->parent->left &&
                z == z->parent->left)
            {
                char ch = z->parent->color;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                RightRotate(root, z->parent->parent);
            }
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->left &&
                z == z->parent->right)
            {
                char ch = z->color;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root, z->parent);
                RightRotate(root, z->parent->parent);
            }
            if (z->parent && z->parent->parent &&
                z->parent == z->parent->parent->right &&
                z == z->parent->right)
            {
                char ch = z->parent->color;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root, z->parent->parent);
            }
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
    struct node* z = (struct node*)malloc(sizeof(struct node));
    z->data = data;
    z->left = z->right = z->parent = NULL;
    if (*root == NULL)
    {
        z->color = 'B';
        (*root) = z;
    }
    else
    {
        struct node* y = NULL;
        struct node* x = (*root);
        while (x != NULL)
        {
            y = x;
            if (z->data < x->data)
                x = x->left;
            else
                x = x->right;
        }
        z->parent = y;
        if (z->data > y->data)
            y->right = z;
        else
            y->left = z;
        z->color = 'R';
        insertFixUp(root, z);
    }
}

// Utility function to fixup the Red-Black tree after deletion
void DeleteFixup(struct node** tree, struct node* node) {
    while (node != *tree && node->color == 'B') {
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
                node = *tree;
            }
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
                node = *tree;
            }
        }
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

int main()
{
    char command;
    int value;

    struct node** tree1 = InitTree(), **tree2 = InitTree();

    Insert(tree1, 1);
    Insert(tree2, 3);
    MergePass(tree1, *tree2, 2);
    FindNode(*tree1, 1);
    FindNode(*tree1, 2);
    FindNode(*tree1, 3);

    /*
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
            return 0;
            break;
        }
    }
    */
    return 0;
}