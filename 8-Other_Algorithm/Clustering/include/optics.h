/**
 * optics.h — OPTICS 聚类 (Ordering Points To Identify Clustering Structure)
 *
 * 核心思想: 生成点的有序列表和可达距离图, 不显式生成簇
 *   1. 对每个未访问点, 计算核心距离和可达距离
 *   2. 按可达距离升序处理邻域点
 *   3. 从可达距离图中通过阈值提取簇
 *
 * 核心距离: 点到其第(minPts-1)近邻的距离
 * 可达距离: max(核心距离, 当前点与目标点的距离)
 *
 * 优势: 能发现任意形状和不同密度的簇, 对参数不敏感
 *
 * 参考文献: Ankerst et al. (1999) "OPTICS: Ordering Points To Identify
 *           the Clustering Structure"
 */
#ifndef OPTICS_H
#define OPTICS_H

#include <cmath>
#include <limits>

template<int DIM, int SIZE>
class OPTICS {
    double eps;
    int minPts;
    const double (*features)[DIM];
    int* labels;
    bool processed[SIZE];
    double reachability[SIZE]; // 可达距离
    double core_dist[SIZE];    // 核心距离
    int order[SIZE];           // 处理顺序
    int order_cnt;

    double dist2(int i, int j) const {
        double s = 0;
        for (int d = 0; d < DIM; d++) {
            double diff = features[i][d] - features[j][d];
            s += diff * diff;
        }
        return s;
    }

    // 计算点到其第(minPts-1)近邻的距离
    double compute_core_distance(int p, int neighbors[], int ncnt) {
        if (ncnt < minPts) return -1; // 不是核心点
        // 简单选择排序找到第 minPts 个距离
        double dists[SIZE];
        for (int i = 0; i < ncnt; i++)
            dists[i] = dist2(p, neighbors[i]);
        // 找第 minPts 小的
        for (int i = 0; i < minPts; i++) {
            int min_idx = i;
            for (int j = i + 1; j < ncnt; j++)
                if (dists[j] < dists[min_idx]) min_idx = j;
            double tmp = dists[i];
            dists[i] = dists[min_idx];
            dists[min_idx] = tmp;
        }
        return std::sqrt(dists[minPts - 1]); // 开根号
    }

    void neighbors_and_sort(int p, int neighbors[], int& ncnt) {
        ncnt = 0;
        double eps2 = eps * eps;
        for (int i = 0; i < SIZE; i++) {
            if (dist2(p, i) <= eps2) neighbors[ncnt++] = i;
        }
    }

public:
    OPTICS(double e, int mp, const double (*feat)[DIM], int* lbl)
        : eps(e), minPts(mp), features(feat), labels(lbl) {
        for (int i = 0; i < SIZE; i++) {
            processed[i] = false;
            reachability[i] = std::numeric_limits<double>::max();
        }
        order_cnt = 0;
    }

    void run() {
        for (int i = 0; i < SIZE; i++) labels[i] = -1;

        for (int p = 0; p < SIZE; p++) {
            if (processed[p]) continue;
            processed[p] = true;
            order[order_cnt++] = p;

            int neighbors[SIZE]; int ncnt;
            neighbors_and_sort(p, neighbors, ncnt);

            core_dist[p] = compute_core_distance(p, neighbors, ncnt);

            if (core_dist[p] != -1) { // 如果是核心点
                // 使用优先队列 (这里用简单插入排序近似)
                int seeds[SIZE]; int seed_cnt = 0;
                for (int i = 0; i < ncnt; i++) {
                    int q = neighbors[i];
                    if (processed[q]) continue;
                    double new_r = (core_dist[p] > std::sqrt(dist2(p, q)))
                                   ? core_dist[p] : std::sqrt(dist2(p, q));
                    if (new_r < reachability[q]) {
                        reachability[q] = new_r;
                        // 插入 seeds (按可达距离排序)
                        int pos = seed_cnt;
                        while (pos > 0 && reachability[seeds[pos - 1]] > reachability[q]) {
                            seeds[pos] = seeds[pos - 1]; pos--;
                        }
                        seeds[pos] = q;
                        if (pos == seed_cnt) seed_cnt++;
                    }
                }

                while (seed_cnt > 0) {
                    int q = seeds[0];
                    // 移除第一个
                    for (int s = 1; s < seed_cnt; s++) seeds[s - 1] = seeds[s];
                    seed_cnt--;

                    processed[q] = true;
                    order[order_cnt++] = q;

                    int q_nb[SIZE]; int q_cnt;
                    neighbors_and_sort(q, q_nb, q_cnt);
                    core_dist[q] = compute_core_distance(q, q_nb, q_cnt);

                    if (core_dist[q] != -1) {
                        for (int i = 0; i < q_cnt; i++) {
                            int r = q_nb[i];
                            if (processed[r]) continue;
                            double new_r = (core_dist[q] > std::sqrt(dist2(q, r)))
                                           ? core_dist[q] : std::sqrt(dist2(q, r));
                            if (new_r < reachability[r]) {
                                reachability[r] = new_r;
                                int pos = seed_cnt;
                                while (pos > 0 && reachability[seeds[pos - 1]] > reachability[r]) {
                                    seeds[pos] = seeds[pos - 1]; pos--;
                                }
                                seeds[pos] = r;
                                if (pos == seed_cnt) seed_cnt++;
                            }
                        }
                    }
                }
            }
        }
    }

    // 从可达距离图中提取簇 (使用阈值 xi)
    void extract_clusters(double xi = 0.05) {
        int cluster_id = 0;
        bool in_cluster = false;

        for (int i = 0; i < order_cnt; i++) {
            int p = order[i];
            if (reachability[p] > xi || core_dist[p] == -1) {
                in_cluster = false;
                labels[p] = -1;
            } else {
                if (!in_cluster) { in_cluster = true; cluster_id++; }
                labels[p] = cluster_id - 1;
            }
        }

        // 噪声点标记
        for (int i = 0; i < SIZE; i++)
            if (labels[i] == -1 || labels[i] >= cluster_id)
                labels[i] = -1;
    }
};

#endif
