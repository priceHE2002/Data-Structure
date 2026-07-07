/**
 * mean_shift.h — Mean Shift 聚类
 *
 * 核心思想: 通过核密度估计的梯度上升找到密度峰值
 *   1. 对每个点, 计算带宽内的均值偏移向量
 *   2. 沿偏移方向移动点 (爬坡)
 *   3. 收敛到同一峰值的点归为同一簇
 *
 * 优势: 无需预设K, 对任意形状簇有效
 * 缺点: 计算复杂度 O(n²), 带宽选择敏感
 *
 * 参考文献: Comaniciu & Meer (2002) "Mean Shift: A Robust Approach Toward
 *           Feature Space Analysis"
 */
#ifndef MEAN_SHIFT_H
#define MEAN_SHIFT_H

#include <cmath>
#include <cstdlib>

template<int DIM, int SIZE>
class MeanShift {
    double bandwidth;
    const double (*features)[DIM];
    int* labels;

    double dist2(int i, int j) const {
        double s = 0;
        for (int d = 0; d < DIM; d++) {
            double diff = features[i][d] - features[j][d];
            s += diff * diff;
        }
        return s;
    }

    double dist2_point(const double a[], const double b[]) const {
        double s = 0;
        for (int d = 0; d < DIM; d++) {
            double diff = a[d] - b[d];
            s += diff * diff;
        }
        return s;
    }

    // 对单个点执行 mean shift 迭代
    void shift_point(const double start[], double result[], int max_iter = 100) {
        for (int d = 0; d < DIM; d++) result[d] = start[d];

        double bw2 = bandwidth * bandwidth;
        for (int iter = 0; iter < max_iter; iter++) {
            double new_pt[DIM] = {0};
            double total_w = 0;
            for (int j = 0; j < SIZE; j++) {
                double d2 = dist2_point(result, features[j]);
                if (d2 <= bw2) {
                    double w = std::exp(-d2 / (2 * bw2)); // 高斯核
                    for (int d = 0; d < DIM; d++) new_pt[d] += w * features[j][d];
                    total_w += w;
                }
            }
            if (total_w < 1e-10) break;
            for (int d = 0; d < DIM; d++) new_pt[d] /= total_w;

            double shift = 0;
            for (int d = 0; d < DIM; d++) {
                double diff = new_pt[d] - result[d];
                shift += diff * diff;
            }
            for (int d = 0; d < DIM; d++) result[d] = new_pt[d];
            if (shift < 1e-6) break;
        }
    }

public:
    MeanShift(double bw, const double (*feat)[DIM], int* lbl)
        : bandwidth(bw), features(feat), labels(lbl) {}

    void run() {
        // 对每个点执行 mean shift
        double modes[SIZE][DIM];
        for (int i = 0; i < SIZE; i++)
            shift_point(features[i], modes[i]);

        // 合并收敛到同一模式的点
        int cluster_id = 0;
        for (int i = 0; i < SIZE; i++) labels[i] = -1;

        double merge_threshold = bandwidth * 0.5;
        double m2 = merge_threshold * merge_threshold;

        for (int i = 0; i < SIZE; i++) {
            if (labels[i] != -1) continue;
            labels[i] = cluster_id;
            for (int j = i + 1; j < SIZE; j++) {
                if (labels[j] != -1) continue;
                if (dist2_point(modes[i], modes[j]) < m2)
                    labels[j] = cluster_id;
            }
            cluster_id++;
        }
    }
};

#endif
