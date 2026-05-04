#include <cstddef>
#include<iostream>
using namespace std;

//二叉排序树
typedef struct BSTNode{
    int key;
    BSTNode *lchild, *rchild;
}BSTNode, *BSTree;

//二叉排序树的查找（非递归实现）
BSTNode* BST_Search_1(BSTree T, int key)
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
    return T;
}

//二叉排序树的查找（递归实现）
BSTNode *BST_Search_2(BSTree T, int key)
{
    if(T == nullptr)//如果T == nullptr，返回nullptr，查找失败
    {
        return nullptr;
    }
    if(T->key == key)//如果T->key == key ，即找到该结点，查找成功
    {
        return T;
    }

    else if(key < T->key)
    {
        return BST_Search_2(T->lchild, key);//在左子树中查找
    }
    else {
        return BST_Search_2(T->rchild, key);//在右子树中查找
    }
}

//在二叉排序树中插入关键字为key的新结点(非递归实现)，返回1则插入成功，返回0则插入失败
//插入的结点只能是叶子结点
int BST_Insert_1(BSTree &T, int key)
{
    if (T == nullptr)
    {
        T = new BSTNode;
        if (T == nullptr)
        {
            cout << "插入失败，指针内存申请失败" << endl;
            return 0;
        }
        T->key = key;
        T->lchild = nullptr;
        T->rchild = nullptr;
        return 1;
    }

    BSTNode *p = T;
    BSTNode *parent = nullptr;
    while (p != nullptr)
    {
        parent = p;
        if (key == p->key)
        {
            cout << "插入失败，关键字为key的结点已存在" << endl;
            return 0;
        }
        if (key < p->key)
            p = p->lchild;
        else
            p = p->rchild;
    }

    BSTNode *newNode = new BSTNode;
    if (newNode == nullptr)
    {
        cout << "插入失败，指针内存申请失败" << endl;
        return 0;
    }
    newNode->key = key;
    newNode->lchild = nullptr;
    newNode->rchild = nullptr;

    if (key < parent->key)
        parent->lchild = newNode;
    else
        parent->rchild = newNode;

    return 1;
}

//在二叉排序树中插入关键字为key的新结点(递归实现)，返回1则插入成功，返回0则插入失败
//插入的结点只能是叶子结点
int BST_Insert_2(BSTree &T,int key)
{
    if(T == nullptr)
    {
        T = new BSTNode;
        if(T == nullptr)
        {
            cout<<"插入失败，指针内存申请失败"<<endl;
            return 0;
        }
        T->key = key;
        T->lchild = nullptr;
        T->rchild = nullptr;
        return 1;//插入成功
    }
    else if(T->key == key)//如果T->key == key ，则关键字为key的结点已经存在，不需要插入
    {
        cout<<"插入失败，关键字为key的结点已经存在"<<endl;
        return 0;
    }
    else if(key < T->key)
    {
        return BST_Insert_2(T->lchild, key);//在左子树中查找
    }
    else {
        return BST_Insert_2(T->rchild, key);//在右子树中查找
    }
} 

//按照str[]中的关键字序列构建二叉排序树
void Create_BST(BSTree &T, int str[], int n)
{
    T = nullptr;
    int i = 0;
    while (i < n)
    {
        BST_Insert_2(T, str[i]);
        i++;
    }
}

void InOrder(BSTree T)
{
    if (T != nullptr)
    {
        InOrder(T->lchild);
        cout << T->key << " ";
        InOrder(T->rchild);
    }
}

void DestroyBST(BSTree &T)
{
    if (T != nullptr)
    {
        DestroyBST(T->lchild);
        DestroyBST(T->rchild);
        delete T;
        T = nullptr;
    }
}

int main()
{
    BSTree root = nullptr;

    int arr[] = {50, 30, 80, 20, 40, 70, 90, 35, 85};
    int n = sizeof(arr) / sizeof(arr[0]);
    Create_BST(root, arr, n);

    cout << "中序遍历 BST: ";
    InOrder(root);
    cout << endl;

    int targets[] = {40, 90, 35, 100};
    for (int i = 0; i < 4; i++)
    {
        int key = targets[i];

        BSTNode *result1 = BST_Search_1(root, key);
        BSTNode *result2 = BST_Search_2(root, key);

        cout << "查找 " << key << " (非递归): ";
        if (result1 != nullptr)
            cout << "找到，地址: " << result1 << ", key = " << result1->key << endl;
        else
            cout << "未找到" << endl;

        cout << "查找 " << key << " (递归):   ";
        if (result2 != nullptr)
            cout << "找到，地址: " << result2 << ", key = " << result2->key << endl;
        else
            cout << "未找到" << endl;

        cout << "结果一致: " << (result1 == result2 ? "是" : "否") << endl;
        cout << endl;
    }

    cout << "插入新元素 25..." << endl;
    BST_Insert_1(root, 25);
    cout << "插入后中序遍历: ";
    InOrder(root);
    cout << endl;

    cout << "插入重复元素 40..." << endl;
    BST_Insert_1(root, 40);

    cout << "插入新元素 95..." << endl;
    BST_Insert_2(root, 95);
    cout << "插入后中序遍历: ";
    InOrder(root);
    cout << endl;

    DestroyBST(root);

    return 0;
}