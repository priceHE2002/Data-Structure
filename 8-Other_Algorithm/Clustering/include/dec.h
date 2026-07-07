/**
 * dec.h — Deep Embedded Clustering (DEC)
 *
 * Xie, Girshick, Farhadi (2016) "Unsupervised Deep Embedding for Clustering Analysis"
 *
 * 三阶段:
 *   Phase 1 — 逐层预训练 (Greedy Layer-wise Pretraining)
 *   Phase 2 — 自编码器精调 (重建损失)
 *   Phase 3 — KL散度优化 (聚类导向微调编码器)
 *
 * 网络结构: DIM → HIDDEN → BOTTLENECK → HIDDEN → DIM
 *   激活函数: LeakyReLU (避免神经元死亡)
 */
#ifndef DEC_H
#define DEC_H

#include <cmath>
#include <cstring>
#include <random>
#include <limits>

// ==================== 全连接层 ====================
struct DenseLayer {
    int in_dim, out_dim;
    double* W;
    double* b;
    double* dW, *db;
    double* vW, *vb;

    DenseLayer() : W(nullptr), b(nullptr), dW(nullptr), db(nullptr), vW(nullptr), vb(nullptr) {}

    void init(int in_d, int out_d, std::mt19937& rng) {
        in_dim = in_d; out_dim = out_d;
        int nw = in_dim * out_dim;
        W  = new double[nw]();
        dW = new double[nw]();
        vW = new double[nw]();
        b  = new double[out_dim]();
        db = new double[out_dim]();
        vb = new double[out_dim]();
        double scale = std::sqrt(2.0 / (in_dim + out_dim));
        std::normal_distribution<> nd(0, scale);
        for (int i = 0; i < nw; i++) W[i] = nd(rng);
    }

    void copy_from(const DenseLayer& other) {
        for (int i = 0; i < in_dim * out_dim; i++) W[i] = other.W[i];
        for (int i = 0; i < out_dim; i++) b[i] = other.b[i];
    }

    ~DenseLayer() { delete[] W; delete[] b; delete[] dW; delete[] db; delete[] vW; delete[] vb; }
    DenseLayer(const DenseLayer&) = delete;
    DenseLayer& operator=(const DenseLayer&) = delete;

    void forward(const double* in, double* out) const {
        for (int o = 0; o < out_dim; o++) {
            out[o] = b[o];
            for (int i = 0; i < in_dim; i++) out[o] += W[o * in_dim + i] * in[i];
        }
    }

    void backward(const double* in, const double* g_out, double* g_in, double scale = 1.0) {
        for (int o = 0; o < out_dim; o++) {
            double go = g_out[o] * scale;
            for (int i = 0; i < in_dim; i++) dW[o * in_dim + i] += go * in[i];
            db[o] += go;
        }
        if (g_in) {
            for (int i = 0; i < in_dim; i++) {
                g_in[i] = 0;
                for (int o = 0; o < out_dim; o++)
                    g_in[i] += W[o * in_dim + i] * g_out[o] * scale;
            }
        }
    }

    void zero_grad() {
        for (int i = 0; i < in_dim * out_dim; i++) dW[i] = 0;
        for (int i = 0; i < out_dim; i++) db[i] = 0;
    }

    void step(double lr, double momentum) {
        for (int i = 0; i < in_dim * out_dim; i++) {
            vW[i] = momentum * vW[i] - lr * dW[i];
            W[i] += vW[i];
        }
        for (int i = 0; i < out_dim; i++) {
            vb[i] = momentum * vb[i] - lr * db[i];
            b[i] += vb[i];
        }
    }
};

// ==================== LeakyReLU ====================
inline double lrelu(double x) { return x > 0 ? x : 0.1 * x; }
inline double lrelu_grad(double x) { return x > 0 ? 1.0 : 0.1; }

inline void leaky_relu_forward(int n, const double* x, double* y) {
    for (int i = 0; i < n; i++) y[i] = lrelu(x[i]);
}
inline void leaky_relu_backward(int n, const double* x, double* g) {
    for (int i = 0; i < n; i++) g[i] *= lrelu_grad(x[i]);
}

// ==================== 单层自编码器 (用于逐层预训练) ====================
template<int IN_DIM, int OUT_DIM, int SIZE>
struct LayerAE {
    DenseLayer enc;
    DenseLayer dec;
    double* hidden;
    double* recon;
    std::mt19937 rng;

    LayerAE() {
        hidden = new double[SIZE * OUT_DIM]();
        recon  = new double[SIZE * IN_DIM]();
        std::random_device rd; rng.seed(rd());
        enc.init(IN_DIM, OUT_DIM, rng);
        dec.init(OUT_DIM, IN_DIM, rng);
    }
    ~LayerAE() { delete[] hidden; delete[] recon; }
    LayerAE(const LayerAE&) = delete;
    LayerAE& operator=(const LayerAE&) = delete;

    // 前向和反向都针对单个样本，返回 MSE
    double fwd_one(const double* in, int idx) {
        double* h = hidden + idx * OUT_DIM;
        double* r = recon  + idx * IN_DIM;
        enc.forward(in, h);
        leaky_relu_forward(OUT_DIM, h, h);
        dec.forward(h, r);
        double loss = 0;
        for (int d = 0; d < IN_DIM; d++) { double diff = r[d] - in[d]; loss += diff * diff; }
        return loss;
    }

    void bwd_one(const double* in, int idx) {
        double* h = hidden + idx * OUT_DIM;
        double* r = recon  + idx * IN_DIM;
        double g_r[IN_DIM];
        for (int d = 0; d < IN_DIM; d++) g_r[d] = 2.0 * (r[d] - in[d]);
        double g_h[OUT_DIM];
        dec.backward(h, g_r, g_h);
        leaky_relu_backward(OUT_DIM, h, g_h);
        enc.backward(in, g_h, nullptr);
    }

    void train(const double data[SIZE][IN_DIM], int epochs, double lr) {
        for (int ep = 0; ep < epochs; ep++) {
            enc.zero_grad(); dec.zero_grad();
            for (int i = 0; i < SIZE; i++) {
                fwd_one(data[i], i);
                bwd_one(data[i], i);
            }
            enc.step(lr, 0.9); dec.step(lr, 0.9);
        }
    }

    // 编码全部数据到 hidden 空间
    void encode_all(const double data[SIZE][IN_DIM], double output[SIZE][OUT_DIM]) {
        for (int i = 0; i < SIZE; i++) {
            double* h = hidden + i * OUT_DIM;
            enc.forward(data[i], h);
            leaky_relu_forward(OUT_DIM, h, h);
            for (int d = 0; d < OUT_DIM; d++) output[i][d] = h[d];
        }
    }
};

// ==================== 深度自编码器 (用于 Phase 2 精调) ====================
template<int DIM, int HIDDEN, int BOTTLENECK, int SIZE>
struct DeepAE {
    DenseLayer e1, e2;   // DIM → HIDDEN → BOTTLENECK
    DenseLayer d1, d2;   // BOTTLENECK → HIDDEN → DIM
    double* h1;           // [SIZE][HIDDEN]
    double* bot;          // [SIZE][BOTTLENECK]
    double* h2;           // [SIZE][HIDDEN]
    double* rec;          // [SIZE][DIM]
    std::mt19937 rng;

    DeepAE() {
        h1  = new double[SIZE * HIDDEN]();
        bot = new double[SIZE * BOTTLENECK]();
        h2  = new double[SIZE * HIDDEN]();
        rec = new double[SIZE * DIM]();
        std::random_device rd; rng.seed(rd());
        e1.init(DIM, HIDDEN, rng);
        e2.init(HIDDEN, BOTTLENECK, rng);
        d1.init(BOTTLENECK, HIDDEN, rng);
        d2.init(HIDDEN, DIM, rng);
    }
    ~DeepAE() { delete[] h1; delete[] bot; delete[] h2; delete[] rec; }
    DeepAE(const DeepAE&) = delete;
    DeepAE& operator=(const DeepAE&) = delete;

    // 从逐层预训练后的 encoder 权重初始化
    void set_encoder(const DenseLayer& layer1_enc, const DenseLayer& layer2_enc) {
        for (int i = 0; i < DIM * HIDDEN; i++) e1.W[i] = layer1_enc.W[i];
        for (int i = 0; i < HIDDEN; i++) e1.b[i] = layer1_enc.b[i];
        for (int i = 0; i < HIDDEN * BOTTLENECK; i++) e2.W[i] = layer2_enc.W[i];
        for (int i = 0; i < BOTTLENECK; i++) e2.b[i] = layer2_enc.b[i];
    }

    double fwd_one(const double* in, int idx) {
        double* h1p = h1 + idx * HIDDEN;
        double* bp  = bot + idx * BOTTLENECK;
        double* h2p = h2 + idx * HIDDEN;
        double* rp  = rec + idx * DIM;
        e1.forward(in, h1p);
        leaky_relu_forward(HIDDEN, h1p, h1p);
        e2.forward(h1p, bp);
        // bottleneck: 线性
        d1.forward(bp, h2p);
        leaky_relu_forward(HIDDEN, h2p, h2p);
        d2.forward(h2p, rp);
        double loss = 0;
        for (int d = 0; d < DIM; d++) { double diff = rp[d] - in[d]; loss += diff * diff; }
        return loss;
    }

    void bwd_one(const double* in, int idx) {
        double* h1p = h1 + idx * HIDDEN;
        double* bp  = bot + idx * BOTTLENECK;
        double* h2p = h2 + idx * HIDDEN;
        double* rp  = rec + idx * DIM;
        double g_r[DIM];
        for (int d = 0; d < DIM; d++) g_r[d] = 2.0 * (rp[d] - in[d]);
        double g_h2[HIDDEN];
        d2.backward(h2p, g_r, g_h2);
        leaky_relu_backward(HIDDEN, h2p, g_h2);
        double g_bot[BOTTLENECK];
        d1.backward(bp, g_h2, g_bot);
        // bottleneck: 线性, 无激活
        double g_h1[HIDDEN];
        e2.backward(h1p, g_bot, g_h1);
        leaky_relu_backward(HIDDEN, h1p, g_h1);
        e1.backward(in, g_h1, nullptr);
    }

    void zero_grads() { e1.zero_grad(); e2.zero_grad(); d1.zero_grad(); d2.zero_grad(); }
    void step(double lr, double mom) { e1.step(lr, mom); e2.step(lr, mom); d1.step(lr, mom); d2.step(lr, mom); }

    void forward_all(const double data[SIZE][DIM]) { for (int i = 0; i < SIZE; i++) fwd_one(data[i], i); }

    void train(const double data[SIZE][DIM], int epochs, double lr) {
        for (int ep = 0; ep < epochs; ep++) {
            zero_grads();
            for (int i = 0; i < SIZE; i++) { fwd_one(data[i], i); bwd_one(data[i], i); }
            step(lr, 0.9);
        }
    }

    // 仅编码器前向: input → bottleneck (用于 DEC Phase 3)
    void encode_one(const double* in, int idx) {
        double* h1p = h1 + idx * HIDDEN;
        double* bp  = bot + idx * BOTTLENECK;
        e1.forward(in, h1p);
        leaky_relu_forward(HIDDEN, h1p, h1p);
        e2.forward(h1p, bp);
    }

    // 从 bottleneck 梯度反向传播到 encoder 参数
    void backward_from_bottleneck(const double* in, int idx, const double* grad_bot) {
        double* h1p = h1 + idx * HIDDEN;
        double grad_h1[HIDDEN];
        e2.backward(h1p, grad_bot, grad_h1);
        leaky_relu_backward(HIDDEN, h1p, grad_h1);
        e1.backward(in, grad_h1, nullptr);
    }
};

// ==================== DEC 模型 ====================

template<int DIM, int SIZE, int HIDDEN, int BOTTLENECK>
class DEC {
    int K;
    const double (*features)[DIM];
    int* labels;
    double norm_data[SIZE][DIM];
    DeepAE<DIM, HIDDEN, BOTTLENECK, SIZE> ae;
    double centroids[10][BOTTLENECK];
    double q[SIZE][10];
    double p[SIZE][10];
    std::mt19937 rng;

    void compute_q() {
        for (int i = 0; i < SIZE; i++) {
            double* zi = ae.bot + i * BOTTLENECK;
            double sum = 0;
            for (int j = 0; j < K; j++) {
                double d2 = 0;
                for (int d = 0; d < BOTTLENECK; d++) { double diff = zi[d] - centroids[j][d]; d2 += diff * diff; }
                q[i][j] = 1.0 / (1.0 + d2);
                sum += q[i][j];
            }
            if (sum < 1e-15) sum = 1e-15;
            for (int j = 0; j < K; j++) q[i][j] /= sum;
        }
    }

    void compute_p() {
        double f[10] = {};
        for (int i = 0; i < SIZE; i++) for (int j = 0; j < K; j++) f[j] += q[i][j];
        for (int i = 0; i < SIZE; i++) {
            double sum = 0;
            for (int j = 0; j < K; j++) {
                p[i][j] = (f[j] > 1e-10) ? q[i][j] * q[i][j] / f[j] : 0;
                sum += p[i][j];
            }
            if (sum < 1e-15) sum = 1e-15;
            for (int j = 0; j < K; j++) p[i][j] /= sum;
        }
    }

    double kl_loss() const {
        double kl = 0;
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < K; j++)
                if (q[i][j] > 1e-15 && p[i][j] > 1e-15)
                    kl += p[i][j] * std::log(p[i][j] / q[i][j]);
        return kl;
    }

    // DEC 梯度: ∂L/∂z_i = 2 * Σ_j (q_ij - p_ij) * (z_i - μ_j) / (1 + ||z_i-μ_j||²)
    // 见 Xie et al. (2016) Eq. 5, α = 1 时系数为 2
    void compute_dec_gradient(double grad_z[SIZE][BOTTLENECK]) {
        for (int i = 0; i < SIZE; i++) {
            double* zi = ae.bot + i * BOTTLENECK;
            for (int d = 0; d < BOTTLENECK; d++) grad_z[i][d] = 0;
            for (int j = 0; j < K; j++) {
                double d2 = 0;
                double diff[BOTTLENECK];
                for (int d = 0; d < BOTTLENECK; d++) { diff[d] = zi[d] - centroids[j][d]; d2 += diff[d] * diff[d]; }
                double w = (q[i][j] - p[i][j]) / (1.0 + d2);
                for (int d = 0; d < BOTTLENECK; d++) grad_z[i][d] += 2.0 * w * diff[d];
            }
        }
    }

    void kmeans_on_bottleneck() {
        // K-Means++ init
        for (int j = 0; j < BOTTLENECK; j++) centroids[0][j] = ae.bot[j];
        for (int k = 1; k < K; k++) {
            double dists[SIZE], total = 0;
            for (int i = 0; i < SIZE; i++) {
                double* zi = ae.bot + i * BOTTLENECK;
                double md = 1e100;
                for (int m = 0; m < k; m++) {
                    double d2 = 0;
                    for (int d = 0; d < BOTTLENECK; d++) { double diff = zi[d] - centroids[m][d]; d2 += diff * diff; }
                    if (d2 < md) md = d2;
                }
                dists[i] = md; total += md;
            }
            std::uniform_real_distribution<> dis(0, total);
            double r = dis(rng), cum = 0;
            int pick = 0;
            for (int i = 0; i < SIZE; i++) { cum += dists[i]; if (cum >= r) { pick = i; break; } }
            for (int d = 0; d < BOTTLENECK; d++) centroids[k][d] = ae.bot[pick * BOTTLENECK + d];
        }

        // Lloyd
        for (int iter = 0; iter < 100; iter++) {
            int assign[SIZE];
            for (int i = 0; i < SIZE; i++) {
                double* zi = ae.bot + i * BOTTLENECK;
                double best = 1e100; int bj = 0;
                for (int j = 0; j < K; j++) {
                    double d2 = 0;
                    for (int d = 0; d < BOTTLENECK; d++) { double diff = zi[d] - centroids[j][d]; d2 += diff * diff; }
                    if (d2 < best) { best = d2; bj = j; }
                }
                assign[i] = bj;
            }
            double nc[10][BOTTLENECK] = {}; int cnt[10] = {};
            for (int i = 0; i < SIZE; i++) {
                double* zi = ae.bot + i * BOTTLENECK;
                cnt[assign[i]]++;
                for (int d = 0; d < BOTTLENECK; d++) nc[assign[i]][d] += zi[d];
            }
            bool conv = true;
            for (int j = 0; j < K; j++) {
                if (cnt[j] > 0) {
                    for (int d = 0; d < BOTTLENECK; d++) nc[j][d] /= cnt[j];
                    double diff = 0;
                    for (int d = 0; d < BOTTLENECK; d++) { double delta = nc[j][d] - centroids[j][d]; diff += delta * delta; }
                    if (diff > 1e-10) conv = false;
                }
                for (int d = 0; d < BOTTLENECK; d++) centroids[j][d] = nc[j][d];
            }
            if (conv) break;
        }
    }

public:
    DEC(int k, const double (*feat)[DIM], int* lbl) : K(k), features(feat), labels(lbl) {
        std::random_device rd; rng.seed(rd());
        double mins[DIM], maxs[DIM];
        for (int d = 0; d < DIM; d++) { mins[d] = 1e100; maxs[d] = -1e100; }
        for (int i = 0; i < SIZE; i++)
            for (int d = 0; d < DIM; d++) {
                if (features[i][d] < mins[d]) mins[d] = features[i][d];
                if (features[i][d] > maxs[d]) maxs[d] = features[i][d];
            }
        for (int i = 0; i < SIZE; i++)
            for (int d = 0; d < DIM; d++) {
                double range = maxs[d] - mins[d];
                norm_data[i][d] = (range > 1e-10) ? (features[i][d] - mins[d]) / range : 0.5;
            }
    }

    void run() {
        // ====== Phase 1: Layer-wise Pretraining ======
        // 第一层: DIM → HIDDEN → DIM
        LayerAE<DIM, HIDDEN, SIZE> lae1;
        lae1.train(norm_data, 300, 0.01);
        double h1_data[SIZE][HIDDEN];
        lae1.encode_all(norm_data, h1_data);

        // 第二层: HIDDEN → BOTTLENECK → HIDDEN
        LayerAE<HIDDEN, BOTTLENECK, SIZE> lae2;
        lae2.train(h1_data, 300, 0.01);

        // 用预训练好的权重初始化深度自编码器
        ae.set_encoder(lae1.enc, lae2.enc);
        ae.d1.copy_from(lae2.dec);
        ae.d2.copy_from(lae1.dec);

        // ====== Phase 2: Deep AE Fine-tuning ======
        ae.train(norm_data, 300, 0.001);

        // ====== Phase 3: DEC KL-divergence Optimization ======
        for (int i = 0; i < SIZE; i++) ae.encode_one(norm_data[i], i);
        kmeans_on_bottleneck();

        double grad_z[SIZE][BOTTLENECK];

        for (int iter = 0; iter < 200; iter++) {
            compute_q();
            compute_p();

            double kl = kl_loss();
            if (iter > 10 && kl > prev_kl - 1e-8) break;
            prev_kl = kl;

            // Step A: 先做 KL 反向传播 (此时 h1/bot 是正确的编码值)
            compute_dec_gradient(grad_z);
            ae.e1.zero_grad(); ae.e2.zero_grad();
            for (int i = 0; i < SIZE; i++)
                ae.backward_from_bottleneck(norm_data[i], i, grad_z[i]);

            // Step B: 再做重建损失反向传播 (会覆盖 h1/bot, 但这是最后一组梯度)
            ae.d1.zero_grad(); ae.d2.zero_grad();
            for (int i = 0; i < SIZE; i++) {
                ae.fwd_one(norm_data[i], i);
                ae.bwd_one(norm_data[i], i);
            }

            ae.e1.step(0.001, 0.9); ae.e2.step(0.001, 0.9);
            ae.d1.step(0.001, 0.9); ae.d2.step(0.001, 0.9);

            // 重新编码, 更新簇中心
            for (int i = 0; i < SIZE; i++) ae.encode_one(norm_data[i], i);

            double sum_q[10] = {};
            double new_mu[10][BOTTLENECK] = {};
            for (int i = 0; i < SIZE; i++) {
                double* zi = ae.bot + i * BOTTLENECK;
                for (int j = 0; j < K; j++) {
                    sum_q[j] += q[i][j];
                    for (int d = 0; d < BOTTLENECK; d++)
                        new_mu[j][d] += q[i][j] * zi[d];
                }
            }
            for (int j = 0; j < K; j++)
                if (sum_q[j] > 1e-10)
                    for (int d = 0; d < BOTTLENECK; d++)
                        centroids[j][d] = new_mu[j][d] / sum_q[j];
        }

        // 最终分配
        compute_q();
        for (int i = 0; i < SIZE; i++) {
            int best = 0;
            for (int j = 1; j < K; j++) if (q[i][j] > q[i][best]) best = j;
            labels[i] = best;
        }
    }
};

#endif
