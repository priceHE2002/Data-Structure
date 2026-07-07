/**
 * kmeans_plusplus.h — K-Means++ 聚类
 *
 * 核心改进: 使用 K-Means++ 初始化策略选择初始中心
 *   1. 随机选第一个中心
 *   2. 对每个后续中心, 以与已有中心的最短距离平方为权重采样
 *   3. 使初始中心分散, 避免收敛到局部最优
 *
 * 参考文献: Arthur & Vassilvitskii (2007) "k-means++: The Advantages of Careful Seeding"
 */
#ifndef KMEANS_PLUSPLUS_H
#define KMEANS_PLUSPLUS_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <random>

inline double squared_euclidean(const double a[], const double b[], int dim) {
    double s = 0;
    for (int i = 0; i < dim; i++) { double d = a[i] - b[i]; s += d * d; }
    return s;
}

template<int DIM, int SIZE>
class KMeansPlusPlus {
    int K;
    const double (*features)[DIM];
    int* labels;
    double centroids[10][DIM];
    std::mt19937 rng;

    void init_centroids() {
        // Step 1: 随机选第一个中心
        std::uniform_int_distribution<> dis0(0, SIZE - 1);
        int idx = dis0(rng);
        for (int d = 0; d < DIM; d++) centroids[0][d] = features[idx][d];

        // Step 2: K-Means++ 采样后续中心
        double min_dist_sq[SIZE];
        for (int k = 1; k < K; k++) {
            double total_w = 0;
            for (int i = 0; i < SIZE; i++) {
                double md = squared_euclidean(features[i], centroids[0], DIM);
                for (int j = 1; j < k; j++) {
                    double d = squared_euclidean(features[i], centroids[j], DIM);
                    if (d < md) md = d;
                }
                min_dist_sq[i] = md;
                total_w += md;
            }
            // 按权重采样
            std::uniform_real_distribution<> dis(0, total_w);
            double r = dis(rng);
            double cum = 0;
            int pick = 0;
            for (int i = 0; i < SIZE; i++) {
                cum += min_dist_sq[i];
                if (cum >= r) { pick = i; break; }
            }
            for (int d = 0; d < DIM; d++) centroids[k][d] = features[pick][d];
        }
    }

    void assign() {
        for (int i = 0; i < SIZE; i++) {
            double min_d = std::numeric_limits<double>::max();
            int best = 0;
            for (int j = 0; j < K; j++) {
                double d = squared_euclidean(features[i], centroids[j], DIM);
                if (d < min_d) { min_d = d; best = j; }
            }
            labels[i] = best;
        }
    }

    void update() {
        for (int c = 0; c < K; c++) {
            double sum[DIM] = {0}; int cnt = 0;
            for (int i = 0; i < SIZE; i++)
                if (labels[i] == c) {
                    for (int d = 0; d < DIM; d++) sum[d] += features[i][d];
                    cnt++;
                }
            if (cnt > 0)
                for (int d = 0; d < DIM; d++) centroids[c][d] = sum[d] / cnt;
        }
    }

public:
    KMeansPlusPlus(int k, const double (*feat)[DIM], int* lbl)
        : K(k), features(feat), labels(lbl) {
        std::random_device rd; rng.seed(rd());
    }

    void run(int max_iter = 100) {
        init_centroids();
        for (int iter = 0; iter < max_iter; iter++) {
            int old[SIZE];
            for (int i = 0; i < SIZE; i++) old[i] = labels[i];
            assign();
            bool ok = true;
            for (int i = 0; i < SIZE; i++)
                if (labels[i] != old[i]) { ok = false; break; }
            if (ok) break;
            update();
        }
    }
};

#endif
