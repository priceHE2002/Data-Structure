/**
 * gmm.h — Gaussian Mixture Model (EM算法)
 *
 * 核心思想: 用多个高斯分布拟合数据, 通过EM迭代优化
 *   E-step: 计算每个点属于每个高斯分量的后验概率 (责任度)
 *   M-step: 根据责任度重新估计均值、协方差(简化)和混合权重
 *
 * 简化: 使用对角协方差矩阵 (Naive Bayes 假设)
 *
 * 参考文献: Dempster et al. (1977) "Maximum Likelihood from Incomplete Data
 *           via the EM Algorithm"
 */
#ifndef GMM_H
#define GMM_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template<int DIM, int SIZE>
class GMM {
    int K;
    const double (*features)[DIM];
    int* labels;
    double means[10][DIM];
    double vars[10][DIM];   // 对角协方差
    double weights[10];
    double resp[SIZE][10];  // 责任度 (后验概率)
    std::mt19937 rng;

    double gaussian_pdf(int i, int k) const {
        double log_prob = 0;
        for (int d = 0; d < DIM; d++) {
            double diff = features[i][d] - means[k][d];
            double v = vars[k][d];
            if (v < 1e-10) v = 1e-10;
            log_prob += -0.5 * std::log(2 * M_PI * v) - (diff * diff) / (2 * v);
        }
        return std::exp(log_prob);
    }

    void e_step() {
        for (int i = 0; i < SIZE; i++) {
            double total = 0;
            for (int k = 0; k < K; k++) {
                resp[i][k] = weights[k] * gaussian_pdf(i, k);
                total += resp[i][k];
            }
            if (total < 1e-300) total = 1e-300;
            for (int k = 0; k < K; k++)
                resp[i][k] /= total;
        }
    }

    void m_step() {
        for (int k = 0; k < K; k++) {
            double Nk = 0;
            double sum[DIM] = {0};
            for (int i = 0; i < SIZE; i++) {
                Nk += resp[i][k];
                for (int d = 0; d < DIM; d++)
                    sum[d] += resp[i][k] * features[i][d];
            }
            if (Nk < 1e-10) Nk = 1e-10;
            weights[k] = Nk / SIZE;
            for (int d = 0; d < DIM; d++)
                means[k][d] = sum[d] / Nk;
        }

        // 更新方差
        for (int k = 0; k < K; k++) {
            double Nk = 0;
            double sum_var[DIM] = {0};
            for (int i = 0; i < SIZE; i++) {
                Nk += resp[i][k];
                for (int d = 0; d < DIM; d++) {
                    double diff = features[i][d] - means[k][d];
                    sum_var[d] += resp[i][k] * diff * diff;
                }
            }
            if (Nk < 1e-10) Nk = 1e-10;
            for (int d = 0; d < DIM; d++)
                vars[k][d] = sum_var[d] / Nk + 1e-8;
        }
    }

    double log_likelihood() const {
        double ll = 0;
        for (int i = 0; i < SIZE; i++) {
            double p = 0;
            for (int k = 0; k < K; k++)
                p += weights[k] * gaussian_pdf(i, k);
            ll += std::log(p > 1e-300 ? p : 1e-300);
        }
        return ll;
    }

public:
    GMM(int k, const double (*feat)[DIM], int* lbl)
        : K(k), features(feat), labels(lbl) {
        std::random_device rd; rng.seed(rd());
    }

    void run(int max_iter = 50) {
        // 初始化: 用 K-Means++ 方式初始化均值
        std::uniform_int_distribution<> dis0(0, SIZE - 1);
        int idx = dis0(rng);
        for (int d = 0; d < DIM; d++) means[0][d] = features[idx][d];

        double min_d_sq[SIZE];
        for (int k = 1; k < K; k++) {
            double total_w = 0;
            for (int i = 0; i < SIZE; i++) {
                double md = 1e100;
                for (int j = 0; j < k; j++) {
                    double s = 0;
                    for (int d = 0; d < DIM; d++) {
                        double diff = features[i][d] - means[j][d];
                        s += diff * diff;
                    }
                    if (s < md) md = s;
                }
                min_d_sq[i] = md;
                total_w += md;
            }
            std::uniform_real_distribution<> dis(0, total_w);
            double r = dis(rng), cum = 0;
            int pick = 0;
            for (int i = 0; i < SIZE; i++) {
                cum += min_d_sq[i];
                if (cum >= r) { pick = i; break; }
            }
            for (int d = 0; d < DIM; d++) means[k][d] = features[pick][d];
        }

        // 初始化方差和权重
        for (int k = 0; k < K; k++) {
            weights[k] = 1.0 / K;
            for (int d = 0; d < DIM; d++)
                vars[k][d] = 1.0;
        }

        double prev_ll = -1e100;
        for (int iter = 0; iter < max_iter; iter++) {
            e_step();
            m_step();
            double ll = log_likelihood();
            if (std::abs(ll - prev_ll) < 1e-3) break;
            prev_ll = ll;
        }

        // 硬分配
        for (int i = 0; i < SIZE; i++) {
            int best = 0;
            double max_r = resp[i][0];
            for (int k = 1; k < K; k++)
                if (resp[i][k] > max_r) { max_r = resp[i][k]; best = k; }
            labels[i] = best;
        }
    }
};

#endif
