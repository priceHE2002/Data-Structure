/**
 * hierarchical.h — Agglomerative Hierarchical Clustering
 *
 * 自底向上凝聚层次聚类:
 *   1. 每个点初始为一个簇
 *   2. 每次合并距离最近的两个簇
 *   3. 重复直到剩余 K 个簇
 *
 * 支持 linkage 策略:
 *   - single (最小距离)
 *   - complete (最大距离)
 *   - average (平均距离)
 *
 * 参考文献: Johnson (1967) "Hierarchical Clustering Schemes"
 */
#ifndef HIERARCHICAL_H
#define HIERARCHICAL_H

#include <cmath>
#include <limits>

enum Linkage { SINGLE, COMPLETE, AVERAGE };

template<int DIM, int SIZE>
class Hierarchical {
    int K;
    Linkage linkage;
    const double (*features)[DIM];
    int* labels;
    double dist_mat[SIZE][SIZE]; // 预计算距离矩阵

    double dist2(int i, int j) const {
        double s = 0;
        for (int d = 0; d < DIM; d++) {
            double diff = features[i][d] - features[j][d];
            s += diff * diff;
        }
        return s;
    }

    // 计算两个簇之间的距离
    double cluster_dist(const int cluster[], int c1, int c2, int cur_n) const {
        double d = (linkage == SINGLE) ? std::numeric_limits<double>::max() : 0;
        bool first = true;
        for (int i = 0; i < cur_n; i++) {
            if (cluster[i] != c1) continue;
            for (int j = 0; j < cur_n; j++) {
                if (cluster[j] != c2) continue;
                double dij = dist_mat[i][j];
                if (linkage == SINGLE) {
                    if (dij < d) d = dij;
                } else if (linkage == COMPLETE) {
                    if (dij > d) d = dij;
                } else { // AVERAGE
                    d += dij;
                    first = false;
                }
            }
        }
        if (linkage == AVERAGE && !first) {
            int cnt1 = 0, cnt2 = 0;
            for (int i = 0; i < cur_n; i++) {
                if (cluster[i] == c1) cnt1++;
                if (cluster[i] == c2) cnt2++;
            }
            if (cnt1 * cnt2 > 0) d /= (cnt1 * cnt2);
        }
        return d;
    }

public:
    Hierarchical(int k, Linkage l, const double (*feat)[DIM], int* lbl)
        : K(k), linkage(l), features(feat), labels(lbl) {}

    void run() {
        // 预计算距离矩阵
        for (int i = 0; i < SIZE; i++)
            for (int j = i + 1; j < SIZE; j++)
                dist_mat[i][j] = dist_mat[j][i] = dist2(i, j);

        // 初始化: 每个点一个簇
        int cluster[SIZE];
        int num_clusters = SIZE;
        for (int i = 0; i < SIZE; i++) cluster[i] = i;

        // 凝聚合并
        while (num_clusters > K) {
            double min_d = std::numeric_limits<double>::max();
            int merge_a = -1, merge_b = -1;

            // 维护活跃簇列表
            bool active[SIZE] = {false};
            for (int i = 0; i < SIZE; i++) active[cluster[i]] = true;

            for (int ca = 0; ca < SIZE; ca++) {
                if (!active[ca]) continue;
                for (int cb = ca + 1; cb < SIZE; cb++) {
                    if (!active[cb]) continue;
                    double d = cluster_dist(cluster, ca, cb, SIZE);
                    if (d < min_d) { min_d = d; merge_a = ca; merge_b = cb; }
                }
            }

            if (merge_a == -1) break;

            // 合并: 将 merge_b 合并到 merge_a
            for (int i = 0; i < SIZE; i++)
                if (cluster[i] == merge_b) cluster[i] = merge_a;

            num_clusters--;
        }

        // 将簇 ID 映射到 0..K-1
        int mapping[SIZE];
        int next_id = 0;
        for (int i = 0; i < SIZE; i++) mapping[i] = -1;
        for (int i = 0; i < SIZE; i++) {
            if (mapping[cluster[i]] == -1)
                mapping[cluster[i]] = next_id++;
            labels[i] = mapping[cluster[i]];
        }
    }
};

#endif
