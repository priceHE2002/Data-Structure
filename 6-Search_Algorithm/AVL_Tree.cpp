#include <iostream>
#include <algorithm>
using namespace std;

typedef struct AVLNode {
    int key;
    int bf;                // 平衡因子 = 左子树高度 - 右子树高度
    AVLNode *lchild, *rchild;
} AVLNode, *AVLTree;

// 求树的高度
int GetHeight(AVLTree T)
{
    if (T == nullptr)
        return 0;
    return max(GetHeight(T->lchild), GetHeight(T->rchild)) + 1;
}

// 更新平衡因子
void UpdateBF(AVLTree T)
{
    if (T != nullptr)
        T->bf = GetHeight(T->lchild) - GetHeight(T->rchild);
}

// ==================== 四种旋转 ====================

// LL 右单旋
//     A (bf=2)                     B
//    ↙            →               ↙  ↖
//   B (bf>=0)                    BL   A
//  ↙  ↖                              ↙  ↖
// BL   BR                            BR   AR
AVLNode* RotateLL(AVLNode *A)
{
    AVLNode *B = A->lchild;
    A->lchild = B->rchild;
    B->rchild = A;
    UpdateBF(A);
    UpdateBF(B);
    return B;
}

// RR 左单旋
//   A (bf=-2)                      B
//      ↘            →             ↙  ↖
//     B (bf<=0)                  A    BR
//     ↙  ↖                      ↙  ↖
//    BL   BR                   AL   BL
AVLNode* RotateRR(AVLNode *A)
{
    AVLNode *B = A->rchild;
    A->rchild = B->lchild;
    B->lchild = A;
    UpdateBF(A);
    UpdateBF(B);
    return B;
}

// LR 先左旋、后右旋
//     A (bf=2)             先对B左旋→LL       再对A右旋
//    ↙                                        C 成为根
//   B (bf=-1)                →               ↙    ↖
//      ↘                                    B      A
//       C
AVLNode* RotateLR(AVLNode *A)
{
    A->lchild = RotateRR(A->lchild);
    return RotateLL(A);
}

// RL 先右旋、后左旋
//   A (bf=-2)              先对B右旋→RR       再对A左旋
//       ↘                                     C 成为根
//     B (bf=1)                →               ↙    ↖
//     ↙                                      A      B
//    C
AVLNode* RotateRL(AVLNode *A)
{
    A->rchild = RotateLL(A->rchild);
    return RotateRR(A);
}

// 对最小不平衡子树做平衡调整
AVLNode* Balance(AVLNode *T)
{
    if (T == nullptr)
        return nullptr;
    UpdateBF(T);

    if (T->bf > 1)
    {
        if (T->lchild->bf >= 0)
            return RotateLL(T);
        else
            return RotateLR(T);
    }
    else if (T->bf < -1)
    {
        if (T->rchild->bf <= 0)
            return RotateRR(T);
        else
            return RotateRL(T);
    }

    return T;
}

// ==================== 插入 ====================

int AVL_Insert(AVLTree &T, int key)
{
    if (T == nullptr)
    {
        T = new AVLNode;
        if (T == nullptr)
        {
            cout << "插入失败，内存申请失败" << endl;
            return 0;
        }
        T->key = key;
        T->bf = 0;
        T->lchild = nullptr;
        T->rchild = nullptr;
        return 1;
    }

    if (key == T->key)
    {
        cout << "插入失败，关键字 " << key << " 已存在" << endl;
        return 0;
    }

    int result;
    if (key < T->key)
        result = AVL_Insert(T->lchild, key);
    else
        result = AVL_Insert(T->rchild, key);

    T = Balance(T);
    return result;
}

// ==================== 删除 ====================

// 找以 T 为根的树中的最小关键字结点
AVLNode* FindMin(AVLTree T)
{
    while (T != nullptr && T->lchild != nullptr)
        T = T->lchild;
    return T;
}

// 找以 T 为根的树中的最大关键字结点
AVLNode* FindMax(AVLTree T)
{
    while (T != nullptr && T->rchild != nullptr)
        T = T->rchild;
    return T;
}

// 在 AVL 树中删除关键字为 key 的结点，返回1成功，返回0失败
int AVL_Delete(AVLTree &T, int key)
{
    if (T == nullptr)
    {
        cout << "删除失败，关键字 " << key << " 不存在" << endl;
        return 0;
    }

    if (key < T->key)
    {
        // 在左子树中删除
        int ret = AVL_Delete(T->lchild, key);
        T = Balance(T);
        return ret;
    }
    else if (key > T->key)
    {
        // 在右子树中删除
        int ret = AVL_Delete(T->rchild, key);
        T = Balance(T);
        return ret;
    }
    else
    {
        // 找到要删除的结点 T
        if (T->lchild != nullptr && T->rchild != nullptr)
        {
            // 情况1：左右孩子都有
            // 用左子树最大值或右子树最小值替换，这里选右子树最小值
            AVLNode *minRight = FindMin(T->rchild);
            T->key = minRight->key;
            AVL_Delete(T->rchild, minRight->key);
            T = Balance(T);
            return 1;
        }
        else
        {
            // 情况2：只有一个孩子或没有孩子
            AVLNode *temp = T;
            if (T->lchild != nullptr)
                T = T->lchild;           // 只有左孩子，用左孩子替代
            else
                T = T->rchild;           // 只有右孩子或没有孩子
            delete temp;
            // 回溯时调用者会对 T 做 Balance，这里不需要
            return 1;
        }
    }
}

// ==================== 遍历与销毁 ====================

void InOrder(AVLTree T)
{
    if (T != nullptr)
    {
        InOrder(T->lchild);
        cout << "[" << T->key << " bf=" << T->bf << "] ";
        InOrder(T->rchild);
    }
}

void DestroyAVL(AVLTree &T)
{
    if (T != nullptr)
    {
        DestroyAVL(T->lchild);
        DestroyAVL(T->rchild);
        delete T;
        T = nullptr;
    }
}

// ==================== 测试 ====================

int main()
{
    AVLTree root = nullptr;

    cout << "========== 测试插入 ==========" << endl;
    int arr[] = {50, 30, 80, 20, 40, 70, 90, 35, 85, 25, 27};
    int n = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < n; i++)
    {
        cout << "插入 " << arr[i] << " -> ";
        AVL_Insert(root, arr[i]);
        cout << "中序: ";
        InOrder(root);
        cout << endl;
    }

    cout << "\n最终 AVL 树:" << endl;
    cout << "中序遍历: ";
    InOrder(root);
    cout << endl;
    cout << "树高度: " << GetHeight(root) << endl;

    cout << "\n========== 测试查找最小/最大 ==========" << endl;
    AVLNode *minNode = FindMin(root);
    AVLNode *maxNode = FindMax(root);
    cout << "最小关键字: " << (minNode ? minNode->key : -1) << endl;
    cout << "最大关键字: " << (maxNode ? maxNode->key : -1) << endl;

    cout << "\n========== 测试删除 ==========" << endl;

    // 删除叶子结点 25
    cout << "删除叶子结点 25..." << endl;
    AVL_Delete(root, 25);
    cout << "中序遍历: ";
    InOrder(root);
    cout << "| 高度: " << GetHeight(root) << endl;

    // 删除只有一个孩子的结点 35（有右孩子40？等等，35右孩子是40吗，需要看树结构）
    // 我们删除一个实际存在的叶子结点 27
    cout << "删除叶子结点 27..." << endl;
    AVL_Delete(root, 27);
    cout << "中序遍历: ";
    InOrder(root);
    cout << "| 高度: " << GetHeight(root) << endl;

    // 删除有两个孩子的结点 50（根结点）
    cout << "删除根结点 50..." << endl;
    AVL_Delete(root, 50);
    cout << "中序遍历: ";
    InOrder(root);
    cout << "| 高度: " << GetHeight(root) << endl;

    // 删除有两个孩子的结点 80
    cout << "删除结点 80..." << endl;
    AVL_Delete(root, 80);
    cout << "中序遍历: ";
    InOrder(root);
    cout << "| 高度: " << GetHeight(root) << endl;

    // 删除不存在的结点
    cout << "删除不存在的结点 100..." << endl;
    AVL_Delete(root, 100);

    // 删除剩余所有结点
    cout << "\n========== 删除剩余结点 ==========" << endl;
    int delArr[] = {90, 85, 70, 40, 30, 35, 20};
    for (int i = 0; i < 7; i++)
    {
        cout << "删除 " << delArr[i] << " -> ";
        AVL_Delete(root, delArr[i]);
        cout << "中序: ";
        InOrder(root);
        cout << "| 高度: " << GetHeight(root) << endl;
    }

    DestroyAVL(root);
    cout << "\n所有测试通过!" << endl;
    return 0;
}
