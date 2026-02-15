#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* * ヘルパー関数: ヒープメモリの確保とエラーチェック 
 */
double* allocate_doubles(int n) {
    double *ptr = (double*)malloc(n * sizeof(double));
    if (ptr == NULL) {
        fprintf(stderr, "メモリの確保に失敗しました。\n");
        exit(1);
    }
    return ptr;
}

/*
 * 単調性を保つための接線ベクトル(tangents)を計算する関数
 * 入力:
 * values: 点列のY座標 (サイズ N)
 * N: 点の数
 * 出力:
 * 計算された接線配列へのポインタ (サイズ N)
 */
double* compute_monotonic_tangents(double *values, int N) {
    if (N < 2) return NULL;

    // 1. 各区間の傾き (secants: 割線) を計算
    // delta[i] = (y[i+1] - y[i]) / 1.0  (Xの間隔は1と仮定)
    double *secants = allocate_doubles(N - 1);
    for (int i = 0; i < N - 1; i++) {
        secants[i] = values[i + 1] - values[i];
    }

    // 2. 接線 (tangents) の初期化
    double *tangents = allocate_doubles(N);

    // 端点は単純な差分（片側差分）を使用
    tangents[0] = secants[0];
    tangents[N - 1] = secants[N - 2];

    // 内部の点は隣接する傾きの平均を使用
    for (int i = 1; i < N - 1; i++) {
        // 符号が異なる（極値である）場合は、単調性を保つため傾きを0にする
        if (secants[i - 1] * secants[i] < 0.0) {
            tangents[i] = 0.0;
        } else {
            tangents[i] = (secants[i - 1] + secants[i]) / 2.0;
        }
    }

    // 平坦な区間の処理（傾きが0なら接線も0）
    for (int i = 0; i < N - 1; i++) {
        if (secants[i] == 0.0) {
            tangents[i] = 0.0;
            tangents[i + 1] = 0.0;
        }
    }

    // 3. 単調性の厳密な保証 (オーバーシュート防止)
    // 接線ベクトルが半径3の円の外に出ないようにスケーリングする
    for (int i = 0; i < N - 1; i++) {
        if (secants[i] != 0.0) {
            double alpha = tangents[i] / secants[i];
            double beta = tangents[i + 1] / secants[i];
            
            double sum_sq = alpha * alpha + beta * beta;

            // alpha^2 + beta^2 > 9 の場合、係数をスケーリング
            if (sum_sq > 9.0) {
                double tau = 3.0 / sqrt(sum_sq);
                tangents[i] = tau * alpha * secants[i];
                tangents[i + 1] = tau * beta * secants[i];
            }
        }
    }

    free(secants); // 不要になった一時配列を解放
    return tangents;
}

/*
 * エルミート基底関数を用いた補間値の計算
 * p0, p1: 区間の始点と終点の値
 * m0, m1: 区間の始点と終点の接線（傾き）
 * t: 局所パラメータ (0.0 <= t <= 1.0)
 */
double hermite_interpolate(double p0, double p1, double m0, double m1, double t) {
    double t2 = t * t;
    double t3 = t2 * t;

    // エルミート基底関数
    // h00 = 2t^3 - 3t^2 + 1
    double h00 = 2 * t3 - 3 * t2 + 1;
    // h10 = t^3 - 2t^2 + t
    double h10 = t3 - 2 * t2 + t;
    // h01 = -2t^3 + 3t^2
    double h01 = -2 * t3 + 3 * t2;
    // h11 = t^3 - t^2
    double h11 = t3 - t2;

    return (p0 * h00) + (m0 * h10) + (p1 * h01) + (m1 * h11);
}

int main() {
    /* --- 入力処理 --- */
    int N;
    printf("点の数を入力してください: ");
    if (scanf("%d", &N) != 1 || N < 2) {
        fprintf(stderr, "有効な数値を入力してください (N >= 2)\n");
        return 1;
    }

    double *values = allocate_doubles(N);
    printf("各点の値を入力してください (%d個):\n", N);
    for (int i = 0; i < N; i++) {
        scanf("%lf", &values[i]);
    }

    /* --- 計算処理 --- */
    // 単調性を考慮した接線を計算
    double *tangents = compute_monotonic_tangents(values, N);

    /* --- 出力処理 (補間) --- */
    // 各区間を10分割して出力
    // 全体のステップ数は (N-1) * 10
    printf("\n--- 補間結果 ---\n");
    for (int i = 0; i <= 10 * (N - 1); i++) {
        double global_t = i / 10.0;
        
        // どの区間(index)にいるか、および区間内の相対位置(t)を計算
        int idx = (int)global_t;
        
        // 最後の点を超えないようにクランプ
        if (idx >= N - 1) idx = N - 2;

        double t = global_t - idx; // 0.0 から 1.0 の範囲

        // 補間計算
        double out = hermite_interpolate(
            values[idx],      // p0
            values[idx + 1],  // p1
            tangents[idx],    // m0
            tangents[idx + 1],// m1
            t
        );

        printf("%.2f -> %lf\n", global_t, out);
    }

    /* --- メモリ解放 --- */
    free(values);
    free(tangents);

    return 0;
}
