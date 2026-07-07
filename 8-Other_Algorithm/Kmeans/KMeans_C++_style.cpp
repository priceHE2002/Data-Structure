/**
 * 球面 K-Means 聚类 — 使用 Iris 真实数据集
 * 数据集: UCI Iris (150 样本, 4 特征, 3 类别)
 */

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <limits>
using namespace std;

const int DIMENSIONS = 4;   // 花萼长, 花萼宽, 花瓣长, 花瓣宽
const int DATA_SIZE   = 150; // Iris 总样本数
const int MAX_ITERS   = 100;

class Point {
public:
    vector<double> features; // 特征向量
    int clusterId;           // 簇编号: -1=未分配, 0~K-1=已分配
    int trueLabel;           // 真实标签（仅用于评估聚类效果）

    Point() : features(DIMENSIONS, 0.0), clusterId(-1), trueLabel(-1) {}
};

// 归一化为单位向量（投影到球面）
void Normalize(vector<double>& v) {
    double sq = 0;
    for (int i = 0; i < DIMENSIONS; i++) sq += v[i] * v[i];
    double norm = sqrt(sq);
    if (norm > 0)
        for (int i = 0; i < DIMENSIONS; i++) v[i] /= norm;
}

// 余弦相似度 cos(θ) = (A·B) / (||A|| × ||B||)
double CosineSimilarity(const vector<double>& a, const vector<double>& b) {
    double dot = 0, na = 0, nb = 0;
    for (int i = 0; i < DIMENSIONS; i++) {
        dot += a[i] * b[i];
        na  += a[i] * a[i];
        nb  += b[i] * b[i];
    }
    double denom = sqrt(na) * sqrt(nb);
    return (denom == 0) ? 0.0 : dot / denom;
}

class SphericalKMeans {
private:
    int K;
    vector<Point> points;
    vector<vector<double>> centroids;
    mt19937 rng;

    // 从 Iris 数据集读取数据（含真实标签）
    void LoadIris() {
        // Iris 数据: {花萼长, 花萼宽, 花瓣长, 花瓣宽, 真实类别(0/1/2)}
        // 来源: UCI Machine Learning Repository
        const double raw[150][5] = {
            {5.1,3.5,1.4,0.2,0},{4.9,3.0,1.4,0.2,0},{4.7,3.2,1.3,0.2,0},
            {4.6,3.1,1.5,0.2,0},{5.0,3.6,1.4,0.2,0},{5.4,3.9,1.7,0.4,0},
            {4.6,3.4,1.4,0.3,0},{5.0,3.4,1.5,0.2,0},{4.4,2.9,1.4,0.2,0},
            {4.9,3.1,1.5,0.1,0},{5.4,3.7,1.5,0.2,0},{4.8,3.4,1.6,0.2,0},
            {4.8,3.0,1.4,0.1,0},{4.3,3.0,1.1,0.1,0},{5.8,4.0,1.2,0.2,0},
            {5.7,4.4,1.5,0.4,0},{5.4,3.9,1.3,0.4,0},{5.1,3.5,1.4,0.3,0},
            {5.7,3.8,1.7,0.3,0},{5.1,3.8,1.5,0.3,0},{5.4,3.4,1.7,0.2,0},
            {5.1,3.7,1.5,0.4,0},{4.6,3.6,1.0,0.2,0},{5.1,3.3,1.7,0.5,0},
            {4.8,3.4,1.9,0.2,0},{5.0,3.0,1.6,0.2,0},{5.0,3.4,1.6,0.4,0},
            {5.2,3.5,1.5,0.2,0},{5.2,3.4,1.4,0.2,0},{4.7,3.2,1.6,0.2,0},
            {4.8,3.1,1.6,0.2,0},{5.4,3.4,1.5,0.4,0},{5.2,4.1,1.5,0.1,0},
            {5.5,4.2,1.4,0.2,0},{4.9,3.1,1.5,0.2,0},{5.0,3.2,1.2,0.2,0},
            {5.5,3.5,1.3,0.2,0},{4.9,3.6,1.4,0.1,0},{4.4,3.0,1.3,0.2,0},
            {5.1,3.4,1.5,0.2,0},{5.0,3.5,1.3,0.3,0},{4.5,2.3,1.3,0.3,0},
            {4.4,3.2,1.3,0.2,0},{5.0,3.5,1.6,0.6,0},{5.1,3.8,1.9,0.4,0},
            {4.8,3.0,1.4,0.3,0},{5.1,3.8,1.6,0.2,0},{4.6,3.2,1.4,0.2,0},
            {5.3,3.7,1.5,0.2,0},{5.0,3.3,1.4,0.2,0},
            {7.0,3.2,4.7,1.4,1},{6.4,3.2,4.5,1.5,1},{6.9,3.1,4.9,1.5,1},
            {5.5,2.3,4.0,1.3,1},{6.5,2.8,4.6,1.5,1},{5.7,2.8,4.5,1.3,1},
            {6.3,3.3,4.7,1.6,1},{4.9,2.4,3.3,1.0,1},{6.6,2.9,4.6,1.3,1},
            {5.2,2.7,3.9,1.4,1},{5.0,2.0,3.5,1.0,1},{5.9,3.0,4.2,1.5,1},
            {6.0,2.2,4.0,1.0,1},{6.1,2.9,4.7,1.4,1},{5.6,2.9,3.6,1.3,1},
            {6.7,3.1,4.4,1.4,1},{5.6,3.0,4.5,1.5,1},{5.8,2.7,4.1,1.0,1},
            {6.2,2.2,4.5,1.5,1},{5.6,2.5,3.9,1.1,1},{5.9,3.2,4.8,1.8,1},
            {6.1,2.8,4.0,1.3,1},{6.3,2.5,4.9,1.5,1},{6.1,2.8,4.7,1.2,1},
            {6.4,2.9,4.3,1.3,1},{6.6,3.0,4.4,1.4,1},{6.8,2.8,4.8,1.4,1},
            {6.7,3.0,5.0,1.7,1},{6.0,2.9,4.5,1.5,1},{5.7,2.6,3.5,1.0,1},
            {5.5,2.4,3.8,1.1,1},{5.5,2.4,3.7,1.0,1},{5.8,2.7,3.9,1.2,1},
            {6.0,2.7,5.1,1.6,1},{5.4,3.0,4.5,1.5,1},{6.0,3.4,4.5,1.6,1},
            {6.7,3.1,4.7,1.5,1},{6.3,2.3,4.4,1.3,1},{5.6,3.0,4.1,1.3,1},
            {5.5,2.5,4.0,1.3,1},{5.5,2.6,4.4,1.2,1},{6.1,3.0,4.6,1.4,1},
            {5.8,2.6,4.0,1.2,1},{5.0,2.3,3.3,1.0,1},{5.6,2.7,4.2,1.3,1},
            {5.7,3.0,4.2,1.2,1},{5.7,2.9,4.2,1.3,1},{6.2,2.9,4.3,1.3,1},
            {5.1,2.5,3.0,1.1,1},{5.7,2.8,4.1,1.3,1},
            {6.3,3.3,6.0,2.5,2},{5.8,2.7,5.1,1.9,2},{7.1,3.0,5.9,2.1,2},
            {6.3,2.9,5.6,1.8,2},{6.5,3.0,5.8,2.2,2},{7.6,3.0,6.6,2.1,2},
            {4.9,2.5,4.5,1.7,2},{7.3,2.9,6.3,1.8,2},{6.7,2.5,5.8,1.8,2},
            {7.2,3.6,6.1,2.5,2},{6.5,3.2,5.1,2.0,2},{6.4,2.7,5.3,1.9,2},
            {6.8,3.0,5.5,2.1,2},{5.7,2.5,5.0,2.0,2},{5.8,2.8,5.1,2.4,2},
            {6.4,3.2,5.3,2.3,2},{6.5,3.0,5.5,1.8,2},{7.7,3.8,6.7,2.2,2},
            {7.7,2.6,6.9,2.3,2},{6.0,2.2,5.0,1.5,2},{6.9,3.2,5.7,2.3,2},
            {5.6,2.8,4.9,2.0,2},{7.7,2.8,6.7,2.0,2},{6.3,2.7,4.9,1.8,2},
            {6.7,3.3,5.7,2.1,2},{7.2,3.2,6.0,1.8,2},{6.2,2.8,4.8,1.8,2},
            {6.1,3.0,4.9,1.8,2},{6.4,2.8,5.6,2.1,2},{7.2,3.0,5.8,1.6,2},
            {7.4,2.8,6.1,1.9,2},{7.9,3.8,6.4,2.0,2},{6.4,2.8,5.6,2.2,2},
            {6.3,2.8,5.1,1.5,2},{6.1,2.6,5.6,1.4,2},{7.7,3.0,6.1,2.3,2},
            {6.3,3.4,5.6,2.4,2},{6.4,3.1,5.5,1.8,2},{6.0,3.0,4.8,1.8,2},
            {6.9,3.1,5.4,2.1,2},{6.7,3.1,5.6,2.4,2},{6.9,3.1,5.1,2.3,2},
            {5.8,2.7,5.1,1.9,2},{6.8,3.2,5.9,2.3,2},{6.7,3.3,5.7,2.5,2},
            {6.7,3.0,5.2,2.3,2},{6.3,2.5,5.0,1.9,2},{6.5,3.0,5.2,2.0,2},
            {6.2,3.4,5.4,2.3,2},{5.9,3.0,5.1,1.8,2},
        };

        for (int i = 0; i < DATA_SIZE; i++) {
            for (int j = 0; j < DIMENSIONS; j++)
                points[i].features[j] = raw[i][j];
            points[i].trueLabel = (int)raw[i][4];
        }
    }

    void InitCentroids() {
        // 从数据点中随机挑选 K 个作为初始中心（避免随机值导致退化解）
        uniform_int_distribution<> dis(0, DATA_SIZE - 1);
        for (int i = 0; i < K; i++) {
            int idx = dis(rng);
            for (int j = 0; j < DIMENSIONS; j++)
                centroids[i][j] = points[idx].features[j];
            // 加入微小扰动避免多个中心完全一致
            uniform_real_distribution<> noise(-0.01, 0.01);
            for (int j = 0; j < DIMENSIONS; j++)
                centroids[i][j] += noise(rng);
            Normalize(centroids[i]);
        }
    }

    void AssignClusters() {
        for (int i = 0; i < DATA_SIZE; i++) {
            double maxSim = -numeric_limits<double>::infinity();
            int best = 0;
            for (int j = 0; j < K; j++) {
                double sim = CosineSimilarity(points[i].features, centroids[j]);
                if (sim > maxSim) { maxSim = sim; best = j; }
            }
            points[i].clusterId = best;
        }
    }

    void UpdateCentroids() {
        for (int c = 0; c < K; c++) {
            vector<double> sum(DIMENSIONS, 0.0);
            int count = 0;
            for (int i = 0; i < DATA_SIZE; i++) {
                if (points[i].clusterId == c) {
                    for (int d = 0; d < DIMENSIONS; d++)
                        sum[d] += points[i].features[d];
                    count++;
                }
            }
            if (count == 0) {
                uniform_real_distribution<> dis(0.0, 1.0);
                for (int d = 0; d < DIMENSIONS; d++)
                    centroids[c][d] = dis(rng);
                Normalize(centroids[c]);
            } else {
                for (int d = 0; d < DIMENSIONS; d++)
                    centroids[c][d] = sum[d] / count;
                Normalize(centroids[c]); // 球面 K-Means 核心：均值后归一化
            }
        }
    }

    bool HasConverged(const vector<int>& old) const {
        for (int i = 0; i < DATA_SIZE; i++)
            if (points[i].clusterId != old[i]) return false;
        return true;
    }

public:
    SphericalKMeans(int k) : K(k), points(DATA_SIZE),
        centroids(k, vector<double>(DIMENSIONS)) {
        random_device rd;
        rng.seed(rd());
    }

    void Run() {
        LoadIris();
        InitCentroids();

        for (int iter = 0; iter < MAX_ITERS; iter++) {
            vector<int> old;
            for (int i = 0; i < DATA_SIZE; i++)
                old.push_back(points[i].clusterId);

            AssignClusters();

            if (HasConverged(old)) {
                cout << "经过 " << (iter + 1) << " 次迭代后收敛!" << endl;
                break;
            }
            UpdateCentroids();
        }
    }

    // 输出聚类结果并与真实标签对比
    void PrintResults() const {
        const char* species[] = {"setosa", "versicolor", "virginica"};

        // 统计每个簇中真实类别的分布
        int confusion[3][3] = {{0}}; // confusion[簇编号][真实标签]
        for (int i = 0; i < DATA_SIZE; i++)
            confusion[points[i].clusterId][points[i].trueLabel]++;

        cout << "\n===== 聚类结果 vs 真实标签 =====" << endl;
        cout << "簇\\真实 | setosa  versicolor  virginica | 总数" << endl;
        cout << "--------+-------------------------------+-----" << endl;
        for (int c = 0; c < K; c++) {
            int total = 0;
            printf("  %d     |", c);
            for (int l = 0; l < 3; l++) {
                printf("  %8d", confusion[c][l]);
                total += confusion[c][l];
            }
            printf("   | %3d\n", total);
        }

        // 计算聚类纯度
        int correct = 0, bestMatch[3] = {-1,-1,-1};
        bool used[3] = {false};
        // 贪心匹配：每个簇对应数量最多的真实标签
        for (int c = 0; c < K; c++) {
            int maxCnt = 0, bestLabel = 0;
            for (int l = 0; l < 3; l++) {
                if (!used[l] && confusion[c][l] > maxCnt) {
                    maxCnt = confusion[c][l];
                    bestLabel = l;
                }
            }
            if (maxCnt > 0) {
                bestMatch[c] = bestLabel;
                used[bestLabel] = true;
                correct += maxCnt;
            }
        }

        cout << "\n聚类纯度: " << correct << " / " << DATA_SIZE
             << " = " << (100.0 * correct / DATA_SIZE) << "%" << endl;

        // 输出每个簇的物种标签
        cout << "\n簇 → 物种映射:" << endl;
        for (int c = 0; c < K; c++) {
            cout << "  簇 " << c << " → ";
            if (bestMatch[c] >= 0)
                cout << species[bestMatch[c]];
            else
                cout << "无匹配";
            cout << endl;
        }
    }
};

int main() {
    cout << "========== 球面 K-Means — Iris 数据集 ==========" << endl;

    const int K = 3; // Iris 有 3 个物种
    SphericalKMeans skm(K);
    skm.Run();
    skm.PrintResults();

    cout << "\n聚类完成!" << endl;
    return 0;
}
