# Data-Structure

本项目是基于 C++ 实现的数据结构与算法学习仓库，涵盖了线性表、栈与队列、树、图、排序算法等核心内容。适合数据结构与算法课程学习、考研复习以及面试准备。

每个 `.cpp` 文件都是**独立可编译运行**的完整程序，包含数据结构定义、基本操作实现和测试用的 `main` 函数。

---

## 📚 目录结构

```
Data-Structure/
├── 0-LinarList/          # 线性表
│   ├── LinkList/
│   │   ├── LinkListWithHead.cpp      # 带头结点的单链表
│   │   ├── LinkListWithoutHead.cpp   # 不带头结点的单链表
│   │   ├── DoubleLinkList.cpp        # 双链表
│   │   ├── CLinkList.cpp             # 循环链表
│   │   └── StaticLinkList.cpp        # 静态链表
│   └── SeqList/
│       └── SeqList.cpp               # 顺序表
├── 1-Stack/              # 栈
│   ├── LinkStack/
│   │   └── LinkStack.cpp             # 链栈
│   ├── SeqStack/
│   │   └── SqStack.cpp               # 顺序栈
│   └── bracketCheck/
│       ├── bracketCheck.cpp          # 括号匹配
│       └── InfixToPostfix.cpp        # 中缀转后缀表达式
├── 2-Queue/              # 队列
│   ├── LinkQueue/
│   │   └── LinkQueue.cpp             # 链队列
│   └── SqQueue/
│       ├── SqQueue1.cpp              # 顺序队列
│       └── SqQueue2.cpp              # 循环队列
├── 3-String/             # 字符串
│   └── HString.cpp                   # 堆分配存储的串（含 KMP 算法）
├── 4-Tree/               # 树与二叉树
│   ├── SqBinaryTree.cpp              # 顺序存储二叉树
│   ├── LinkBiTree.cpp                # 链式存储二叉树
│   ├── ThreadBiTree.cpp              # 线索二叉树（中序、先序、后序）
│   └── DisjointSet.cpp               # 并查集
├── 5-Graph/              # 图
│   ├── MGraph.cpp                    # 邻接矩阵存储的图
│   └── ALGraph.cpp                   # 邻接表存储的图
└── 6-Sort_Algorithm/     # 排序算法
    ├── bubbleSort/
    │   └── BubbleSort.cpp
    ├── inserSort/
    │   └── InsertSort.cpp
    ├── selectSort/
    │   └── SelectSort.cpp
    └── ptrLearning/
        └── Pointer.cpp
```

---

## 🚀 快速开始

### 环境要求

- **编译器**：支持 C++11 或更高版本的编译器（如 GCC、Clang）
- **操作系统**：Linux / macOS / Windows

### 编译运行

每个 `.cpp` 文件都是独立的，可以直接编译运行：

```bash
# 以顺序表为例
cd 0-LinarList/SeqList
g++ -std=c++11 SeqList.cpp -o SeqList
./SeqList

# 以链栈为例
cd ../../1-Stack/LinkStack
g++ -std=c++11 LinkStack.cpp -o LinkStack
./LinkStack

# 以线索二叉树为例
cd ../../4-Tree
g++ -std=c++11 ThreadBiTree.cpp -o ThreadBiTree
./ThreadBiTree
```

---

## 📖 各模块详解

### 1. 线性表 (0-LinarList)

| 文件 | 说明 | 核心操作 |
|------|------|---------|
| `SeqList.cpp` | 顺序表 | 初始化、插入、删除、查找、扩容 |
| `LinkListWithHead.cpp` | 带头结点的单链表 | 头插法、尾插法、逆置、合并 |
| `LinkListWithoutHead.cpp` | 不带头结点的单链表 | 基本插入删除 |
| `DoubleLinkList.cpp` | 双链表 | 双向遍历、插入、删除 |
| `CLinkList.cpp` | 循环链表 | 循环遍历 |
| `StaticLinkList.cpp` | 静态链表 | 用数组模拟链表 |

---

### 2. 栈与队列 (1-Stack / 2-Queue)

#### 栈 (Stack)

| 文件 | 说明 | 核心应用 |
|------|------|---------|
| `SqStack.cpp` | 顺序栈 | 基于数组实现 |
| `LinkStack.cpp` | 链栈 | 基于链表实现 |
| `bracketCheck.cpp` | 括号匹配 | 栈的经典应用 |
| `InfixToPostfix.cpp` | 中缀转后缀 | 支持多位数，栈的应用 |

#### 队列 (Queue)

| 文件 | 说明 | 核心特点 |
|------|------|---------|
| `SqQueue1.cpp` | 普通顺序队列 | 基础实现 |
| `SqQueue2.cpp` | 循环队列 | 解决假溢出问题 |
| `LinkQueue.cpp` | 链队列 | 无容量限制 |

---

### 3. 字符串 (3-String)

| 文件 | 说明 | 核心算法 |
|------|------|---------|
| `HString.cpp` | 堆分配存储的串 | 串的比较、连接、子串提取、**KMP模式匹配** |

**KMP 算法**：包含 next 数组和 nextval 数组的计算与优化

---

### 4. 树与二叉树 (4-Tree)

| 文件 | 说明 | 核心内容 |
|------|------|---------|
| `SqBinaryTree.cpp` | 顺序存储二叉树 | 数组实现、父子结点计算 |
| `LinkBiTree.cpp` | 链式存储二叉树 | 遍历、深度计算、层序遍历 |
| `ThreadBiTree.cpp` | **线索二叉树** | 中序、先序、后序线索化及遍历 |
| `DisjointSet.cpp` | **并查集** | 路径压缩、按秩合并 |

**线索二叉树遍历函数**：

```cpp
// 中序线索化及遍历
CreateInThread(root);
InOrderThread(root);        // 正序遍历
RevInOrderThread(root);     // 逆序遍历

// 先序线索化及遍历
CreatePreThread(root);
PreOrderThread(root);       // 先序遍历

// 后序线索化及遍历
CreatePostThread(root);
RevPostOrderThread(root);   // 逆向后序遍历
```

**并查集核心操作**：

```cpp
Initial(S);          // 初始化
Find(S, x);          // 查找根结点
FindPro(S, x);       // 查找 + 路径压缩
Union(S, root1, root2);  // 合并集合
```

---

### 5. 图 (5-Graph)

| 文件 | 说明 | 存储方式 |
|------|------|---------|
| `MGraph.cpp` | 邻接矩阵存储的图 | 二维数组 |
| `ALGraph.cpp` | 邻接表存储的图 | 链表 |

**核心操作**：
- 图的创建（插入顶点、插入边）
- 判断边是否存在 (`Adjacent`)
- 查找邻接点 (`Neighbors`)
- 图的遍历（DFS、BFS）

---

### 6. 排序算法 (6-Sort_Algorithm)

| 文件 | 说明 | 时间复杂度 | 稳定性 |
|------|------|-----------|--------|
| `BubbleSort.cpp` | 冒泡排序 | O(n²) | 稳定 |
| `InsertSort.cpp` | 直接插入排序 | O(n²) | 稳定 |
| `SelectSort.cpp` | 简单选择排序 | O(n²) | 不稳定 |
| `Pointer.cpp` | 指针学习 | - | - |

---

## 🎯 学习建议

### 1. 按顺序学习

建议按照以下顺序学习：

```
线性表 → 栈与队列 → 字符串 → 树与二叉树 → 图 → 排序算法
```

### 2. 动手实践

- 每个 `.cpp` 文件都包含完整的 `main` 函数测试用例
- 建议先阅读代码理解原理，然后自己手写一遍
- 尝试修改代码，添加新功能（如删除操作、其他遍历方式等）

### 3. 重点掌握（面试高频）

- **链表**：头插法、尾插法、逆置、判断环
- **栈与队列**：中缀转后缀、括号匹配、用栈实现队列
- **树**：遍历（递归 + 非递归）、线索二叉树、并查集
- **图**：邻接矩阵与邻接表的转换、DFS/BFS、最短路径
- **排序**：时间复杂度分析、稳定性判断、快速排序

---

## 📝 代码规范

本项目代码遵循以下规范：

- **命名规范**：
  - 结构体/类名：大驼峰（如 `MGraph`、`LinkList`）
  - 函数名：小驼峰（如 `insertNode`、`inOrderTraversal`）
  - 变量名：小驼峰（如 `headNode`、`currentPtr`）

- **注释规范**：
  - 函数前添加功能说明
  - 关键逻辑添加行注释

- **内存管理**：
  - 动态分配的内存及时释放
  - 使用 `new (nothrow)` 检查内存分配是否成功

- **文件组织**：
  - 每个 `.cpp` 文件独立可编译
  - 包含数据结构定义 + 操作实现 + `main` 测试函数

---

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

---

## 📄 许可证

本项目仅供学习交流使用。

---

**Happy Coding! 🎉**
