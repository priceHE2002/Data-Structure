/**
 * dbscan.h — DBSCAN 密度聚类
 *
 * 核心概念:
 *   eps    : 邻域半径
 *   minPts : 核心点的最小邻域点数
 *
 * 点的分类:
 *   - 核心点: 邻域内点数 >= minPts
 *   - 边界点: 邻域内点数 < minPts, 但在某核心点的邻域内
 *   - 噪声点: 既不是核心点也不是边界点 (-1 表示)
 *
 * 参考文献: Ester et al. (1996) "A Density-Based Algorithm for Discovering
 *           Clusters in Large Spatial Databases with Noise"
 */
#ifndef DBSCAN_H
#define DBSCAN_H

#include <cmath>

template<int DIM, int SIZE>
class DBSCAN {
    double eps;
    int minPts;
    const double (*features)[DIM];
    int* labels;
    bool visited[SIZE];
    int cluster_id;

    double dist2(int i, int j) const {
        double s = 0;
        for (int d = 0; d < DIM; d++) {
            double diff = features[i][d] - features[j][d];
            s += diff * diff;
        }
        return s;
    }

    void region_query(int p, int neighbors[], int& cnt) const {
        cnt = 0;
        double eps2 = eps * eps;
        for (int i = 0; i < SIZE; i++) {
            if (dist2(p, i) <= eps2) neighbors[cnt++] = i;
        }
    }

    // 递归扩展簇 (DFS)
    void expand_cluster(int p, int neighbors[], int ncnt) {
        labels[p] = cluster_id;
        for (int ni = 0; ni < ncnt; ni++) {
            int q = neighbors[ni];
            if (!visited[q]) {
                visited[q] = true;
                int q_neighbors[SIZE]; int q_cnt;
                region_query(q, q_neighbors, q_cnt);
                if (q_cnt >= minPts) {
                    // 合并邻域
                    for (int qi = 0; qi < q_cnt; qi++) {
                        bool found = false;
                        for (int xi = 0; xi < ncnt; xi++)
                            if (neighbors[xi] == q_neighbors[qi]) { found = true; break; }
                        if (!found && ncnt < SIZE) neighbors[ncnt++] = q_neighbors[qi];
                    }
                }
            }
            if (labels[q] == -1) labels[q] = cluster_id;
        }
    }

public:
    DBSCAN(double e, int mp, const double (*feat)[DIM], int* lbl)
        : eps(e), minPts(mp), features(feat), labels(lbl) {
        for (int i = 0; i < SIZE; i++) visited[i] = false;
    }

    void run() {
        cluster_id = 0;
        for (int i = 0; i < SIZE; i++) labels[i] = -1; // 初始化为噪声

        for (int i = 0; i < SIZE; i++) {
            if (visited[i]) continue;
            visited[i] = true;

            int neighbors[SIZE]; int cnt;
            region_query(i, neighbors, cnt);

            if (cnt < minPts) {
                labels[i] = -1; // 标记为噪声
            } else {
                expand_cluster(i, neighbors, cnt);
                cluster_id++;
            }
        }
    }
};

#endif
