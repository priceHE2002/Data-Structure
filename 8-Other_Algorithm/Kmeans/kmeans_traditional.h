/**
 * kmeans_traditional.h — 传统 K-Means（欧氏距离）
 *
 * 聚类步骤:
 *   1. 随机选 K 个点作为初始中心
 *   2. 分配: 每个点归入欧氏距离最近的中心
 *   3. 更新: 重新计算每簇的算术均值作为新中心
 *   4. 重复步骤 2-3 直到收敛
 */

#ifndef KMEANS_TRADITIONAL_H
#define KMEANS_TRADITIONAL_H

#include <cmath>
#include <limits>
#include <random>

// 欧氏距离: d = sqrt(Σ (a_i - b_i)²)
inline double euclidean(const double a[], const double b[], int dim) {
    double s = 0;
    for (int i = 0; i < dim; i++) { double d = a[i] - b[i]; s += d * d; }
    return std::sqrt(s);
}

template<int DIM, int SIZE>
class TraditionalKMeans {
    int K;
    const double (*features)[DIM];  // 指向外部数据的指针（不拥有数据）
    int* labels;                    // 存放聚类结果
    double centroids[10][DIM];      // 最多支持 K=10
    std::mt19937 rng;

    void init_centroids() {
        std::uniform_int_distribution<> dis(0, SIZE - 1);
        for (int i = 0; i < K; i++) {
            int idx = dis(rng);
            for (int d = 0; d < DIM; d++) centroids[i][d] = features[idx][d];
        }
    }

    void assign() {
        for (int i = 0; i < SIZE; i++) {
            double min_d = std::numeric_limits<double>::max();
            int best = 0;
            for (int j = 0; j < K; j++) {
                double d = euclidean(features[i], centroids[j], DIM);
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
            if (cnt == 0) {
                std::uniform_int_distribution<> dis(0, SIZE - 1);
                int idx = dis(rng);
                for (int d = 0; d < DIM; d++) centroids[c][d] = features[idx][d];
            } else {
                for (int d = 0; d < DIM; d++) centroids[c][d] = sum[d] / cnt;
            }
        }
    }

public:
    TraditionalKMeans(int k, const double (*feat)[DIM], int* lbl)
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
