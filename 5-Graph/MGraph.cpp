#include<iostream>
#include<queue>
using namespace std;
#define MaxVertexNum 100 // 顶点数目最大值

// 无向无权图的邻接矩阵存储结构
struct MGraph{
    char Vex[MaxVertexNum];                // 顶点表：存储每个顶点的数据（例如字符 'A', 'B' 等）
    bool Edge[MaxVertexNum][MaxVertexNum]; // 邻接矩阵（边表）：Edge[i][j]为true表示顶点i和j之间有边
    int vexnum, edgenum;                   // 顶点数，边数（弧数）
};

// 初始化图
void InitMGraph(MGraph &G) 
{
    G.vexnum = 0;  // 初始顶点数为0
    G.edgenum = 0; // 初始边数为0

    // 遍历邻接矩阵，将所有可能的边初始化为 false（代表无边）
    // 时间复杂度为 O(V^2)，V为最大顶点数
    for (int i = 0; i < MaxVertexNum; i++) {
        for (int j = 0; j < MaxVertexNum; j++) {
            G.Edge[i][j] = false; 
        }
    }
}

// 判断图G是否存在边(x , y)
bool Adjacent(MGraph &G, int x, int y) 
{
    // 越界检查：确保传入的顶点索引合法
    if(x < 0 || x >= G.vexnum || y < 0 || y >= G.vexnum)
    {
        cout << "x或y输入非法" << endl;
        return false;
    }

    return G.Edge[x][y]; // 直接返回矩阵中对应的值，true即存在边，false即不存在
}

// 列出图G中与结点x相邻接的所有的顶点
void Neighbors(MGraph &G, int x)
{
    // 遍历第 x 行的所有元素
    for (int i = 0; i < G.vexnum; i++)
    {
        // 如果存在边，说明 i 是 x 的邻接点
        if(G.Edge[x][i])
        {
            cout << G.Vex[i] << " "; // 打印对应顶点的信息
        }
    }
    cout << endl;
}

// 在图G中插入边(x , y)
void InsertEdge(MGraph &G, int x, int y)
{
    // 越界检查
    if(x < 0 || x >= G.vexnum || y < 0 || y >= G.vexnum)
    {
        cout << "x或y输入非法" << endl;
        return;
    }

    // 检查边是否已经存在，避免重复增加边数
    if (!G.Edge[x][y]) 
    {
        G.Edge[x][y] = true; // 建立 x 到 y 的边
        G.Edge[y][x] = true; // 因为是无向图，所以对称位置 y 到 x 也要设为 true
        G.edgenum++;         // 图的总边数加1
    }
}

// 在图G中插入顶点X
void InsertVertex(MGraph &G, char X) 
{
    // 检查空间是否已满，防止数组越界
    if (G.vexnum >= MaxVertexNum) {
        return; 
    }
    // 将顶点信息存入顶点表的最尾部
    G.Vex[G.vexnum] = X;
    // 顶点数加 1
    G.vexnum++;
}

// 求图中顶点x的第一个邻接点
// 若有则返回顶点号，若x没有邻接点或越界，则返回-1
int FirstNeighbor(MGraph &G, int x)
{
    // 检查索引合法性
    if (x < 0 || x >= G.vexnum) 
    {
        return -1; 
    }

    // 直接在邻接矩阵第 x 行中从头查找第一个为true的元素
    for (int j = 0; j < G.vexnum; j++)
    {
        if (G.Edge[x][j] == true) 
        {
            return j; // 找到即返回索引
        }
    }
    // 如果遍历完都没有找到，说明是孤立点，返回-1
    return -1;
}

// 假设图G中顶点y是顶点x的一个邻接点，返回除y之外顶点x的下一个邻接点的顶点号
int NextNeighbor(MGraph &G, int x, int y) 
{
    // 检查索引合法性，并确保y确实是x的邻接点
    if (x < 0 || x >= G.vexnum || y < 0 || y >= G.vexnum || G.Edge[x][y] == false) 
    {
        return -1; 
    }

    // 直接从 y 的下一个位置 (y + 1) 开始向后寻找下一个邻接点
    for (int j = y + 1; j < G.vexnum; j++)
    {
        if (G.Edge[x][j] == true) 
        {
            return j;
        }
    }

    // 如果遍历到最后都没找到，说明 y 已经是 x 的最后一个邻接点
    return -1;
}

bool visited[MaxVertexNum]; // 全局访问标记数组，用于记录顶点是否已被访问过

// 从顶点v出发，广度优先遍历(BFS)图G
void BFS(MGraph &G, int v)
{
    queue<int> q; // 辅助队列q，用于存放待处理其邻居的顶点

    visited[v] = true; // 将初始顶点v标记为已访问
    cout << v << " ";  // 访问并打印顶点v（此处打印的是索引）
    q.push(v);         // 顶点v入队

    while(!q.empty()) // 当队列q不为空时，说明还有顶点的邻居未被检查
    {
        int u = q.front(); // 取出队头元素赋值给u
        q.pop();           // 队头元素出队

        // 遍历所有可能的顶点，寻找 u 的邻接点
        for(int w = 0; w < G.vexnum; w++)
        {
            // 如果 w 还没被访问过，且 u 和 w 之间有边相连
            if(!visited[w] && G.Edge[u][w] == true)
            {
                cout << w << " ";   // 访问w结点
                visited[w] = true;  // 立即将w标记为已访问，防止被重复入队
                q.push(w);          // 将w入队，以便后续检查w的邻居
            }
        }
        
        /* 下面这段注释掉的代码是使用 FirstNeighbor 和 NextNeighbor 的标准写法，
        作用与上面的 for 循环完全一致。它抽象层次更高，适用于任何图的存储结构（包括邻接表）。
        for(int w = FirstNeighbor(G,u); w >= 0 ; w = NextNeighbor(G, u, w))
        {
            if(!visited[w])
            {
                cout<< w << " ";
                visited[w] = true;
                q.push(w);
            }
        }
        */
    }
}

// 判断一个无向图是否是一棵树
bool isTree(MGraph &G)
{
    // 树的必要条件：边数 E 等于 顶点数 V - 1
    if(G.vexnum - 1 != G.edgenum)
    {
        return false;
    }
    
    // 初始化visited数组，准备进行遍历连通性测试
    for(int i = 0; i < G.vexnum; i++)
    {
        visited[i] = false;
    }
    
    // 从顶点0开始进行一次 BFS 遍历
    // 如果是一棵树，那么图必须是连通的，一次 BFS 就能访问到所有顶点
    BFS(G, 0);
    
    // 检查是否所有顶点都在同一个连通分量中（即是否都被访问到了）
    for(int j = 0; j < G.vexnum; j++)
    {
        if(!visited[j])
        {
            // 如果存在未被访问到的顶点，说明图不连通，不是树
            return false;
        }
    }
    return true; // 既满足 E = V - 1，又是连通的，则必为一棵树
}

// 对非连通图（或一般图）进行完整的广度优先遍历
void BFSTraverse(MGraph &G)
{
    // 将访问标记数组重置为全 false
    for(int i = 0; i < MaxVertexNum; i++) {
        visited[i] = false;
    }
    
    // 遍历所有顶点作为可能的起点
    // 这是为了防止图包含多个分离的连通分量（孤岛）
    for(int j = 0; j < G.vexnum; j++)
    {
        if(!visited[j]) // 只有当该顶点未被之前的 BFS 覆盖时，才以此为新起点发起 BFS
        {
            BFS(G, j);
        }
    }
}

int d[MaxVertexNum];//数组d用于记录顶点v到每个顶点的路径长度
int path[MaxVertexNum]; // 数组path用于记录最短路径中当前顶点的前驱顶点（从哪个顶点过来的）

// BFS求解单源最短路径
void BFS_Min_Distance(MGraph &G, int v)
{
    // 合法性检查，防止传入不存在的顶点导致数组越界
    if(v < 0 || v >= G.vexnum) return; 
    
    queue<int> q; // 辅助队列，用于存放待处理其邻居的顶点
    
    // 1. 初始化阶段
    for(int i = 0; i < G.vexnum ; i++) // 遍历图中所有的有效顶点
    {
        d[i] = -1;         // 初始化为 -1，表示尚未发现路径（或不可达）
        path[i] = -1;      // 初始化为 -1，表示暂时没有前驱结点
        visited[i] = false; // 初始化所有顶点为未访问状态
    }
    
    // 2. 处理源点 v
    d[v] = 0;          // 源点到自身的距离显然为 0
    q.push(v);         // 源点 v 入队，作为 BFS 扩散的起点
    visited[v] = true; // 标记源点已被访问，防止重复入队
    
    // 3. 广度优先搜索
    while(!q.empty()) // 当队列不为空时，说明还有顶点需要向外扩散寻找邻居
    {
        int u = q.front(); // 取出当前的队头顶点 u
        q.pop();           // 队头元素出队
        
        // 遍历整个邻接矩阵的第 u 行，寻找 u 的所有邻接点
        for(int w = 0; w < G.vexnum; w++) 
        {
            // 如果 w 尚未被访问过，并且 u 和 w 之间存在一条边
            if(!visited[w] && G.Edge[u][w] == true) 
            {
                // 核心逻辑：因为是 BFS，第一次访问到 w 时走过的路径一定是最短的
                d[w] = d[u] + 1; // v 到 w 的最短距离 = v 到 u 的距离 + 1 条边
                path[w] = u;     // 记录 w 的前驱结点是 u (即最短路径是从 u 走到 w 的)
                q.push(w);         // 将新发现的顶点 w 入队，下一层将从 w 继续向外扩散
                visited[w] = true; // 立即将 w 标记为已访问，防止被同一层或后续层的其他顶点重复发现
            }
        }
    }
}

//从顶点v开始，对图G进行深度优先遍历
void DFS(MGraph &G,int v)
{
    cout<<v<<" "; //访问v结点
    visited[v] = true; //将v结点的访问数组设置为true
    for(int w = 0; w < G.vexnum; w++)
    {
        if(!visited[w] && G.Edge[v][w] == true)//如果w结点没有被访问，并且v和w之间有边
        {
            DFS(G, w);
        }
    }
}

// 对非连通图（或一般图）进行完整的深度优先遍历
void DFSTraverse(MGraph &G)
{
    // 将访问标记数组重置为全 false
    for(int i = 0; i < MaxVertexNum; i++) {
        visited[i] = false;
    }
    
    // 遍历所有顶点作为可能的起点，处理非连通图的孤岛问题
    for(int j = 0; j < G.vexnum; j++)
    {
        if(!visited[j]) // 如果没被访问过，就以它为起点向深处探索
        {
            DFS(G, j);
        }
    }
}

int main()
{
    MGraph G;
    
    // ================= 测试 1：初始化与图的构建 =================
    cout << "========== 测试 1：初始化与图的构建 ==========" << endl;
    InitMGraph(G);
    
    // 插入 6 个顶点 (0:A, 1:B, 2:C, 3:D, 4:E, 5:F)
    InsertVertex(G, 'A'); 
    InsertVertex(G, 'B'); 
    InsertVertex(G, 'C'); 
    InsertVertex(G, 'D'); 
    InsertVertex(G, 'E'); 
    InsertVertex(G, 'F'); 
    
    /* 构建的图结构大致如下：
       连通分量1 (有环):       孤立点 (非连通):
         A(0) --- B(1)          F(5)
          |        |
         C(2) --- D(3)
          |
         E(4)
    */

    InsertEdge(G, 0, 1); // A - B
    InsertEdge(G, 0, 2); // A - C
    InsertEdge(G, 1, 3); // B - D
    InsertEdge(G, 2, 3); // C - D
    InsertEdge(G, 2, 4); // C - E
    // 注意：F(5) 故意不与其他点连接，用于测试非连通图的遍历
    
    cout << "✅ 图 G 初始化完成！顶点数: " << G.vexnum << "，边数: " << G.edgenum << "\n" << endl;

    // ================= 测试 2：邻接关系与查询功能 =================
    cout << "========== 测试 2：邻接关系与查询功能 ==========" << endl;
    
    cout << "1. A(0) 和 B(1) 是否相邻？ " << (Adjacent(G, 0, 1) ? "是" : "否") << " (预期: 是)" << endl;
    cout << "2. A(0) 和 D(3) 是否相邻？ " << (Adjacent(G, 0, 3) ? "是" : "否") << " (预期: 否)" << endl;
    
    cout << "3. C(2) 的所有邻接点信息: ";
    Neighbors(G, 2); // 预期输出: A D E 
    
    int first = FirstNeighbor(G, 2);
    cout << "4. C(2) 的第一个邻接点索引: " << first << " (对应顶点 " << G.Vex[first] << ")" << endl;
    
    int next = NextNeighbor(G, 2, first);
    cout << "5. C(2) 在索引 " << first << " 之后的下一个邻接点索引: " << next << " (对应顶点 " << G.Vex[next] << ")\n" << endl;

    // ================= 测试 3：广度优先遍历 (BFS) =================
    cout << "========== 测试 3：广度优先遍历 (BFS) ==========" << endl;
    
    // 单次 BFS 测试前，重置 visited 数组
    for(int i = 0; i < MaxVertexNum; i++) visited[i] = false;
    cout << "1. 从 A(0) 开始的单次 BFS (只能遍历到连通的部分): ";
    BFS(G, 0); 
    cout << "\n   (注意：孤立点 F(5) 不会被遍历到)\n" << endl;

    cout << "2. 对整个非连通图进行 BFSTraverse 全局遍历: ";
    BFSTraverse(G); 
    cout << "\n   (注意：所有顶点包含 F(5) 都应该被遍历到)\n" << endl;

    // ================= 测试 4：判断是否为树 (isTree) =================
    cout << "========== 测试 4：判断是否为树 (isTree) ==========" << endl;
    
    cout << "1. 图 G 是否是一棵树？ " << (isTree(G) ? "是" : "否") << endl;
    cout << "   (预期: 否。因为图 G 边数不满足 V-1，且包含环 A-B-D-C，还不连通)\n" << endl;

    // 为了验证 isTree 的正确性，我们新建一个绝对是一棵树的图 T
    MGraph T;
    InitMGraph(T);
    InsertVertex(T, 'R'); // 0 (根)
    InsertVertex(T, 'X'); // 1
    InsertVertex(T, 'Y'); // 2
    InsertVertex(T, 'Z'); // 3
    
    // 构建树形结构：
    //       R(0)
    //      /    \
    //    X(1)   Y(2)
    //            |
    //           Z(3)
    InsertEdge(T, 0, 1); // R - X
    InsertEdge(T, 0, 2); // R - Y
    InsertEdge(T, 2, 3); // Y - Z
    
    cout << "2. 新建图 T (包含4个顶点, 3条无环边，完全连通)。" << endl;
    cout << "   图 T 是否是一棵树？ " << (isTree(T) ? "✅ 是" : "❌ 否") << " (预期: 是)\n" << endl;

        // ================= 测试 5：深度优先遍历 (DFS) =================
    cout << "========== 测试 5：深度优先遍历 (DFS) ==========" << endl;
    
    // 单次 DFS 测试前，必须重置 visited 数组
    for(int i = 0; i < MaxVertexNum; i++) visited[i] = false;
    cout << "1. 从 A(0) 开始的单次 DFS: ";
    DFS(G, 0); 
    cout << "\n   (预期输出: 0 1 3 2 4。 解释：0->1, 1->3, 3->2, 2->4，一条路走到黑)\n" << endl;

    cout << "2. 对整个非连通图 G 进行 DFSTraverse 全局遍历: ";
    DFSTraverse(G); 
    cout << "\n   (预期输出: 0 1 3 2 4 5。孤立点 5 会在主循环中被触发)\n" << endl;
    
        // ================= 测试 6：BFS 单源最短路径 =================
    cout << "========== 测试 6：BFS 单源最短路径 ==========" << endl;
    
    int start_v = 0; // 我们选择从 A(0) 开始测试
    cout << "正在计算从顶点 " << G.Vex[start_v] << " 到其余各顶点的最短路径...\n" << endl;
    
    // 1. 调用算法计算 d 和 path 数组
    BFS_Min_Distance(G, start_v);
    
    // 2. 遍历所有顶点，打印距离并回溯路径
    for(int i = 0; i < G.vexnum; i++)
    {
        cout << "目标顶点 " << G.Vex[i] << " : ";
        
        if(d[i] == -1)
        {
            cout << "无法到达 (图不连通)" << endl;
        }
        else if (i == start_v)
        {
            cout << "距离 = 0 (这是起点自身)" << endl;
        }
        else
        {
            cout << "距离 = " << d[i] << "， 最短路径轨迹: ";
            
            // 利用 path 数组逆向回溯路径
            int route[MaxVertexNum]; // 用于临时存放路径节点
            int count = 0;           // 记录路径长度
            int curr = i;            // 从目标节点开始往回找
            
            // 只要没回到起点前（起点的 path 是 -1），就一直回溯
            while(curr != -1)
            {
                route[count++] = curr; // 将当前节点加入轨迹
                curr = path[curr];     // 顺藤摸瓜找到它是从哪个节点过来的
            }
            
            // 因为是逆向回溯的，所以我们需要倒序打印，才能得到正向的 A -> ... 路径
            for(int j = count - 1; j >= 0; j--)
            {
                cout << G.Vex[route[j]];
                if(j > 0) cout << " -> "; // 不是最后一个节点就打印箭头
            }
            cout << endl;
        }
    }
    cout << endl;

    return 0;
}