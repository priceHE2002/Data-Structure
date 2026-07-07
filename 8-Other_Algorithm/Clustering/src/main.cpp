/**
 * main.cpp — 聚类算法全面对比实验
 *
 * 项目结构:
 *   include/  : 10种聚类算法头文件
 *   data/     : 数据集 (Iris, Wine)
 *   src/      : 实验入口 main.cpp
 *   Makefile  : 构建脚本
 *
 * 算法清单 (11种):
 *   1. Traditional K-Means     — 传统K均值 (欧氏距离)
 *   2. Spherical K-Means       — 球面K均值 (余弦相似度)
 *   3. K-Means++               — 改进初始化
 *   4. K-Medoids (PAM)         — K中心点 (对离群点鲁棒)
 *   5. DBSCAN                  — 密度聚类
 *   6. GMM (EM)                — 高斯混合模型
 *   7. Hierarchical            — 凝聚层次聚类
 *   8. Mean Shift              — 均值漂移
 *   9. Fuzzy C-Means           — 模糊C均值
 *  10. OPTICS                  — 有序点识别聚类结构
 *  11. DEC                     — 深度嵌入式聚类 (SOTA, ICML 2016)
 */

#include <cstdio>
#include <cmath>
#include <algorithm>
#include "../data/dataset_iris.h"
#include "../data/dataset_wine.h"
#include "../include/kmeans_traditional.h"
#include "../include/kmeans_spherical.h"
#include "../include/kmeans_plusplus.h"
#include "../include/kmedoids.h"
#include "../include/dbscan.h"
#include "../include/gmm.h"
#include "../include/hierarchical.h"
#include "../include/mean_shift.h"
#include "../include/fuzzy_cmeans.h"
#include "../include/optics.h"
#include "../include/dec.h"

// ======================== 评估指标 ========================

double purity(const int labels[], const int truth[], int n, int num_classes) {
    int confusion[20][20] = {};
    for (int i = 0; i < n; i++)
        if (labels[i] >= 0) confusion[labels[i]][truth[i]]++;

    int correct = 0;
    bool used[20] = {};
    for (int c = 0; c < num_classes; c++) {
        int max_cnt = 0, best_label = 0;
        for (int l = 0; l < 20; l++) {
            if (!used[l] && confusion[l][c] > max_cnt) {
                max_cnt = confusion[l][c]; best_label = l;
            }
        }
        if (max_cnt > 0) { used[best_label] = true; correct += max_cnt; }
    }
    return 100.0 * correct / n;
}

double nmi(const int labels[], const int truth[], int n, int num_classes) {
    const int max_k = 20;
    int cnt_l[max_k] = {}, cnt_t[max_k] = {};
    int joint[max_k][max_k] = {};

    for (int i = 0; i < n; i++) {
        int l = (labels[i] >= 0) ? labels[i] : 0;
        int t = truth[i];
        if (l < max_k && t < max_k) {
            cnt_l[l]++; cnt_t[t]++;
            joint[l][t]++;
        }
    }

    double mi = 0;
    for (int l = 0; l < max_k; l++) {
        for (int t = 0; t < max_k; t++) {
            if (joint[l][t] == 0) continue;
            double p_joint = (double)joint[l][t] / n;
            double p_l = (double)cnt_l[l] / n;
            double p_t = (double)cnt_t[t] / n;
            mi += p_joint * std::log(p_joint / (p_l * p_t));
        }
    }

    double h_l = 0, h_t = 0;
    for (int l = 0; l < max_k; l++)
        if (cnt_l[l] > 0) {
            double p = (double)cnt_l[l] / n;
            h_l -= p * std::log(p);
        }
    for (int t = 0; t < max_k; t++)
        if (cnt_t[t] > 0) {
            double p = (double)cnt_t[t] / n;
            h_t -= p * std::log(p);
        }

    double denom = (h_l + h_t) / 2.0;
    return (denom < 1e-10) ? 0 : mi / denom;
}

double ari(const int labels[], const int truth[], int n) {
    const int max_k = 20;
    int contingency[max_k][max_k] = {};
    int row_sum[max_k] = {}, col_sum[max_k] = {};

    for (int i = 0; i < n; i++) {
        int l = (labels[i] >= 0) ? labels[i] : 0;
        int t = truth[i];
        if (l < max_k && t < max_k) {
            contingency[l][t]++; row_sum[l]++; col_sum[t]++;
        }
    }

    double sum_comb = 0;
    for (int i = 0; i < max_k; i++)
        for (int j = 0; j < max_k; j++)
            if (contingency[i][j] > 1)
                sum_comb += (double)contingency[i][j] * (contingency[i][j] - 1) / 2.0;

    double row_comb = 0;
    for (int i = 0; i < max_k; i++)
        if (row_sum[i] > 1) row_comb += (double)row_sum[i] * (row_sum[i] - 1) / 2.0;

    double col_comb = 0;
    for (int j = 0; j < max_k; j++)
        if (col_sum[j] > 1) col_comb += (double)col_sum[j] * (col_sum[j] - 1) / 2.0;

    double total = (double)n * (n - 1) / 2.0;
    double expected = row_comb * col_comb / total;
    double max_ijk = (row_comb + col_comb) / 2.0;
    double denom = max_ijk - expected;
    return (std::abs(denom) < 1e-10) ? 0 : (sum_comb - expected) / denom;
}

// ======================== 打印行 ========================

void print_row(const char* algo, double p, double n, double a) {
    printf("║ %-18s │ %8.2f │ %8.4f │ %8.4f ║\n", algo, p, n, a);
}

void print_row_no_score(const char* algo, double p) {
    printf("║ %-18s │ %8.2f │ %8s │ %8s ║\n", algo, p, "-", "-");
}

void print_header(const char* name, int dim, int size, int classes) {
    printf("╔══════════════════════════════════════════════════════╗\n");
    printf("║  %-50s ║\n", name);
    printf("║  dim=%-3d  size=%-4d  classes=%-2d                       ║\n",
           dim, size, classes);
    printf("╠══════════════════════════════════════════════════════╣\n");
    printf("║ %-18s │ %8s │ %8s │ %8s ║\n",
           "Algorithm", "Purity%", "NMI", "ARI");
    printf("╠══════════════════════════════════════════════════════╣\n");
}

void print_sep() {
    printf("╠══════════════════════════════════════════════════════╣\n");
}

void print_footer() {
    printf("╚══════════════════════════════════════════════════════╝\n");
}

// ======================== 实验框架 ========================

template<int DIM, int SIZE, int CLASSES>
struct Experiment {
    typedef void (*Loader)(double[SIZE][DIM], int[SIZE]);

    const char* name;
    Loader load;
    double features[SIZE][DIM];
    int truth[SIZE];
    int K;

    Experiment(const char* n, Loader l) : name(n), load(l), K(CLASSES) {
        load(features, truth);
    }

    void run_all() {
        printf("\n");
        print_header(name, DIM, SIZE, CLASSES);

        // ---- Part I: 划分方法 ----
        {
            int labels[SIZE] = {};
            TraditionalKMeans<DIM, SIZE> algo(K, features, labels);
            algo.run();
            print_row("Traditional KM",
                purity(labels, truth, SIZE, CLASSES),
                nmi(labels, truth, SIZE, CLASSES),
                ari(labels, truth, SIZE));
        }

        {
            int labels[SIZE] = {};
            KMeansPlusPlus<DIM, SIZE> algo(K, features, labels);
            algo.run();
            print_row("K-Means++",
                purity(labels, truth, SIZE, CLASSES),
                nmi(labels, truth, SIZE, CLASSES),
                ari(labels, truth, SIZE));
        }

        {
            int labels[SIZE] = {};
            KMedoids<DIM, SIZE> algo(K, features, labels);
            algo.run();
            print_row("K-Medoids",
                purity(labels, truth, SIZE, CLASSES),
                nmi(labels, truth, SIZE, CLASSES),
                ari(labels, truth, SIZE));
        }

        print_sep();

        // ---- Part II: 软聚类/概率方法 ----
        {
            int labels[SIZE] = {};
            GMM<DIM, SIZE> algo(K, features, labels);
            algo.run();
            print_row("GMM (EM)",
                purity(labels, truth, SIZE, CLASSES),
                nmi(labels, truth, SIZE, CLASSES),
                ari(labels, truth, SIZE));
        }

        {
            int labels[SIZE] = {};
            FuzzyCMeans<DIM, SIZE> algo(K, 2.0, features, labels);
            algo.run();
            print_row("Fuzzy C-Means",
                purity(labels, truth, SIZE, CLASSES),
                nmi(labels, truth, SIZE, CLASSES),
                ari(labels, truth, SIZE));
        }

        print_sep();

        // ---- Part III: 密度方法 ----
        // DBSCAN: 参数搜索
        {
            double best_p = 0;
            double eps_vals[] = {0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
            int mp_vals[] = {2, 3, 5};
            for (int ei = 0; ei < 6; ei++) {
                for (int mi = 0; mi < 3; mi++) {
                    int labels[SIZE] = {};
                    DBSCAN<DIM, SIZE> db(eps_vals[ei], mp_vals[mi], features, labels);
                    db.run();
                    double p = purity(labels, truth, SIZE, CLASSES);
                    if (p > best_p) best_p = p;
                }
            }
            print_row_no_score("DBSCAN", best_p);
        }

        // OPTICS: 参数搜索
        {
            double best_p = 0;
            double eps_vals[] = {0.5, 1.0, 1.5, 2.0, 3.0};
            double xi_vals[] = {0.03, 0.05, 0.08, 0.12, 0.2};
            for (int ei = 0; ei < 5; ei++) {
                for (int xi = 0; xi < 5; xi++) {
                    int labels[SIZE] = {};
                    OPTICS<DIM, SIZE> opt(eps_vals[ei], 3, features, labels);
                    opt.run();
                    opt.extract_clusters(xi_vals[xi]);
                    double p = purity(labels, truth, SIZE, CLASSES);
                    if (p > best_p) best_p = p;
                }
            }
            print_row_no_score("OPTICS", best_p);
        }

        // Mean Shift: 带宽搜索
        {
            double best_p = 0;
            double bw_vals[] = {0.8, 1.2, 1.6, 2.0, 2.5, 3.0};
            for (int bi = 0; bi < 6; bi++) {
                int labels[SIZE] = {};
                MeanShift<DIM, SIZE> ms(bw_vals[bi], features, labels);
                ms.run();
                double p = purity(labels, truth, SIZE, CLASSES);
                if (p > best_p) best_p = p;
            }
            print_row_no_score("Mean Shift", best_p);
        }

        print_sep();

        // ---- Part IV: 层次 & 相似度方法 ----
        {
            int labels[SIZE] = {};
            Hierarchical<DIM, SIZE> algo(K, AVERAGE, features, labels);
            algo.run();
            print_row("Hierarchical",
                purity(labels, truth, SIZE, CLASSES),
                nmi(labels, truth, SIZE, CLASSES),
                ari(labels, truth, SIZE));
        }

        {
            int labels[SIZE] = {};
            SphericalKMeans<DIM, SIZE> algo(K, features, labels);
            algo.run();
            print_row("Spherical KM",
                purity(labels, truth, SIZE, CLASSES),
                nmi(labels, truth, SIZE, CLASSES),
                ari(labels, truth, SIZE));
        }

        print_sep();

        // ---- Part V: 深度学习 ----
        {
            int labels[SIZE] = {};
            // 根据输入维度选择网络结构: Iris(4维)→hidden=10,bottleneck=3; Wine(13维)→hidden=25,bottleneck=5
            if (DIM <= 6) {
                DEC<DIM, SIZE, 10, 3> dec(K, features, labels);
                dec.run();
            } else {
                DEC<DIM, SIZE, 25, 5> dec(K, features, labels);
                dec.run();
            }
            print_row("DEC (DeepEmb)",
                purity(labels, truth, SIZE, CLASSES),
                nmi(labels, truth, SIZE, CLASSES),
                ari(labels, truth, SIZE));
        }

        print_footer();
    }
};

// ======================== 主函数 ========================

int main() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║         聚类算法全面对比实验 (11种算法)                     ║\n");
    printf("║                                                           ║\n");
    printf("║  Part I   - 基于划分: K-Means, K-Means++, K-Medoids        ║\n");
    printf("║  Part II  - 软聚类/概率: GMM (EM), Fuzzy C-Means            ║\n");
    printf("║  Part III - 基于密度: DBSCAN, OPTICS, Mean Shift            ║\n");
    printf("║  Part IV  - 层次 & 相似度: Hierarchical, Spherical K-Means  ║\n");
    printf("║  Part V   - 深度学习: DEC (Deep Embedded Clustering)        ║\n");
    printf("║                                                           ║\n");
    printf("║  评估指标: Purity (纯度), NMI (归一化互信息), ARI (调整兰德) ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");

    Experiment<IRIS_DIM, IRIS_SIZE, IRIS_CLASSES> exp1("UCI Iris Dataset", load_iris);
    exp1.run_all();

    Experiment<WINE_DIM, WINE_SIZE, WINE_CLASSES> exp2("UCI Wine Dataset", load_wine);
    exp2.run_all();

    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                        总结分析                            ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║ K-Means++    : 通过改进初始化获得更稳定的结果               ║\n");
    printf("║ K-Medoids    : 使用实际数据点作中心, 对离群点更鲁棒         ║\n");
    printf("║ Spherical KM : 余弦相似度, 适合方向敏感的高维数据           ║\n");
    printf("║ GMM (EM)     : 概率软聚类, 能捕捉不同形状的分布             ║\n");
    printf("║ Fuzzy C-Means: 隶属度软聚类, 处理边界模糊的数据             ║\n");
    printf("║ Hierarchical : 树状结构, 可观察不同层次的聚类关系           ║\n");
    printf("║ DBSCAN       : 发现任意形状簇, 自动识别噪声                 ║\n");
    printf("║ OPTICS       : 改进的密度聚类, 对不同密度更敏感             ║\n");
    printf("║ Mean Shift   : 密度峰值梯度上升, 无需预设簇数               ║\n");
    printf("║ DEC          : 深度学习特征+聚类联合优化 (ICML 2016)        ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n");

    return 0;
}
