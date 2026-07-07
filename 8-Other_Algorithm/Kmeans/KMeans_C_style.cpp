/**
 * ============================================================
 *  球面 K-Means 聚类算法（C 风格实现）
 * ============================================================
 *
 *  算法简介：
 *    球面 K-Means 是 K-Means 的变体，适用于方向敏感、模长无关
 *    的聚类场景（如文本向量、基因表达数据）。
 *
 *  核心思想：
 *    1. 所有数据点被视为单位球面上的点
 *    2. 用【余弦相似度】代替欧氏距离来衡量"远近"
 *    3. 每次更新簇中心后，将中心向量【归一化】回单位球面
 *
 *  与普通 K-Means 的区别：
 *    - 普通 K-Means: 欧氏距离 + 算术均值中心
 *    - 球面 K-Means: 余弦相似度 + 归一化均值中心
 *
 *  算法流程：
 *    1. 随机初始化 K 个簇中心
 *    2. 重复迭代直到收敛（或达到最大迭代次数）：
 *       a. 分配步骤：每个点分配到余弦相似度最大的簇
 *       b. 更新步骤：重新计算每个簇的中心（均值 + 归一化）
 *    3. 输出聚类结果
 * ============================================================
 */

#include <iostream>
#include <random>
#include <cmath>
#include <limits>
#include <vector>
using namespace std;

// ==================== 常量定义 ====================

#define DIMENSIONS 100    // 每个数据点的特征维度数
#define MAX_SIZE   100    // 数据点总数

// ==================== 数据结构 ====================

/**
 * SinglePoint —— 单个高维数据点
 *
 * feature[] : 特征向量，长度为 DIMENSIONS
 * clusterNum: 该点所属的簇编号
 *   - 初始值 -1：未分配
 *   - 数值 0~K-1：分配到对应簇
 */
struct SinglePoint {
    double feature[DIMENSIONS];
    int clusterNum;
};

/**
 * 将 SinglePoint 的所有特征置零，簇编号设为 -2（未初始化）。
 * 用于临时求和变量或占位符的初始化。
 */
void InitSinglePoint(SinglePoint& a) {
    for (int i = 0; i < DIMENSIONS; ++i) {
        a.feature[i] = 0;
    }
    a.clusterNum = -2;
}

/**
 * PointSet —— 数据集
 *
 * 包含 MAX_SIZE 个 SinglePoint，代表整个待聚类的数据集。
 */
struct PointSet {
    SinglePoint point[MAX_SIZE];
};

// ==================== 数据初始化 ====================

/**
 * 随机初始化所有数据点的特征值
 *
 * 每个维度的值从 [0, 1) 均匀分布中采样。
 * 全部点的 clusterNum 初始化为 -1（未分配）。
 *
 * 随机数生成栈：
 *   random_device → mt19937 → uniform_real_distribution
 */
void InitPoints(PointSet& a) {
    std::random_device rd;                     // 真随机数种子源
    std::mt19937 gen(rd());                    // Mersenne Twister 引擎
    std::uniform_real_distribution<> dis(0.0, 1.0);  // [0, 1) 均匀分布

    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < DIMENSIONS; ++j) {
            a.point[i].clusterNum = -1;         // 初始化为"未分配"
            a.point[i].feature[j] = dis(gen);   // 随机生成特征值
        }
    }
}

// ==================== 核心计算函数 ====================

/**
 * 计算两个向量 a 和 b 的余弦相似度
 *
 * 公式: cos(θ) = (A · B) / (||A|| × ||B||)
 *
 * 返回值范围 [-1, 1]:
 *   1 : 方向完全相同
 *   0 : 正交（无相关性）
 *  -1 : 方向完全相反
 *
 * 若任一向量为零向量（范数为 0），返回 0。
 */
double CosineSimilarity(const SinglePoint& a, const SinglePoint& b) {
    double dotProduct = 0;   // 点积 A · B
    double normA = 0;        // ||A||²
    double normB = 0;        // ||B||²

    for (int k = 0; k < DIMENSIONS; ++k) {
        dotProduct += a.feature[k] * b.feature[k];
        normA += a.feature[k] * a.feature[k];
        normB += b.feature[k] * b.feature[k];
    }

    double denom = sqrt(normA) * sqrt(normB);  // ||A|| × ||B||
    if (denom == 0)
        return 0;   // 零向量，无法计算方向，返回 0
    return dotProduct / denom;
}

// ==================== 簇分配与更新 ====================

/**
 * 【分配步骤】将每个数据点分配到最近（相似度最高）的簇中心
 *
 * 对每个点，遍历所有 K 个簇中心，计算余弦相似度，
 * 选择相似度最大的簇作为其所属簇。
 *
 * 注意：使用 negative infinity 初始化 maxCosineSimilarity，
 * 保证任意实际相似度都能覆盖初始值。
 */
void UpgradePointsCluster(PointSet& points, SinglePoint cluster[], int clusters) {
    for (int i = 0; i < MAX_SIZE; ++i) {
        double maxCosineSimilarity = -numeric_limits<double>::infinity(); // 负无穷
        int bestCluster = 0;

        // 遍历所有簇中心，找相似度最高的那个
        for (int j = 0; j < clusters; ++j) {
            double sim = CosineSimilarity(points.point[i], cluster[j]);
            if (sim > maxCosineSimilarity) {
                maxCosineSimilarity = sim;
                bestCluster = j;
            }
        }
        // 将点分配到最佳簇
        points.point[i].clusterNum = bestCluster;
    }
}

/**
 * 【更新步骤】重新计算每个簇的中心（球面 K-Means 核心）
 *
 * 对每个簇 c:
 *   1. 找出所有属于簇 c 的点
 *   2. 计算这些点的特征均值（算术平均）
 *   3. 将均值向量归一化为单位向量（投影到球面上）
 *
 * 空簇处理：若某簇没有分配到任何点（count == 0），
 *           则随机重新初始化该簇的中心。
 */
void UpgradeClusterCenter(PointSet& points, SinglePoint cluster[], int clusters) {
    for (int c = 0; c < clusters; c++) {
        // ---- 第 1 步：累加属于簇 c 的所有点的特征 ----
        SinglePoint sum;
        InitSinglePoint(sum);
        int count = 0;

        for (int i = 0; i < MAX_SIZE; ++i) {
            if (points.point[i].clusterNum == c) {
                for (int j = 0; j < DIMENSIONS; j++) {
                    sum.feature[j] += points.point[i].feature[j];
                }
                count++;
            }
        }

        // ---- 第 2 步：根据 count 更新中心 ----
        if (count == 0) {
            // 空簇：随机重新初始化，避免陷入退化
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0, 1.0);
            for (int k = 0; k < DIMENSIONS; k++) {
                cluster[c].feature[k] = dis(gen);
            }
        }
        else {
            // ---- 第 2a 步：计算算术均值（普通 K-Means 到这里就结束了） ----
            for (int k = 0; k < DIMENSIONS; k++) {
                cluster[c].feature[k] = sum.feature[k] / count;
            }

            // ---- 第 2b 步：【球面 K-Means 关键】将均值归一化为单位向量 ----
            // 若不归一化，更新后的中心可能偏离球面，
            // 后续迭代的余弦相似度计算就会越来越不准确。
            double norm = 0;
            for (int k = 0; k < DIMENSIONS; k++) {
                norm += cluster[c].feature[k] * cluster[c].feature[k];
            }
            norm = sqrt(norm);
            if (norm > 0) {
                for (int k = 0; k < DIMENSIONS; k++) {
                    cluster[c].feature[k] /= norm;
                }
            }
        }
    }
}

// ==================== K-Means 主函数 ====================

/**
 * 球面 K-Means 聚类主函数
 *
 * 参数:
 *   points   - 待聚类的数据集（原地修改 clusterNum）
 *   clusters - 目标簇数 K
 *
 * 流程:
 *   1. 随机初始化 K 个簇中心
 *   2. 进入迭代循环：
 *      a. 保存上轮分配结果
 *      b. 执行【分配步骤】
 *      c. 检查收敛（分配结果不再变化）
 *      d. 执行【更新步骤】
 *   3. 达到最大迭代次数 MAX_ITERS 或收敛后退出
 */
void KMeans(PointSet& points, int clusters) {
    // ---- 1. 初始化 K 个簇中心 ----
    vector<SinglePoint> clusterVec(clusters);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < clusters; ++i) {
        for (int j = 0; j < DIMENSIONS; ++j) {
            clusterVec[i].clusterNum = -2;    // 标记为簇中心（非数据点）
            clusterVec[i].feature[j] = dis(gen);
        }
    }

    const int MAX_ITERS = 100;                 // 最大迭代次数（防止死循环）
    int oldClusterNum[MAX_SIZE];               // 记录上轮迭代的簇分配（用于收敛判断）

    // ---- 2. 迭代优化 ----
    for (int iter = 0; iter < MAX_ITERS; iter++) {
        // 2a. 保存本轮迭代前的分配状态
        for (int i = 0; i < MAX_SIZE; i++) {
            oldClusterNum[i] = points.point[i].clusterNum;
        }

        // 2b. 分配步骤：将每个点分配到最相似的簇
        UpgradePointsCluster(points, clusterVec.data(), clusters);

        // 2c. 检查收敛：所有点的簇归属是否都没有变化
        bool converged = true;
        for (int i = 0; i < MAX_SIZE; i++) {
            if (points.point[i].clusterNum != oldClusterNum[i]) {
                converged = false;
                break;
            }
        }

        if (converged) {
            cout << "经过 " << (iter + 1) << " 次迭代后收敛!" << endl;
            break;
        }

        // 2d. 更新步骤：重新计算每个簇的中心
        UpgradeClusterCenter(points, clusterVec.data(), clusters);
    }
}

// ==================== 主程序 ====================

int main()
{
    cout << "========== 球面 K-Means 聚类测试 ==========" << endl;
    cout << "维度: " << DIMENSIONS << ", 数据点数: " << MAX_SIZE << endl;

    // ---- 初始化数据 ----
    PointSet data;
    InitPoints(data);

    const int K = 3;    // 设定聚类簇数
    cout << "目标聚类数: " << K << endl;

    // ---- 执行聚类 ----
    KMeans(data, K);

    // ---- 统计并输出结果 ----
    int count[K] = {0};
    for (int i = 0; i < MAX_SIZE; i++) {
        int c = data.point[i].clusterNum;
        if (c >= 0 && c < K)
            count[c]++;
    }

    cout << "\n各簇包含的点数:" << endl;
    for (int c = 0; c < K; c++) {
        cout << "  簇 " << c << ": " << count[c] << " 个点" << endl;
    }

    cout << "\n聚类完成!" << endl;
    return 0;
}
