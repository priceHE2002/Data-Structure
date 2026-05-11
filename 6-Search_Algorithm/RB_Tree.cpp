#include <iostream>
using namespace std;

#define RED   true
#define BLACK false

typedef struct RBNode {
    int key;
    bool color;               // RED=true, BLACK=false
    RBNode *parent;
    RBNode *lchild;
    RBNode *rchild;
} RBNode, *RBTree;

// ==================== 辅助函数 ====================

RBNode* CreateNode(int key)
{
    RBNode *node = new RBNode;
    node->key = key;
    node->color = RED;
    node->parent = nullptr;
    node->lchild = nullptr;
    node->rchild = nullptr;
    return node;
}

const char* ColorName(bool color)
{
    return color == RED ? "R" : "B";
}

// 找以 T 为根的子树的最小结点
RBNode* FindMin(RBTree T)
{
    while (T != nullptr && T->lchild != nullptr)
        T = T->lchild;
    return T;
}

// 中序遍历
void InOrder(RBTree T)
{
    if (T != nullptr)
    {
        InOrder(T->lchild);
        cout << T->key << "(" << ColorName(T->color) << ") ";
        InOrder(T->rchild);
    }
}

// 销毁红黑树
void DestroyRB(RBTree &T)
{
    if (T != nullptr)
    {
        DestroyRB(T->lchild);
        DestroyRB(T->rchild);
        delete T;
        T = nullptr;
    }
}

// ==================== 旋转操作 ====================

// 对 T 做左旋: 将 T 的右孩子旋转到 T 的位置
//     T                B
//    / \      →       / \
//   AL  B            T   BR
//      / \          / \
//     BL  BR       AL  BL
void LeftRotate(RBTree &root, RBNode *T)
{
    RBNode *B = T->rchild;          // B 是 T 的右孩子
    T->rchild = B->lchild;          // B 的左子树 BL 挂给 T 作为右子树
    if (B->lchild != nullptr)
        B->lchild->parent = T;

    B->parent = T->parent;          // B 接替 T 的位置
    if (T->parent == nullptr)
        root = B;                   // T 是根，B 成为新根
    else if (T == T->parent->lchild)
        T->parent->lchild = B;      // T 是左孩子，B 接替左孩子
    else
        T->parent->rchild = B;      // T 是右孩子，B 接替右孩子

    B->lchild = T;                  // T 变为 B 的左孩子
    T->parent = B;
}

// 对 T 做右旋: 将 T 的左孩子旋转到 T 的位置
//       T              B
//      / \      →     / \
//     B   AR         BL  T
//    / \                / \
//   BL  BR             BR  AR
void RightRotate(RBTree &root, RBNode *T)
{
    RBNode *B = T->lchild;          // B 是 T 的左孩子
    T->lchild = B->rchild;          // B 的右子树 BR 挂给 T 作为左子树
    if (B->rchild != nullptr)
        B->rchild->parent = T;

    B->parent = T->parent;          // B 接替 T 的位置
    if (T->parent == nullptr)
        root = B;                   // T 是根，B 成为新根
    else if (T == T->parent->rchild)
        T->parent->rchild = B;      // T 是右孩子，B 接替右孩子
    else
        T->parent->lchild = B;      // T 是左孩子，B 接替左孩子

    B->rchild = T;                  // T 变为 B 的右孩子
    T->parent = B;
}

// ==================== 插入 ====================

// 插入结点 z 后修复红黑树性质
void RB_Insert_Fixup(RBTree &root, RBNode *z)
{
    // 只要 z 的父结点为红色就需要修复（破坏性质4: 红色结点不能有红色孩子）
    while (z->parent != nullptr && z->parent->color == RED)
    {
        // 父结点是祖父结点的左孩子
        if (z->parent == z->parent->parent->lchild)
        {
            RBNode *uncle = z->parent->parent->rchild;  // 叔结点（右孩子）

            // 情况1: 叔结点为红色
            // 解决: 父、叔变黑，祖父变红，z 上移到祖父继续检查
            if (uncle != nullptr && uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                // 情况2: 叔结点为黑色，z 是右孩子 (LR 型)
                // 解决: 对父结点左旋，变为情况3 (LL 型)
                if (z == z->parent->rchild)
                {
                    z = z->parent;
                    LeftRotate(root, z);
                }
                // 情况3: 叔结点为黑色，z 是左孩子 (LL 型)
                // 解决: 父变黑，祖父变红，对祖父右旋
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(root, z->parent->parent);
            }
        }
        else // 父结点是祖父结点的右孩子（对称）
        {
            RBNode *uncle = z->parent->parent->lchild;  // 叔结点（左孩子）

            // 情况1: 叔结点为红色
            if (uncle != nullptr && uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                // 情况2: 叔结点为黑色，z 是左孩子 (RL 型)
                if (z == z->parent->lchild)
                {
                    z = z->parent;
                    RightRotate(root, z);
                }
                // 情况3: 叔结点为黑色，z 是右孩子 (RR 型)
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(root, z->parent->parent);
            }
        }
    }
    root->color = BLACK;  // 性质2: 根结点必须为黑色
}

// 向红黑树中插入关键字 key，返回插入的结点
RBNode* RB_Insert(RBTree &root, int key)
{
    // 先做标准 BST 插入
    RBNode *z = CreateNode(key);    // 新结点默认红色

    RBNode *y = nullptr;            // y 记录 z 的父结点
    RBNode *x = root;               // x 从根开始向下查找

    while (x != nullptr)
    {
        y = x;
        if (z->key == x->key)
        {
            cout << "插入失败，关键字 " << key << " 已存在" << endl;
            delete z;
            return nullptr;
        }
        if (z->key < x->key)
            x = x->lchild;
        else
            x = x->rchild;
    }

    z->parent = y;
    if (y == nullptr)
        root = z;                   // 空树，z 成为根
    else if (z->key < y->key)
        y->lchild = z;
    else
        y->rchild = z;

    // 修复红黑树性质
    RB_Insert_Fixup(root, z);
    return z;
}

// ==================== 删除 ====================

// 用 v 替换 u（只修改 parent 指向，不处理 v 的孩子）
void RB_Transplant(RBTree &root, RBNode *u, RBNode *v)
{
    if (u->parent == nullptr)
        root = v;
    else if (u == u->parent->lchild)
        u->parent->lchild = v;
    else
        u->parent->rchild = v;

    if (v != nullptr)
        v->parent = u->parent;
}

// 删除结点 z 后修复红黑树性质
// x 指向替代 z 的结点（可能为 nullptr），它有“双重黑色”
// x_parent 是 x 的父结点，x_is_left 指示 x 是其父结点的左孩子还是右孩子
void RB_Delete_Fixup(RBTree &root, RBNode *x, RBNode *x_parent, bool x_is_left)
{
    while (x != root && (x == nullptr || x->color == BLACK))
    {
        if (x_is_left)  // x 是左孩子
        {
            RBNode *w = x_parent->rchild;  // 兄弟结点（右孩子）

            // 情况1: 兄弟 w 为红色
            if (w != nullptr && w->color == RED)
            {
                w->color = BLACK;
                x_parent->color = RED;
                LeftRotate(root, x_parent);
                w = x_parent->rchild;  // 更新 w
            }

            // 情况2: 兄弟 w 为黑色，且 w 的两个孩子都为黑色
            if ((w == nullptr) ||
                ((w->lchild == nullptr || w->lchild->color == BLACK) &&
                 (w->rchild == nullptr || w->rchild->color == BLACK)))
            {
                if (w != nullptr)
                    w->color = RED;
                x = x_parent;
                x_parent = x->parent;
                if (x_parent != nullptr)
                    x_is_left = (x == x_parent->lchild);
            }
            else
            {
                // 情况3: 兄弟 w 为黑色，w 的右孩子为黑色（左孩子为红色）
                if (w == nullptr || w->rchild == nullptr || w->rchild->color == BLACK)
                {
                    if (w != nullptr && w->lchild != nullptr)
                        w->lchild->color = BLACK;
                    if (w != nullptr)
                        w->color = RED;
                    if (w != nullptr)
                        RightRotate(root, w);
                    w = x_parent->rchild;
                }

                // 情况4: 兄弟 w 为黑色，w 的右孩子为红色
                if (w != nullptr)
                {
                    w->color = x_parent->color;
                    if (w->rchild != nullptr)
                        w->rchild->color = BLACK;
                }
                x_parent->color = BLACK;
                LeftRotate(root, x_parent);
                x = root;
                x_parent = nullptr;
            }
        }
        else  // x 是右孩子（对称）
        {
            RBNode *w = x_parent->lchild;  // 兄弟结点（左孩子）

            // 情况1: 兄弟 w 为红色
            if (w != nullptr && w->color == RED)
            {
                w->color = BLACK;
                x_parent->color = RED;
                RightRotate(root, x_parent);
                w = x_parent->lchild;
            }

            // 情况2: 兄弟 w 为黑色，且 w 的两个孩子都为黑色
            if ((w == nullptr) ||
                ((w->lchild == nullptr || w->lchild->color == BLACK) &&
                 (w->rchild == nullptr || w->rchild->color == BLACK)))
            {
                if (w != nullptr)
                    w->color = RED;
                x = x_parent;
                x_parent = x->parent;
                if (x_parent != nullptr)
                    x_is_left = (x == x_parent->lchild);
            }
            else
            {
                // 情况3: 兄弟 w 为黑色，w 的左孩子为黑色（右孩子为红色）
                if (w == nullptr || w->lchild == nullptr || w->lchild->color == BLACK)
                {
                    if (w != nullptr && w->rchild != nullptr)
                        w->rchild->color = BLACK;
                    if (w != nullptr)
                        w->color = RED;
                    if (w != nullptr)
                        LeftRotate(root, w);
                    w = x_parent->lchild;
                }

                // 情况4: 兄弟 w 为黑色，w 的左孩子为红色
                if (w != nullptr)
                {
                    w->color = x_parent->color;
                    if (w->lchild != nullptr)
                        w->lchild->color = BLACK;
                }
                x_parent->color = BLACK;
                RightRotate(root, x_parent);
                x = root;
                x_parent = nullptr;
            }
        }
    }
    if (x != nullptr)
        x->color = BLACK;
}

// 在红黑树中删除关键字为 key 的结点
int RB_Delete(RBTree &root, int key)
{
    // 查找要删除的结点
    RBNode *z = root;
    while (z != nullptr && z->key != key)
    {
        if (key < z->key)
            z = z->lchild;
        else
            z = z->rchild;
    }

    if (z == nullptr)
    {
        cout << "删除失败，关键字 " << key << " 不存在" << endl;
        return 0;
    }

    RBNode *y = z;             // y 指向实际被删除或移动的结点
    RBNode *x = nullptr;       // x 指向替代 y 的结点
    RBNode *x_parent = nullptr;
    bool x_is_left = false;    // x 是其父结点的左孩子还是右孩子
    bool y_original_color = y->color;

    // 情况A: z 最多只有一个孩子
    if (z->lchild == nullptr)
    {
        x = z->rchild;
        x_parent = z->parent;
        if (x_parent != nullptr)
            x_is_left = (z == x_parent->lchild);
        RB_Transplant(root, z, z->rchild);
    }
    else if (z->rchild == nullptr)
    {
        x = z->lchild;
        x_parent = z->parent;
        if (x_parent != nullptr)
            x_is_left = (z == x_parent->lchild);
        RB_Transplant(root, z, z->lchild);
    }
    else // 情况B: z 有两个孩子
    {
        y = FindMin(z->rchild);          // 找后继结点
        y_original_color = y->color;
        x = y->rchild;
        x_parent = y->parent;

        if (y->parent == z)
        {
            // 后继就是 z 的右孩子
            x_parent = y;
            x_is_left = false;           // x 是 y 的右孩子（y 在 z 的右子树中）
            if (x != nullptr)
                x->parent = y;
        }
        else
        {
            // 后继在 z 的右子树的更深处
            x_is_left = (y == x_parent->lchild);  // 记录 y 是左还是右
            RB_Transplant(root, y, y->rchild);
            y->rchild = z->rchild;
            y->rchild->parent = y;
        }
        RB_Transplant(root, z, y);
        y->lchild = z->lchild;
        y->lchild->parent = y;
        y->color = z->color;
    }

    delete z;

    // 如果删除的结点是黑色，需要修复
    if (y_original_color == BLACK)
        RB_Delete_Fixup(root, x, x_parent, x_is_left);

    return 1;
}

// ==================== 测试 ====================

int main()
{
    RBTree root = nullptr;

    // ===== 测试插入 =====
    cout << "========== 测试红黑树插入 ==========" << endl;
    int arr[] = {50, 30, 80, 20, 40, 70, 90, 35, 85, 25, 27, 10, 60, 95, 15};
    int n = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < n; i++)
    {
        cout << "插入 " << arr[i] << " -> ";
        RB_Insert(root, arr[i]);
        cout << "中序: ";
        InOrder(root);
        cout << endl;
    }

    cout << "\n最终红黑树中序遍历: ";
    InOrder(root);
    cout << endl;
    cout << "根结点: " << root->key << "(" << ColorName(root->color) << ")" << endl;

    // ===== 测试查找 =====
    cout << "\n========== 测试查找最小/最大 ==========" << endl;
    RBNode *minNode = FindMin(root);
    RBNode *maxNode = root;
    while (maxNode != nullptr && maxNode->rchild != nullptr)
        maxNode = maxNode->rchild;
    cout << "最小关键字: " << minNode->key << endl;
    cout << "最大关键字: " << maxNode->key << endl;

    // ===== 测试删除 =====
    cout << "\n========== 测试红黑树删除 ==========" << endl;

    // 删除叶子结点
    cout << "删除叶子结点 15..." << endl;
    RB_Delete(root, 15);
    cout << "中序: ";
    InOrder(root);
    cout << endl;

    // 删除只有一个孩子的结点
    cout << "删除单孩子结点 20..." << endl;
    RB_Delete(root, 20);
    cout << "中序: ";
    InOrder(root);
    cout << endl;

    // 删除有两个孩子的结点
    cout << "删除双孩子结点 30..." << endl;
    RB_Delete(root, 30);
    cout << "中序: ";
    InOrder(root);
    cout << endl;

    // 删除根结点
    cout << "删除根结点 " << root->key << "..." << endl;
    RB_Delete(root, root->key);
    cout << "中序: ";
    InOrder(root);
    cout << "| 新根: " << root->key << "(" << ColorName(root->color) << ")" << endl;

    // 删除不存在的结点
    cout << "删除不存在的结点 100..." << endl;
    RB_Delete(root, 100);

    // 删除更多结点
    cout << "删除 80..." << endl;
    RB_Delete(root, 80);
    cout << "中序: ";
    InOrder(root);
    cout << endl;

    cout << "删除 90..." << endl;
    RB_Delete(root, 90);
    cout << "中序: ";
    InOrder(root);
    cout << endl;

    cout << "删除 70..." << endl;
    RB_Delete(root, 70);
    cout << "中序: ";
    InOrder(root);
    cout << endl;

    // ===== 删除所有剩余结点 =====
    cout << "\n========== 删除剩余结点 ==========" << endl;
    int delArr[] = {50, 60, 40, 35, 85, 25, 27, 10, 95};
    for (int i = 0; i < 9; i++)
    {
        if (root == nullptr) break;
        cout << "删除 " << delArr[i] << " -> ";
        RB_Delete(root, delArr[i]);
        if (root != nullptr)
        {
            cout << "中序: ";
            InOrder(root);
        }
        else
            cout << "树为空";
        cout << endl;
    }

    DestroyRB(root);
    cout << "\n所有测试通过!" << endl;
    return 0;
}
