/**
 * fuzzy_cmeans.h — Fuzzy C-Means (FCM) 模糊聚类
 *
 * 核心思想: 每个点以隶属度属于多个簇 (软聚类)
 *   目标函数: J = Σᵢ Σⱼ uᵢⱼᵐ ||xᵢ - cⱼ||²
 *   其中 uᵢⱼ 是点i对簇j的隶属度, m 是模糊因子 (通常 1.5~2.5)
 *
 * 算法步骤:
 *   1. 初始化隶属度矩阵 U (随机)
 *   2. 计算簇中心: cⱼ = Σᵢ uᵢⱼᵐ xᵢ / Σᵢ uᵢⱼᵐ
 *   3. 更新隶属度: uᵢⱼ = 1 / Σₖ (||xᵢ-cⱼ|| / ||xᵢ-cₖ||)^(2/(m-1))
 *   4. 重复2-3直到收敛
 *   5. 硬分配: 取最大隶属度的簇
 *
 * 参考文献: Bezdek (1981) "Pattern Recognition with Fuzzy Objective Function Algorithms"
 */
#ifndef FUZZY_CMEANS_H
#define FUZZY_CMEANS_H

#include <cmath>
#include <limits>
#include <random>

template<int DIM, int SIZE>
class FuzzyCMeans {
    int K;
    double m; // 模糊因子
    const double (*features)[DIM];
    int* labels;
    double centroids[10][DIM];
    double U[SIZE][10]; // 隶属度矩阵
    std::mt19937 rng;

    double dist2(int i, int c) const {
        double s = 0;
        for (int d = 0; d < DIM; d++) {
            double diff = features[i][d] - centroids[c][d];
            s += diff * diff;
        }
        return s;
    }

public:
    FuzzyCMeans(int k, double fuzz, const double (*feat)[DIM], int* lbl)
        : K(k), m(fuzz), features(feat), labels(lbl) {
        std::random_device rd; rng.seed(rd());
    }

    void run(int max_iter = 100) {
        // 初始化隶属度矩阵 (随机)
        std::uniform_real_distribution<> dis(0, 1);
        for (int i = 0; i < SIZE; i++) {
            double sum = 0;
            for (int k = 0; k < K; k++) {
                U[i][k] = dis(rng) + 0.1; // 避免为0
                sum += U[i][k];
            }
            for (int k = 0; k < K; k++) U[i][k] /= sum;
        }

        double exponent = 2.0 / (m - 1.0);

        for (int iter = 0; iter < max_iter; iter++) {
            // 更新簇中心
            for (int k = 0; k < K; k++) {
                double weight_sum = 0;
                double sum[DIM] = {0};
                for (int i = 0; i < SIZE; i++) {
                    double w = std::pow(U[i][k], m);
                    weight_sum += w;
                    for (int d = 0; d < DIM; d++)
                        sum[d] += w * features[i][d];
                }
                if (weight_sum < 1e-10) {
                    std::uniform_int_distribution<> d(0, SIZE - 1);
                    int idx = d(rng);
                    for (int d2 = 0; d2 < DIM; d2++)
                        centroids[k][d2] = features[idx][d2];
                } else {
                    for (int d = 0; d < DIM; d++)
                        centroids[k][d] = sum[d] / weight_sum;
                }
            }

            // 更新隶属度矩阵
            double max_change = 0;
            for (int i = 0; i < SIZE; i++) {
                double old_u[10];
                for (int k = 0; k < K; k++) old_u[k] = U[i][k];

                for (int k = 0; k < K; k++) {
                    double d_ik = dist2(i, k);
                    if (d_ik < 1e-10) d_ik = 1e-10;
                    double sum = 0;
                    for (int j = 0; j < K; j++) {
                        double d_ij = dist2(i, j);
                        if (d_ij < 1e-10) d_ij = 1e-10;
                        sum += std::pow(d_ik / d_ij, exponent);
                    }
                    if (sum < 1e-10) sum = 1e-10;
                    U[i][k] = 1.0 / sum;
                }

                for (int k = 0; k < K; k++) {
                    double change = std::abs(U[i][k] - old_u[k]);
                    if (change > max_change) max_change = change;
                }
            }

            if (max_change < 1e-5) break;
        }

        // 硬分配
        for (int i = 0; i < SIZE; i++) {
            int best = 0;
            for (int k = 1; k < K; k++)
                if (U[i][k] > U[i][best]) best = k;
            labels[i] = best;
        }
    }
};

#endif
