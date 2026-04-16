#include <iostream>
using std::cin;
using std::cout;
using std::endl;

//tag = 0 表示指向子结点，tag = 1 表示指向前驱（或后继）
typedef struct ThreadNode
{
    int data;
    ThreadNode *leftChild, *rightChild;
    int ltag, rtag;
}ThreadNode, *ThreadBiTree;

// 全局变量 pre，记录前驱结点
ThreadNode *pre = nullptr;

//初始化线索二叉树，根结点的数据为 data
bool InitThreadBiTree(ThreadBiTree &root, int data)
{
    root = new ThreadNode;
    if (root == nullptr)
    {
        cout<<"内存分配失败"<<endl;
        return false;
    }
    root->leftChild = nullptr;
    root->rightChild = nullptr;
    root->data = data;
    root->ltag = 0;
    root->rtag = 0;
    return true;
}

//在某个父节点后，插入数据为 data 的节点，child = l 时为左孩子，child = r 时为右孩子
bool InsertNewNode(ThreadNode *parent, int data, char child)
{   
    // 检查 parent 是否为空
    if(parent == nullptr)
    {
        cout<<"parent 结点为空"<<endl;
        return false;
    }

    // 检查 child 参数
    if(child != 'l' && child != 'r')
    {
        cout<<"child 输入非法（仅能为'r' or 'l'）"<<endl;
        return false;
    }

    // 插入左孩子
    if(child == 'l')
    {
        if(parent->leftChild == nullptr)
        {
            ThreadNode *newNode = new ThreadNode;
            if (newNode == nullptr)
            {
                cout<<"内存分配失败"<<endl;
                return false;
            }
            newNode->leftChild = nullptr;
            newNode->rightChild = nullptr;
            newNode->ltag = 0;
            newNode->rtag = 0;
            newNode->data = data;
            parent->leftChild = newNode;
            return true;
        }
        else
        {
            cout<<"parent 左孩子结点不为空，插入失败"<<endl;
            return false;
        }
    }
    // 插入右孩子
    else if(child == 'r')
    {
        if(parent->rightChild == nullptr)
        {
            ThreadNode *newNode = new ThreadNode;
            if (newNode == nullptr)
            {
                cout<<"内存分配失败"<<endl;
                return false;
            }
            newNode->leftChild = nullptr;
            newNode->rightChild = nullptr;
            newNode->ltag = 0;
            newNode->rtag = 0;
            newNode->data = data;
            parent->rightChild = newNode;
            return true;
        }
        else
        {
            cout<<"parent 右孩子结点不为空，插入失败"<<endl;
            return false;
        }
    }

    return false;
}

//访问某个结点，建立线索
void visit(ThreadNode *q)
{
    //该结点的左子树为空，建立前驱线索
    if(q->leftChild == nullptr)
    {
        q->leftChild = pre;
        q->ltag = 1;
    }
    //该结点的前驱线索结点的右子树为空，建立前驱线索结点的后继线索
    if(pre != nullptr && pre->rightChild == nullptr)
    {
        pre->rightChild = q;
        pre->rtag = 1;
    }
    pre = q;
}

//中序遍历二叉树，一边遍历一边线索化
void InThread(ThreadBiTree &T)
{
    if(T != nullptr)
    {
        InThread(T->leftChild);
        visit(T);
        InThread(T->rightChild);
    }
}

//中序线索化二叉树T
void CreateInThread(ThreadBiTree &T)
{
    pre = nullptr;  // 重置 pre
    InThread(T);
    
    // 处理最后一个结点的右线索
    if(pre != nullptr && pre->rightChild == nullptr)
    {
        pre->rtag = 1;
        // pre->rightChild 保持为 nullptr，表示没有后继
    }
}

// 找到以 P 为根的子树中，第一个被中序遍历的结点
ThreadNode *FirstNode(ThreadNode *P)
{
    if(P == nullptr) return nullptr;  // 空指针检查
    
    //循环找到最左下角的结点
    while(P->ltag == 0)
    {
        P=P->leftChild;
    }
    return P;
}

//在中序线索二叉树中找到中序遍历的后继结点
ThreadNode *NextNode(ThreadNode *P)
{
    if(P == nullptr) return nullptr;  // 空指针检查
    
    //右子树中最左下角的结点
    if(P->rtag == 0)
    {
        return FirstNode(P->rightChild);
    }
    //如果 P->rtag == 1，直接返回后继线索
    else {
        return P->rightChild;
    }
}

//对中序线索二叉树进行中序遍历
void InOrderThread(ThreadBiTree T)
{
    if(T == nullptr) return;  // 空树检查
    
    for(ThreadNode *p = FirstNode(T); p != nullptr; p = NextNode(p) )
    {
        cout << p->data << " ";  // 直接打印，不使用 visit（visit 会修改线索）
    }
}

// 找到以 P 为根的子树中，最后一个被中序遍历的结点
ThreadNode *LastNode(ThreadNode *P)
{
    if(P == nullptr) return nullptr;  // 空指针检查
    //循环找到最右下角的结点
    while(P->rtag == 0)
    {
        P=P->rightChild;
    }
    return P;
}

//在中序线索二叉树中找到结点P的前驱结点
ThreadNode *PreNode(ThreadNode *P)
{
    //左子树最右下角的结点
    if(P->ltag == 0)
    {
        return LastNode(P->leftChild);
    }
    //ltag = 1时直接返回P的前驱结点
    else 
    {
        return P->leftChild;
    }
}

//对中序线索二叉树进行逆序的中序遍历
void RevInOrderThread(ThreadBiTree T)
{
    if(T == nullptr) return;  // 空树检查
    
    for(ThreadNode *p = LastNode(T); p != nullptr; p = PreNode(p) )
    {
        cout << p->data << " ";  // 直接打印，不使用 visit（visit 会修改线索）
    }
}

//先序遍历二叉树，一边遍历一边线索化
void PreThread(ThreadBiTree &T)
{
    if(T != nullptr)
    {
        visit(T);
        // 只有当左子树不是线索时才递归
        if(T->ltag == 0)
        {
            PreThread(T->leftChild);
        } 

        if(T->rtag == 0)
        {
            PreThread(T->rightChild);
        }

    }
}

//先序线索化二叉树T
void CreatePreThread(ThreadBiTree &T)
{
    pre = nullptr;  // 重置 pre
    PreThread(T);
    // 处理最后一个结点的右线索
    if(pre != nullptr && pre->rightChild == nullptr)
    {
        pre->rtag = 1;
        // pre->rightChild 保持为 nullptr，表示没有后继
    }
}

//先序线索二叉树找先序后继
ThreadNode *PreOrderNextNode(ThreadNode *P)
{
    if(P == nullptr) return nullptr;  // 空指针检查
    
    //若rtag == 1，直接返回右孩子结点
    if(P->rtag == 1)
    {
        return P->rightChild;
    }
    //如果 P->rtag == 0，若P有左孩子，则先序后继为左孩子；若无左孩子，则先序后继为右孩子
    else
    {
        if(P->ltag == 0)
        {
            return P->leftChild;
        }
        else
        {
            return P->rightChild;
        }
    }
}

//对先序线索二叉树进行先序遍历
void PreOrderThread(ThreadBiTree T)
{
    //空树检查
    if (T == nullptr)
    {
        return;
    }
    for(ThreadNode *P = T; P != nullptr; P = PreOrderNextNode(P))
    {
        cout<<P->data<<" ";
    }
}

//后序遍历二叉树，一边遍历一边线索化
void PostThread(ThreadBiTree &T)
{
    if(T != nullptr)
    {
        PostThread(T->leftChild);
        PostThread(T->rightChild);
        visit(T);
    }
}


//后序线索化二叉树T
void CreatePostThread(ThreadBiTree &T)
{
    pre = nullptr;  // 重置 pre
    PostThread(T);
    // 处理最后一个结点的右线索
    if(pre != nullptr && pre->rightChild == nullptr)
    {
        pre->rtag = 1;
        // pre->rightChild 保持为 nullptr，表示没有后继
    }
}

//后序线索二叉树找后序前驱
ThreadNode *PostOrderPreNode(ThreadNode *P)
{
    if(P == nullptr)
    {
        return nullptr;
    }  // 空指针检查

    //若ltag == 1，直接返回左孩子结点
    if(P->ltag == 1)
    {
        return P->leftChild;
    }
    //如果 P->rtag == 0，若P有右孩子，则后序前驱为右孩子；若无右孩子，则后序前驱为左孩子
    else
    {
        if(P->rtag == 0)
        {
            return P->rightChild;
        }
        else
        {
            return P->leftChild;
        }
    }
}

//对后序线索二叉树进行逆向后序遍历
void RevPostOrderThread(ThreadBiTree T)
{
    //空树检查
    if(T == nullptr)
    {
        return;
    }
    for(ThreadNode *P = T; P != nullptr; P=PostOrderPreNode(P))
    {
        cout << P->data <<" ";
    }
}

// 测试用例 1：测试中序线索化
void testInThread()
{
    cout << "\n========== 测试 1：中序线索化 ==========\n" << endl;
    
    ThreadBiTree root;
    InitThreadBiTree(root, 1);
    
    // 构建二叉树
    //       1
    //      / \
    //     2   3
    //    / \   \
    //   4   5   6
    InsertNewNode(root, 2, 'l');
    InsertNewNode(root, 3, 'r');
    InsertNewNode(root->leftChild, 4, 'l');
    InsertNewNode(root->leftChild, 5, 'r');
    InsertNewNode(root->rightChild, 6, 'r');
    
    // 中序线索化
    CreateInThread(root);
    
    // 中序遍历（正序）
    cout << "中序遍历（正序）: ";
    InOrderThread(root);
    cout << endl;
    
    // 中序遍历（逆序）
    cout << "中序遍历（逆序）: ";
    RevInOrderThread(root);
    cout << endl;
    
    cout << "\n预期结果：" << endl;
    cout << "正序：4 2 5 1 3 6" << endl;
    cout << "逆序：6 3 1 5 2 4" << endl;
}

// 测试用例 2：测试先序线索化
void testPreThread()
{
    cout << "\n========== 测试 2：先序线索化 ==========\n" << endl;
    
    ThreadBiTree root;
    InitThreadBiTree(root, 1);
    
    // 构建二叉树
    InsertNewNode(root, 2, 'l');
    InsertNewNode(root, 3, 'r');
    InsertNewNode(root->leftChild, 4, 'l');
    InsertNewNode(root->leftChild, 5, 'r');
    InsertNewNode(root->rightChild, 6, 'r');
    
    // 先序线索化
    CreatePreThread(root);
    
    // 先序遍历
    cout << "先序遍历：";
    PreOrderThread(root);
    cout << endl;
    
    cout << "\n预期结果：1 2 4 5 3 6" << endl;
}

// 测试用例 3：测试后序线索化
void testPostThread()
{
    cout << "\n========== 测试 3：后序线索化 ==========\n" << endl;
    
    ThreadBiTree root;
    InitThreadBiTree(root, 1);
    
    // 构建二叉树
    InsertNewNode(root, 2, 'l');
    InsertNewNode(root, 3, 'r');
    InsertNewNode(root->leftChild, 4, 'l');
    InsertNewNode(root->leftChild, 5, 'r');
    InsertNewNode(root->rightChild, 6, 'r');
    
    // 后序线索化
    CreatePostThread(root);
    
    cout << "后序线索化完成（后序遍历较复杂，建议手动验证线索）" << endl;
    cout << "预期后序序列：4 5 2 6 3 1" << endl;
}

// 测试用例 4：测试空树
void testEmptyTree()
{
    cout << "\n========== 测试 4：空树测试 ==========\n" << endl;
    
    ThreadBiTree emptyTree = nullptr;
    
    cout << "中序遍历空树：";
    InOrderThread(emptyTree);
    cout << " (空)" << endl;
    
    cout << "逆序遍历空树：";
    RevInOrderThread(emptyTree);
    cout << " (空)" << endl;
    
    cout << "先序遍历空树：";
    PreOrderThread(emptyTree);
    cout << " (空)" << endl;
}

// 测试用例 5：测试只有根结点
void testOnlyRoot()
{
    cout << "\n========== 测试 5：只有根结点 ==========\n" << endl;
    
    ThreadBiTree root;
    InitThreadBiTree(root, 10);
    
    // 中序线索化
    CreateInThread(root);
    
    cout << "中序遍历（只有根结点）: ";
    InOrderThread(root);
    cout << endl;
    
    // 先序线索化
    CreatePreThread(root);
    
    cout << "先序遍历（只有根结点）: ";
    PreOrderThread(root);
    cout << endl;
    
    cout << "\n预期结果：10" << endl;
}

// 测试用例 6：测试左斜树
void testLeftSkewedTree()
{
    cout << "\n========== 测试 6：左斜树 ==========\n" << endl;
    
    ThreadBiTree root;
    InitThreadBiTree(root, 1);
    
    // 构建左斜树
    //   1
    //  /
    // 2
    // /
    // 3
    InsertNewNode(root, 2, 'l');
    InsertNewNode(root->leftChild, 3, 'l');
    
    // 中序线索化
    CreateInThread(root);
    
    cout << "中序遍历（左斜树）: ";
    InOrderThread(root);
    cout << endl;
    
    cout << "逆序遍历（左斜树）: ";
    RevInOrderThread(root);
    cout << endl;
    
    cout << "\n预期结果：" << endl;
    cout << "正序：3 2 1" << endl;
    cout << "逆序：1 2 3" << endl;
}

// 测试用例 7：测试右斜树
void testRightSkewedTree()
{
    cout << "\n========== 测试 7：右斜树 ==========\n" << endl;
    
    ThreadBiTree root;
    InitThreadBiTree(root, 1);
    
    // 构建右斜树
    // 1
    //  \
    //   2
    //    \
    //     3
    InsertNewNode(root, 2, 'r');
    InsertNewNode(root->rightChild, 3, 'r');
    
    // 中序线索化
    CreateInThread(root);
    
    cout << "中序遍历（右斜树）: ";
    InOrderThread(root);
    cout << endl;
    
    cout << "逆序遍历（右斜树）: ";
    RevInOrderThread(root);
    cout << endl;
    
    cout << "\n预期结果：" << endl;
    cout << "正序：1 2 3" << endl;
    cout << "逆序：3 2 1" << endl;
}

// 测试用例 8：测试 FirstNode 和 LastNode
void testFirstAndLastNode()
{
    cout << "\n========== 测试 8：FirstNode 和 LastNode ==========\n" << endl;
    
    ThreadBiTree root;
    InitThreadBiTree(root, 1);
    InsertNewNode(root, 2, 'l');
    InsertNewNode(root, 3, 'r');
    InsertNewNode(root->leftChild, 4, 'l');
    InsertNewNode(root->leftChild, 5, 'r');
    InsertNewNode(root->rightChild, 6, 'r');
    
    CreateInThread(root);
    
    ThreadNode *first = FirstNode(root);
    ThreadNode *last = LastNode(root);
    
    cout << "第一个结点：" << (first ? first->data : -1) << endl;
    cout << "最后一个结点：" << (last ? last->data : -1) << endl;
    
    cout << "\n预期结果：" << endl;
    cout << "第一个结点：4" << endl;
    cout << "最后一个结点：6" << endl;
}

// 测试用例 9：测试 NextNode 和 PreNode
void testNextAndPreNode()
{
    cout << "\n========== 测试 9：NextNode 和 PreNode ==========\n" << endl;
    
    ThreadBiTree root;
    InitThreadBiTree(root, 1);
    InsertNewNode(root, 2, 'l');
    InsertNewNode(root, 3, 'r');
    InsertNewNode(root->leftChild, 4, 'l');
    InsertNewNode(root->leftChild, 5, 'r');
    InsertNewNode(root->rightChild, 6, 'r');
    
    CreateInThread(root);
    
    // 找到结点 2
    ThreadNode *node2 = root->leftChild;
    
    ThreadNode *next = NextNode(node2);
    ThreadNode *pre = PreNode(node2);
    
    cout << "结点 2 的后继：" << (next ? next->data : -1) << endl;
    cout << "结点 2 的前驱：" << (pre ? pre->data : -1) << endl;
    
    cout << "\n预期结果：" << endl;
    cout << "结点 2 的后继：5" << endl;
    cout << "结点 2 的前驱：4" << endl;
}

int main()
{
    cout << "========================================" << endl;
    cout << "     线索二叉树完整测试" << endl;
    cout << "========================================" << endl;
    
    // 测试中序线索化
    testInThread();
    
    // 测试先序线索化
    testPreThread();
    
    // 测试后序线索化
    testPostThread();
    
    // 测试空树
    testEmptyTree();
    
    // 测试只有根结点
    testOnlyRoot();
    
    // 测试左斜树
    testLeftSkewedTree();
    
    // 测试右斜树
    testRightSkewedTree();
    
    // 测试 FirstNode 和 LastNode
    testFirstAndLastNode();
    
    // 测试 NextNode 和 PreNode
    testNextAndPreNode();
    
    cout << "\n========================================" << endl;
    cout << "     所有测试完成！" << endl;
    cout << "========================================" << endl;
    
    return 0;
}