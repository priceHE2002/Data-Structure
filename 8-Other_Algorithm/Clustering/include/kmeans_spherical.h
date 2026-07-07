/**
 * kmeans_spherical.h — 球面 K-Means（余弦相似度 + 中心归一化）
 *
 * 与普通 K-Means 的区别:
 *   - 用余弦相似度代替欧氏距离
 *   - 更新中心后归一化为单位向量
 */

#ifndef KMEANS_SPHERICAL_H
#define KMEANS_SPHERICAL_H

#include <cmath>
#include <limits>
#include <random>

// 将向量归一化为单位向量
inline void normalize(double v[], int dim) {
    double sq = 0;
    for (int i = 0; i < dim; i++) sq += v[i] * v[i];
    double norm = std::sqrt(sq);
    if (norm > 0)
        for (int i = 0; i < dim; i++) v[i] /= norm;
}

// 余弦相似度 = (A·B) / (||A|| × ||B||)
inline double cosine(const double a[], const double b[], int dim) {
    double dot = 0, na = 0, nb = 0;
    for (int i = 0; i < dim; i++) {
        dot += a[i] * b[i];
        na  += a[i] * a[i];
        nb  += b[i] * b[i];
    }
    double denom = std::sqrt(na) * std::sqrt(nb);
    return (denom == 0) ? 0.0 : dot / denom;
}

template<int DIM, int SIZE>
class SphericalKMeans {
    int K;
    const double (*features)[DIM];
    int* labels;
    double centroids[10][DIM];
    std::mt19937 rng;

    void init_centroids() {
        std::uniform_int_distribution<> dis(0, SIZE - 1);
        for (int i = 0; i < K; i++) {
            int idx = dis(rng);
            for (int d = 0; d < DIM; d++) centroids[i][d] = features[idx][d];
            normalize(centroids[i], DIM);
        }
    }

    void assign() {
        for (int i = 0; i < SIZE; i++) {
            double max_sim = -std::numeric_limits<double>::infinity();
            int best = 0;
            for (int j = 0; j < K; j++) {
                double sim = cosine(features[i], centroids[j], DIM);
                if (sim > max_sim) { max_sim = sim; best = j; }
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
                normalize(centroids[c], DIM);
            } else {
                for (int d = 0; d < DIM; d++) centroids[c][d] = sum[d] / cnt;
                normalize(centroids[c], DIM);
            }
        }
    }

public:
    SphericalKMeans(int k, const double (*feat)[DIM], int* lbl)
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
