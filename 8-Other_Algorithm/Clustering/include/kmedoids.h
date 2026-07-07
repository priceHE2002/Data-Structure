/**
 * kmedoids.h — K-Medoids (PAM) 聚类
 *
 * 核心思想: 用实际数据点作为簇中心(medoid), 而非均值
 *   1. BUILD: 贪心选择初始 medoids
 *   2. SWAP: 迭代尝试用非 medoid 点替换 medoid, 若总代价下降则接受
 *
 * 优势: 对离群点鲁棒, 适用于任意距离度量
 * 参考文献: Kaufman & Rousseeuw (1987) "Clustering by means of medoids"
 */
#ifndef KMEDOIDS_H
#define KMEDOIDS_H

#include <cmath>
#include <limits>
#include <random>

template<int DIM, int SIZE>
class KMedoids {
    int K;
    const double (*features)[DIM];
    int* labels;
    int medoids[10];
    std::mt19937 rng;

    double dist(int i, int j) const {
        double s = 0;
        for (int d = 0; d < DIM; d++) {
            double diff = features[i][d] - features[j][d];
            s += diff * diff;
        }
        return s; // 用平方欧氏距离, 无需开根号
    }

    void init_medoids() {
        // BUILD 阶段: 贪心选择
        bool chosen[SIZE] = {false};
        // 第一个 medoid: 选到所有其他点距离之和最小的点
        std::uniform_int_distribution<> dis0(0, SIZE - 1);
        medoids[0] = dis0(rng);
        chosen[medoids[0]] = true;

        for (int k = 1; k < K; k++) {
            int best = -1;
            double best_gain = -std::numeric_limits<double>::max();
            for (int i = 0; i < SIZE; i++) {
                if (chosen[i]) continue;
                double gain = 0;
                for (int j = 0; j < SIZE; j++) {
                    if (chosen[j]) continue;
                    // 当前分配给 j 的最近 medoid 距离
                    double cur_min = dist(j, medoids[0]);
                    for (int m = 1; m < k; m++) {
                        double d = dist(j, medoids[m]);
                        if (d < cur_min) cur_min = d;
                    }
                    double d_new = dist(j, i);
                    if (d_new < cur_min) gain += cur_min - d_new;
                }
                if (gain > best_gain) { best_gain = gain; best = i; }
            }
            medoids[k] = best;
            chosen[best] = true;
        }
    }

    void assign() {
        for (int i = 0; i < SIZE; i++) {
            double min_d = std::numeric_limits<double>::max();
            int best = 0;
            for (int j = 0; j < K; j++) {
                double d = dist(i, medoids[j]);
                if (d < min_d) { min_d = d; best = j; }
            }
            labels[i] = best;
        }
    }

    double total_cost() const {
        double cost = 0;
        for (int i = 0; i < SIZE; i++)
            cost += dist(i, medoids[labels[i]]);
        return cost;
    }

    void swap() {
        assign();
        double cur_cost = total_cost();
        bool changed = true;
        while (changed) {
            changed = false;
            for (int k = 0; k < K; k++) {
                for (int i = 0; i < SIZE; i++) {
                    bool is_medoid = false;
                    for (int m = 0; m < K; m++)
                        if (medoids[m] == i) { is_medoid = true; break; }
                    if (is_medoid) continue;
                    int old_medoid = medoids[k];
                    medoids[k] = i;
                    assign();
                    double new_cost = total_cost();
                    if (new_cost < cur_cost) {
                        cur_cost = new_cost;
                        changed = true;
                        break;
                    } else {
                        medoids[k] = old_medoid;
                        assign();
                    }
                }
                if (changed) break;
            }
        }
    }

public:
    KMedoids(int k, const double (*feat)[DIM], int* lbl)
        : K(k), features(feat), labels(lbl) {
        std::random_device rd; rng.seed(rd());
    }

    void run() {
        init_medoids();
        swap();
        assign();
    }
};

#endif
