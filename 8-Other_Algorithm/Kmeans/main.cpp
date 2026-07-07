/**
 * main.cpp — 传统 K-Means vs 球面 K-Means 对比实验
 *
 * 项目结构:
 *   dataset_iris.h       : Iris 数据集 (150x4)
 *   dataset_wine.h       : Wine 数据集 (178x13)
 *   kmeans_traditional.h : 传统 K-Means（欧氏距离）
 *   kmeans_spherical.h   : 球面 K-Means（余弦相似度）
 */

#include <cstdio>
#include "dataset_iris.h"
#include "dataset_wine.h"
#include "kmeans_traditional.h"
#include "kmeans_spherical.h"

// 计算聚类纯度（贪心匹配）
double purity(const int labels[], const int truth[], int n, int num_classes) {
    int confusion[10][10] = {{0}};
    for (int i = 0; i < n; i++)
        confusion[labels[i]][truth[i]]++;

    int correct = 0;
    bool used[10] = {false};
    for (int c = 0; c < num_classes; c++) {
        int max_cnt = 0, best_label = 0;
        for (int l = 0; l < num_classes; l++) {
            if (!used[l] && confusion[c][l] > max_cnt) {
                max_cnt = confusion[c][l]; best_label = l;
            }
        }
        if (max_cnt > 0) { used[best_label] = true; correct += max_cnt; }
    }
    return 100.0 * correct / n;
}

// 运行一次实验: 传统 vs 球面
template<int DIM, int SIZE, int CLASSES>
void run_experiment(const char* name,
                    void (*load)(double[SIZE][DIM], int[SIZE])) {
    double features[SIZE][DIM];
    int truth[SIZE];
    load(features, truth);

    const int K = CLASSES; // 知道真实类别数

    printf("\n========== %s (dim=%d, n=%d, k=%d) ==========\n",
           name, DIM, SIZE, K);

    // 传统 K-Means
    {
        int labels[SIZE] = {0};
        TraditionalKMeans<DIM, SIZE> km(K, features, labels);
        km.run();
        printf("  传统 K-Means 纯度: %.2f%%\n", purity(labels, truth, SIZE, CLASSES));
    }

    // 球面 K-Means
    {
        int labels[SIZE] = {0};
        SphericalKMeans<DIM, SIZE> skm(K, features, labels);
        skm.run();
        printf("  球面 K-Means 纯度: %.2f%%\n", purity(labels, truth, SIZE, CLASSES));
    }
}

int main() {
    printf("===== K-Means 对比实验 =====\n");
    printf("算法: 传统 K-Means (欧氏距离) vs 球面 K-Means (余弦相似度)\n");

    run_experiment<IRIS_DIM, IRIS_SIZE, IRIS_CLASSES>("Iris", load_iris);
    run_experiment<WINE_DIM, WINE_SIZE, WINE_CLASSES>("Wine", load_wine);

    printf("\n===== 总结 =====\n");
    printf("Iris (4维, 3类): 两类算法通常表现接近, setosa 几乎完美分离\n");
    printf("Wine (13维, 3类): 球面 K-Means 在方向敏感的高维数据上可能更优\n");
    return 0;
}
