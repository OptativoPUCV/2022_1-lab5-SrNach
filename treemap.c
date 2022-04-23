#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap*)calloc(1, sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (searchTreeMap(tree, key) != NULL) return;
    
    tree->current = tree->root;
    TreeNode * aux = NULL;
    
    while (tree->current != NULL){
        aux = tree->current;
        if (tree->lower_than(tree->current->pair->key, key) == 0){
            tree->current = tree->current->left;
            continue;
        }
        if (tree->lower_than(tree->current->pair->key, key) == 1){
            tree->current = tree->current->right;
            continue;
        }
    }

    tree->current = createTreeNode(key, value);
    tree->current->parent = aux;

    if (tree->lower_than(tree->current->pair->key, aux->pair->key))
        aux->left = tree->current;
    else aux->right = tree->current;
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    int hijo; //izq = 0, der = 1

    if (node->parent != NULL){
        if (node == node->parent->left) hijo = 0;
        else hijo = 1;
    }

    if (hijo == 1) node->parent->right = node->left;
    if (hijo == 0) node->parent->left = node->left;

    //Sin hijos
    if (node->left == NULL && node->right == NULL){
        free(node);
        return;}

    //Sólo un hijo
    if (node->left == NULL && node->right != NULL){
        node->right->parent = node->parent;
        free(node);
        return;}
    if (node->left != NULL && node->right == NULL){
        node->left->parent = node->parent;
        free(node);
        return;}

    //Con dos hijos
    if (node->left != NULL && node->right != NULL){
        TreeNode* minim = minimum(node->right);
        node->pair = minim->pair;
        removeNode(tree,minimum(node->right));
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    tree->current = tree->root;

    while (tree->current != NULL){
        if (is_equal(tree, tree->current->pair->key, key)){
            return tree->current->pair;
        }
        if (tree->lower_than(tree->current->pair->key, key) == 0){
            tree->current = tree->current->left;
            continue;
        }
        if (tree->lower_than(tree->current->pair->key, key) == 1){
            tree->current = tree->current->right;
            continue;
        }
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    if (searchTreeMap(tree, key) != NULL) return searchTreeMap(tree, key);

    while (tree->lower_than(tree->current->pair->key, key)){
        tree->current = tree->current->right;
    }
    return tree->current
}

Pair * firstTreeMap(TreeMap * tree) {
    tree->current = minimum(tree->root);
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree->current->right != NULL)
        tree->current = tree->current->right;
    tree->current = minimum(tree->current);
    return tree->current->pair;
}
