#include<iostream>
using namespace std;

//二叉排序树
typedef struct BSTNode{
    int key;
    BSTNode *lchild, *rchild;
}BSTNode, *BSTree;

//二叉排序树的查找
BSTNode* BST_Search(BSTree T, int key)
{
    while(T != nullptr && key != T->key)
    {
        if(key < T->key)
        {
            T = T->lchild;
        }
        else{
            T = T->rchild;
        }
    }
}