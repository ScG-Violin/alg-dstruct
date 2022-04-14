#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

typedef struct tree {
    int size;
    int key[3];
    struct tree* left;
    struct tree* mid;
    struct tree* right;
    struct tree* temp;
    struct tree* prev;

} tree;

char Find(int value, int* key, int size);

void Swap(int* x, int* y);

void Sort2(int* x, int* y);

void Sort3(int* x, int* y, int* z);

void SortKeys(int size, int* key);

tree* CreateNode(int value);

tree* CreateNode2(int value, tree* left, tree* mid, tree* right, tree* temp, tree* prev);

void AddKey(int value, tree* node);

void RemoveKey(int value, tree* node);

void BeNode2(int value, int* key, tree* node, tree* left, tree* mid);

char isLeaf(tree* node);

tree* Split(tree* item);

tree* AddNode(tree* node, int value);

tree* Search(tree* node, int value);

tree* FindMin(tree* node);

tree* Merge(tree* leaf);

tree* Redistribute(tree* leaf);

tree* Fix(tree* leaf);

tree* DeleteNode(tree* node, int value);

void DestroyNode(tree* t);

void PrintNode(tree* node);

int main(void) {
    char command;
    tree* t = NULL;
    int key = 0;
    while (scanf("%c", &command) >= 1) {
        if (command != 'p' && command != 'q') {
            scanf("%i", &key);
        }

        switch (command) {
        case 'a':
            t = AddNode(t, key);
            break;
        case 'r':
            t = DeleteNode(t, key);
            break;
        case 'f':
            if (Search(t, key)) {
                puts("yes");
            }
            else {
                puts("no");
            }

            break;
        case 'p':
            PrintNode(t);
            putchar('\n');
            break;
        case 'q':
            DestroyNode(t);
            return 0;
        }
    }
}

tree* CreateNode(int value) {
    tree* new_node = (tree*)malloc(sizeof(tree));
    if (new_node) {
        new_node->left = NULL;
        new_node->mid = NULL;
        new_node->right = NULL;
        new_node->temp = NULL;
        new_node->prev = NULL;
        new_node->key[0] = value;
        new_node->size = 1;
        return new_node;
    }
    return NULL;
}

tree* CreateNode2(int value, tree* left, tree* mid, tree* right, tree* temp, tree* prev) {
    tree* new_node = (tree*)malloc(sizeof(tree));
    if (new_node) {
        new_node->left = left;
        new_node->mid = mid;
        new_node->right = right;
        new_node->temp = temp;
        new_node->prev = prev;
        new_node->key[0] = value;
        new_node->size = 1;
        return new_node;
    }
    return NULL;
}

char Find(int value, int* key, int size) {
    for (int i = 0; i < size; ++i) 
        if (key[i] == value) return 1;

    return 0;
}

void Swap(int* x, int* y) {
    int r = (*x);
    (*x) = (*y);
    (*y) = r;
}

void Sort2(int* x, int* y) {
    if (*x > *y) Swap(x, y);
}

void Sort3(int* x, int* y, int* z) {
    if (*x > *y)  Swap(x, y);
    if (*x > *z) Swap(x, z);
    if (*y > *z) Swap(y, z);
}

void SortKeys(int size, int* key) {
    if (size == 1) return;
    if (size == 2)  Sort2(&key[0], &key[1]);
    if (size == 3) Sort3(&key[0], &key[1], &key[2]);
}

void AddKey(int value, tree* node) {
    if (node == NULL) return;
    node->key[node->size] = value;
    node->size++;
    SortKeys(node->size, node->key);
}

void RemoveKey(int value, tree* node) {
    if (node == NULL) return;
    if (node->size >= 1 && node->key[0] == value) {
        node->key[0] = node->key[1];
        node->key[1] = node->key[2];
        node->size--;
    }
    else if (node->size == 2 && node->key[1] == value) {
        node->key[1] = node->key[2];
        node->size--;
    }
}

void BeNode2(int value, int* key, tree* node, tree* left, tree* mid) {
    key[0] = value;
    node->left = left;
    node->mid = mid;
    node->right = NULL;
    node->temp = NULL;
    node->prev = NULL;
    node->size = 1;
}

char isLeaf(tree* node) {
    if (node == NULL) return 0;
    if ((node->left == NULL) && (node->mid == NULL) && (node->right == NULL)) return 1;
    return 0;
}

tree* AddNode(tree* node, int value) {
    if (node == NULL)  return CreateNode(value);

    if (Search(node, value)) return Split(node);

    if (isLeaf(node)) AddKey(value, node);
    else 
        if (value <= node->key[0]) AddNode(node->left, value);
    else 
        if ((node->size == 1) || ((node->size == 2) && value <= node->key[1])) AddNode(node->mid, value);
    else  AddNode(node->right, value);

    return Split(node);
}

tree* FindMin(tree* node) {
    if (node == NULL) {
        return node;
    }

    if ((node->left) == NULL) {
        return node;
    }
    else {
        return FindMin(node->left);
    }
}

tree* DeleteNode(tree* node, int value) {
    if (node == NULL) {
        return NULL;
    }

    tree* item = Search(node, value);

    if (item == NULL) {
        return node;
    }

    tree* min = NULL;
    if (item->key[0] == value) {
        min = FindMin(item->mid);
    }
    else {
        min = FindMin(item->right);
    }

    if (min != NULL) {
        int* z = (value == item->key[0] ? &(item->key[0]) : &(item->key[1]));
        Swap(z, &min->key[0]);
        item = min;
    }

    RemoveKey(value, item);
    return Fix(item);
}

tree* Split(tree* item) {
    if (item == NULL) {
        return NULL;
    }

    if (item->size < 3) {
        return item;
    }

    tree* x = CreateNode2(item->key[0], item->left, item->mid, NULL, NULL, item->prev);
    tree* y = CreateNode2(item->key[2], item->right, item->temp, NULL, NULL, item->prev);

    if (x->left) {
        x->left->prev = x;
    }

    if (x->mid) {
        x->mid->prev = x;
    }

    if (y->left) {
        y->left->prev = y;
    }

    if (y->mid) {
        y->mid->prev = y;
    }

    if (item->prev != NULL) {
        AddKey(item->key[1], item->prev);

        if (item->prev->left == item) {
            item->prev->left = NULL;
        }
        else if (item->prev->mid == item) {
            item->prev->mid = NULL;
        }
        else if (item->prev->right == item) {
            item->prev->right = NULL;
        }

        if (item->prev->left == NULL) {
            item->prev->temp = item->prev->right;
            item->prev->right = item->prev->mid;
            item->prev->mid = y;
            item->prev->left = x;
        }
        else if (item->prev->mid == NULL) {
            item->prev->temp = item->prev->right;
            item->prev->right = y;
            item->prev->mid = x;
        }
        else {
            item->prev->temp = y;
            item->prev->right = x;
        }

        tree* tmp = item->prev;
        free(item);
        return tmp;
    }
    else {
        x->prev = item;
        y->prev = item;
        BeNode2(item->key[1], item->key, item, x, y);
        return item;
    }
}

tree* Search(tree* node, int value) {
    if (node == NULL) {
        return NULL;
    }

    if (Find(value, node->key, node->size)) {
        return node;
    }
    else if (value < node->key[0]) {
        return Search(node->left, value);
    }
    else if ((node->size == 2) && (value < node->key[1]) || (node->size == 1)) {
        return Search(node->mid, value);
    }
    else if (node->size == 2) {
        return Search(node->right, value);
    }
}

tree* Fix(tree* leaf) {
    if (leaf == NULL) {
        return NULL;
    }

    if (leaf->size == 0 && leaf->prev == NULL) {
        free(leaf);
        return NULL;
    }

    if (leaf->size != 0) {
        if (leaf->prev) {
            return Fix(leaf->prev);
        }
        else {
            return leaf;
        }
    }

    tree* prev = leaf->prev;
    if (prev->left->size == 2 || prev->mid->size == 2 || prev->size == 2) {
        leaf = Redistribute(leaf);
    }
    else if (prev->size == 2 && prev->right->size == 2) {
        leaf = Redistribute(leaf);
    }
    else {
        leaf = Merge(leaf);
    }

    return Fix(leaf);
}

tree* Merge(tree* leaf) {
    if (leaf == NULL) {
        return NULL;
    }

    tree* prev = leaf->prev;

    if (prev->left == leaf) {
        AddKey(prev->key[0], prev->mid);
        prev->mid->right = prev->mid->mid;
        prev->mid->mid = prev->mid->left;

        if (leaf->left != NULL) {
            prev->mid->left = leaf->left;
        }
        else if (leaf->mid != NULL) {
            prev->mid->left = leaf->mid;
        }

        if (prev->mid->left != NULL) {
            prev->mid->left->prev = prev->mid;
        }

        RemoveKey(prev->key[0], prev);
        free(prev->left);
        prev->left = NULL;
    }
    else if (prev->mid == leaf) {
        AddKey(prev->key[0], prev->left);

        if (leaf->left != NULL) {
            prev->left->right = leaf->left;
        }
        else if (leaf->mid != NULL) {
            prev->left->right = leaf->mid;
        }

        if (prev->left->right != NULL) {
            prev->left->right->prev = prev->left;
        }

        RemoveKey(prev->key[0], prev);
        free(prev->mid);
        prev->mid = NULL;
    }

    if (prev->prev == NULL) {
        tree* tmp = NULL;
        if (prev->left != NULL) {
            tmp = prev->left;
        }
        else {
            tmp = prev->mid;
        }

        tmp->prev = NULL;
        free(prev);
        return tmp;
    }
    return prev;
}

tree* Redistribute(tree* leaf) {
    if (leaf == NULL) {
        return NULL;
    }

    tree* prev = leaf->prev;
    tree* first = prev->left;
    tree* second = prev->mid;
    tree* third = prev->right;


    if ((prev->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2)) {
        if (first == leaf) {
            prev->left = prev->mid;
            prev->mid = prev->right;
            prev->right = NULL;
            AddKey(prev->key[0], prev->left);
            prev->left->right = prev->left->mid;
            prev->left->mid = prev->left->left;

            if (leaf->left != NULL) {
                prev->left->left = leaf->left;
            }
            else if (leaf->mid != NULL) {
                prev->left->left = leaf->mid;
            }

            if (prev->left->left != NULL) {
                prev->left->left->prev = prev->left;
            }

            RemoveKey(prev->key[0], prev);
        }
        else if (second == leaf) {
            AddKey(prev->key[0], first);
            RemoveKey(prev->key[0], prev);
            if (leaf->left != NULL) {
                first->right = leaf->left;
            }
            else if (leaf->mid != NULL) {
                first->right = leaf->mid;
            }

            if (first->right != NULL) {
                first->right->prev = first;
            }

            prev->mid = prev->right;
            prev->right = NULL;

            free(second);
        }
        else if (third == leaf) {
            AddKey(prev->key[1], second);
            prev->right = NULL;
            RemoveKey(prev->key[1], prev);
            if (leaf->left != NULL) {
                second->right = leaf->left;
            }
            else if (leaf->mid != NULL) {
                second->right = leaf->mid;
            }

            if (second->right != NULL) {
                second->right->prev = second;
            }

            free(third);
        }
    }
    else if ((prev->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2))) {
        if (third == leaf) {
            if (leaf->left != NULL) {
                leaf->mid = leaf->left;
                leaf->left = NULL;
            }

            AddKey(prev->key[1], leaf);
            if (second->size == 2) {
                prev->key[1] = second->key[1];
                RemoveKey(second->key[1], second);
                leaf->left = second->right;
                second->right = NULL;

                if (leaf->left != NULL) {
                    leaf->left->prev = leaf;
                }

            }
            else if (first->size == 2) {
                prev->key[1] = second->key[0];
                leaf->left = second->mid;
                second->mid = second->left;
                if (leaf->left != NULL) {
                    leaf->left->prev = leaf;
                }

                second->key[0] = prev->key[0];
                prev->key[0] = first->key[1];
                RemoveKey(first->key[1], first);
                second->left = first->right;
                if (second->left != NULL) {
                    second->left->prev = second;
                }

                first->right = NULL;
            }
        }
        else if (second == leaf) {
            if (third->size == 2) {
                if (leaf->left == NULL) {
                    leaf->left = leaf->mid;
                    leaf->mid = NULL;
                }

                AddKey(prev->key[1], second);
                prev->key[1] = third->key[0];
                RemoveKey(third->key[0], third);
                second->mid = third->left;
                if (second->mid != NULL) {
                    second->mid->prev = second;
                }

                third->left = third->mid;
                third->mid = third->right;
                third->right = NULL;
            }
            else if (first->size == 2) {
                if (leaf->mid == NULL) {
                    leaf->mid = leaf->left;
                    leaf->left = NULL;
                }
                AddKey(prev->key[0], second);
                prev->key[0] = first->key[1];
                RemoveKey(first->key[1], first);
                second->left = first->right;
                if (second->left != NULL) {
                    second->left->prev = second;
                }

                first->right = NULL;
            }
        }
        else if (first == leaf) {
            if (leaf->left == NULL) {
                leaf->left = leaf->mid;
                leaf->mid = NULL;
            }
            AddKey(prev->key[0], first);
            if (second->size == 2) {
                prev->key[0] = second->key[0];
                RemoveKey(second->key[0], second);
                first->mid = second->left;
                if (first->mid != NULL) {
                    first->mid->prev = first;
                }

                second->left = second->mid;
                second->mid = second->right;
                second->right = NULL;
            }
            else if (third->size == 2) {
                prev->key[0] = second->key[0];
                second->key[0] = prev->key[1];
                prev->key[1] = third->key[0];
                RemoveKey(third->key[0], third);
                first->mid = second->left;
                if (first->mid != NULL) {
                    first->mid->prev = first;
                }

                second->left = second->mid;
                second->mid = third->left;
                if (second->mid != NULL) {
                    second->mid->prev = second;
                }

                third->left = third->mid;
                third->mid = third->right;
                third->right = NULL;
            }
        }
    }
    else if (prev->size == 1) {
        AddKey(prev->key[0], leaf);

        if (first == leaf && second->size == 2) {
            prev->key[0] = second->key[0];
            RemoveKey(second->key[0], second);

            if (leaf->left == NULL) {
                leaf->left = leaf->mid;
            }

            leaf->mid = second->left;
            second->left = second->mid;
            second->mid = second->right;
            second->right = NULL;
            if (leaf->mid != NULL) {
                leaf->mid->prev = leaf;
            }
        }
        else if (second == leaf && first->size == 2) {
            prev->key[0] = first->key[1];
            RemoveKey(first->key[1], first);

            if (leaf->mid == NULL) {
                leaf->mid = leaf->left;
            }

            leaf->left = first->right;
            first->right = NULL;

            if (leaf->left != NULL) {
                leaf->left->prev = leaf;
            }
        }
    }
    return prev;
}

void PrintNode(tree* node) {
    if (node == NULL) {
        return;
    }

    PrintNode(node->left);
    for (int i = 0; i < node->size; i++) {
        printf("%d ", node->key[i]);
    }
    PrintNode(node->mid);
    PrintNode(node->right);
}

void DestroyNode(tree* node) {
    if (node == NULL) {
        return;
    }

    DestroyNode(node->left);
    DestroyNode(node->mid);
    DestroyNode(node->right);
    free(node);
}