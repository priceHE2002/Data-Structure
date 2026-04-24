# Data-Structure

本项目是基于 C++ 实现的数据结构与算法学习仓库，涵盖了线性表、栈与队列、树、图、排序算法等核心内容。适合数据结构与算法课程学习、考研复习以及面试准备。

---

## 📚 目录结构

```
Data-Structure/
├── 0-LinarList/          # 线性表
│   ├── LinkList/           # 链表
│   │   ├── LinkListWithHead.cpp      # 带头结点的单链表
│   │   ├── LinkListWithoutHead.cpp   # 不带头结点的单链表
│   │   ├── DoubleLinkList.cpp        # 双链表
│   │   ├── CLinkList.cpp             # 循环链表
│   │   └── StaticLinkList.cpp        # 静态链表
│   └── SeqList/            # 顺序表
│       ├── SeqList.cpp
│       └── SeqList.h
├── 1-Stack/              # 栈
│   ├── SeqStack/
│   │   └── SqStack.cpp               # 顺序栈
│   ├── LinkStack/
│   │   ├── LinkStack.cpp             # 链栈
│   │   └── main.cpp
│   └── bracketCheck/
│       ├── bracketCheck.cpp          # 括号匹配
│       └── InfixToPostfix.cpp        # 中缀转后缀表达式
├── 2-Queue/                # 队列
│   ├── SqQueue/
│   │   ├── SqQueue1.cpp              # 顺序队列
│   │   └── SqQueue2.cpp              # 循环队列
│   └── LinkQueue/
│       └── LinkQueue.cpp             # 链队列
├── 3-String/               # 字符串
│   └── HString.cpp                   # 堆分配存储的串（含 KMP 算法）
├── 4-Tree/                 # 树与二叉树
│   ├── SqBinaryTree.cpp              # 顺序存储二叉树
│   ├── LinkBiTree.cpp                # 链式存储二叉树
│   ├── ThreadBiTree.cpp              # 线索二叉树（中序、先序、后序）
│   └── DisjointSet.cpp               # 并查集
├── 5-Graph/                # 图
│   ├── MGraph.cpp                    # 邻接矩阵存储的图
│   └── ALGraph.cpp                   # 邻接表存储的图
└── 6-Sort_Algorithm/       # 排序算法
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

- **编译器**：支持 C++11 或更高版本的编译器（如 GCC、Clang、MSVC）
- **操作系统**：Linux / macOS / Windows

### 编译运行

以 `Tree/LinkBiTree.cpp` 为例：

```bash
# 进入对应目录
cd Tree

# 使用 g++ 编译
g++ -std=c++11 LinkBiTree.cpp -o LinkBiTree

# 运行
./LinkBiTree
```

---

## 📖 各模块详解

### 1. 线性表 (LinarList)

| 文件 | 说明 |
|------|------|
| `SeqList.cpp` | 顺序表的基本操作：插入、删除、查找 |
| `LinkListWithHead.cpp` | 带头结点的单链表：头插法、尾插法、逆置 |
| `LinkListWithoutHead.cpp` | 不带头结点的单链表 |
| `DoubleLinkList.cpp` | 双链表：双向遍历、插入、删除 |
| `CLinkList.cpp` | 循环链表：循环遍历 |
| `StaticLinkList.cpp` | 静态链表：用数组模拟链表 |

**核心操作**：
- 初始化、插入、删除、查找
- 头插法、尾插法建立链表
- 链表逆置、合并

---

### 2. 栈与队列 (Stack & Queue)

#### 栈 (Stack)

| 文件 | 说明 |
|------|------|
| `SqStack.cpp` | 顺序栈：基于数组实现 |
| `LinkStack.cpp` | 链栈：基于链表实现 |
| `bracketCheck.cpp` | 括号匹配算法 |
| `InfixToPostfix.cpp` | 中缀表达式转后缀表达式（支持多位数） |

**核心应用**：
- 括号匹配
- 表达式求值
- 递归模拟

#### 队列 (Queue)

| 文件 | 说明 |
|------|------|
| `SqQueue1.cpp` | 普通顺序队列 |
| `SqQueue2.cpp` | 循环队列（解决假溢出） |
| `LinkQueue.cpp` | 链队列 |

**核心操作**：
- 入队、出队
- 循环队列的队空/队满判断

---

### 3. 字符串 (String)

| 文件 | 说明 |
|------|------|
| `HString.cpp` | 堆分配存储的串 |

**核心算法**：
- 串的比较、连接、子串提取
- **KMP 模式匹配算法**：next 数组、nextval 数组

---

### 4. 树与二叉树 (Tree)

| 文件 | 说明 |
|------|------|
| `SqBinaryTree.cpp` | 顺序存储二叉树（数组实现） |
| `LinkBiTree.cpp` | 链式存储二叉树：遍历、深度计算、层序遍历 |
| `ThreadBiTree.cpp` | **线索二叉树**：中序、先序、后序线索化及遍历 |
| `DisjointSet.cpp` | **并查集**：路径压缩、按秩合并 |

**核心算法**：
- 二叉树遍历：先序、中序、后序、层序
- 线索二叉树：利用空指针建立前驱/后继线索
- 并查集：集合的合并与查询（带路径压缩优化）

**线索二叉树遍历函数**：

```cpp
// 中序线索化
CreateInThread(root);
InOrderThread(root);      // 正序遍历
RevInOrderThread(root);   // 逆序遍历

// 先序线索化
CreatePreThread(root);
PreOrderThread(root);     // 先序遍历

// 后序线索化
CreatePostThread(root);
RevPostOrderThread(root); // 逆向后序遍历
```

---

### 5. 图 (Graph)

| 文件 | 说明 |
|------|------|
| `MGraph.cpp` | 邻接矩阵存储的图 |
| `ALGraph.cpp` | 邻接表存储的图 |

**核心操作**：
- 图的创建（插入顶点、插入边）
- 判断边是否存在
- 查找邻接点
- 图的遍历（DFS、BFS）

---

### 6. 排序算法 (Sort Algorithm)

| 文件 | 说明 | 时间复杂度 |
|------|------|-----------|
| `BubbleSort.cpp` | 冒泡排序 | O(n²) |
| `InsertSort.cpp` | 直接插入排序 | O(n²) |
| `SelectSort.cpp` | 简单选择排序 | O(n²) |

**核心思想**：
- 冒泡排序：相邻元素比较，大元素后移
- 插入排序：将元素插入到已排序序列的合适位置
- 选择排序：每次选择最小元素放到已排序序列末尾

---

## 🎯 学习建议

### 1. 按顺序学习

建议按照以下顺序学习：

```
线性表 → 栈与队列 → 字符串 → 树与二叉树 → 图 → 排序算法
```

### 2. 动手实践

- 每个数据结构都配有完整的 `main` 函数测试用例
- 建议先阅读代码，理解原理，然后自己手写一遍
- 尝试修改代码，添加新功能

### 3. 重点掌握

- **链表**：头插法、尾插法、逆置（面试高频）
- **栈与队列**：中缀转后缀、括号匹配
- **树**：遍历（递归 + 非递归）、线索二叉树
- **图**：邻接矩阵与邻接表的转换、DFS/BFS
- **排序**：时间复杂度分析、稳定性判断

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

---

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

---

## 📄 许可证

本项目仅供学习交流使用。

---

## 📧 联系方式

如有问题或建议，欢迎通过 GitHub Issue 联系。

---

**Happy Coding! 🎉**
