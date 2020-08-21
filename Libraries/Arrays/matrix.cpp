// Author	: Rahul Sharma <rahulsrma26@gmail.com>

#include <iostream>
#include <vector>
#include <tuple>
#include <memory>
#include <chrono>
#include <immintrin.h>

template<typename Function, class ... Types>
decltype(auto) timeit(unsigned repeat, Function f, Types&& ... args) {
    using namespace chrono;
    const auto start = high_resolution_clock::now();
    auto result = f(std::forward<Types>(args) ...);
    for (unsigned i = 1; i < repeat; ++i)
        result = f(std::forward<Types>(args) ...);
    double duration = (duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6) / repeat;
    return std::make_pair(std::move(result), duration);
}

uint32_t mat_val = 0;

template<typename T>
class matrix {
    std::unique_ptr<T[]> data;
    size_t rows;
    size_t columns;
public:
    matrix(size_t _rows, size_t _columns, bool random = false) :
        rows(_rows), columns(_columns)
    {
        const size_t n = rows * columns;
        data = std::make_unique<T[]>(n);
        if (random)
            for (size_t i = 0; i < n; ++i)
                data[i] = static_cast<T>((mat_val = 1103515245 * mat_val + 12345) & 15) / 16.0f;
    }

    matrix(const matrix<T>& other) {
        *this = other;
    }

    matrix(matrix<T>&& other) {
        *this = std::move(other);
    }

    inline void fill(const T& value) {
        std::fill(data.get(), data.get() + rows * columns, value);
    }

    matrix<T>& operator = (const matrix<T>& other) {
        rows = other.rows;
        columns = other.columns;
        const size_t n = rows * columns;
        data = std::make_unique<T[]>(n);
        std::copy(other.data.get(), other.data.get() + n, data.get());
        return *this;
    }

    matrix<T>& operator = (matrix<T>&& other) {
        rows = other.rows;
        columns = other.columns;
        data = std::move(other.data);
        return *this;
    }

    bool operator == (const matrix<T>& right) const {
        if (rows != right.rows || columns != right.columns)
            return false;
        const size_t n = rows* columns;
        for (size_t i = 0; i < n; ++i)
            if (data[i] != right.data[i])
                return false;
        return true;
    }

    inline T* operator[](size_t row) {
        return &data[row * rows];
    }

    inline const T* operator[](size_t row) const {
        return &data[row * rows];
    }

    size_t size() const {
        return rows;
    }

    template<typename Ch, typename Tr, typename Tm>
    friend decltype(auto) operator << (std::basic_ostream<Ch, Tr>& out, const matrix<Tm>& a);
};

template<typename Ch, typename Tr, typename T>
decltype(auto) operator << (std::basic_ostream<Ch, Tr>& out, const matrix<T>& a) {
    for (int i = 0; i < a.rows; ++i, out << '\n')
        for (int j = 0; j < a.columns; ++j)
            out << a[i][j] << ' ';
    return out;
}

template<class T>
decltype(auto) transpose(const matrix<T>& a) {
    const size_t N = a.size();
    auto at = matrix<T>(N, N);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            at[i][j] = a[j][i];
    return at;
}

template<class T>
decltype(auto) simpleMul(const matrix<T>& a, const matrix<T>& b) {
    const size_t N = a.size();
    auto c = matrix<T>(N, N);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            T v = 0;
            for (int k = 0; k < N; ++k)
                v += a[i][k] * b[k][j];
            c[i][j] = v;
        }
    return c;
}

template<class T>
decltype(auto) simpleMul_t(const matrix<T>& a, const matrix<T>& b) {
    const size_t N = a.size();
    auto bt = transpose(b);
    auto c = matrix<T>(N, N);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            T v = 0;
            for (int k = 0; k < N; ++k)
                v += a[i][k] * bt[j][k];
            c[i][j] = v;
        }
    return c;
}

auto simpleMul_avx_t(const matrix<float>& a, const matrix<float>& b) {
    const size_t N = a.size();
    auto bt = transpose(b);
    auto c = matrix<float>(N, N);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            __m256 sx = _mm256_setzero_ps();
            for (int k = 0; k < N; k += 8) {
                __m256 ax = _mm256_loadu_ps(&a[i][k]);
                __m256 bx = _mm256_loadu_ps(&bt[j][k]);
                __m256 cx = _mm256_mul_ps(ax, bx);
                sx = _mm256_add_ps(sx, cx);
            }
            float v = 0;
            float* values = (float*)&sx;
            for (int k = 0; k < 8; ++k)
                v += values[k];
            c[i][j] = v;
        }
    return c;
}


auto simpleMul_avx_t2(const matrix<float>& a, const matrix<float>& b) {
    const size_t N = a.size();
    auto c = matrix<float>(N, N);
    c.fill(0);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; j += 8) {
            __m256 ax = _mm256_loadu_ps(&a[i][j]);
            for (int k = 0; k < N; ++k) {
                //c[i][k] += a[i][j] * b[j][k];
                __m256 bx = _mm256_loadu_ps(&b[j][k]);
                __m256 cx = _mm256_loadu_ps(&c[i][k]);
                __m256 dx = _mm256_fmadd_ps(ax, bx, cx);
                _mm256_storeu_ps(&c[i][k], dx);
            }
        }
    return c;
}

template<class T, size_t S>
decltype(auto) tiledMul(const matrix<T>& a, const matrix<T>& b) {
    const size_t N = a.size();
    auto c = matrix<T>(N, N);
    c.fill(0);
    for (int y = 0; y < N; y += S)
        for (int x = 0; x < N; x += S)
            for (int z = 0; z < N; z += S)
                for (int i = y; i < y + S; ++i)
                    for (int j = x; j < x + S; ++j)
                        for (int k = z; k < z + S; ++k)
                            c[i][j] += a[i][k] * b[k][j];
    return c;
}

template<class T, size_t S>
decltype(auto) tiledMul_t(const matrix<T>& a, const matrix<T>& b) {
    const size_t N = a.size();
    auto bt = transpose(b);
    auto c = matrix<T>(N, N);
    c.fill(0);
    for (int y = 0; y < N; y += S)
        for (int x = 0; x < N; x += S)
            for (int z = 0; z < N; z += S)
                for (int i = y; i < y + S; ++i)
                    for (int j = x; j < x + S; ++j)
                        for (int k = z; k < z + S; ++k)
                            c[i][j] += a[i][k] * bt[j][k];
    return c;
}

template<size_t S>
auto tiledMul_avx_t(const matrix<float>& a, const matrix<float>& b) {
    const size_t N = a.size();
    auto c = matrix<float>(N, N);
    c.fill(0);
    for (int r = 0; r < N; ++r)
        for (int x = 0; x < N; x += S)
            for (int z = 0; z < N; z += S)
                for (int i = y; i < y + S; ++i)
                    for (int j = x; j < x + S; ++j)
                        for (int k = z; k < z + S; k += 8) {
                            __m256 ax = _mm256_loadu_ps(&a[i][k]);
                            __m256 bx = _mm256_loadu_ps(&b[j][k]);
                            __m256 cx = _mm256_loadu_ps(&c[i][j]);
                            __m256 dx = _mm256_fmadd_ps(ax, bx, cx);
                            _mm256_storeu_ps(&c[i][j], dx);
                        }
    return c;
}

int main() {
    using namespace std;
    ios::sync_with_stdio(false);
    cin.tie(0);

    using mType = float;
    constexpr size_t N = 1024, T = 32;
    const auto a = matrix<mType>(N, N, true);
    const auto b = matrix<mType>(N, N, true);
    cout << &a[0][0] << ", " << &b[0][0] << '\n';

    auto[at, ta] = timeit(100, transpose<mType>, a);
    auto[att, tat] = timeit(100, transpose<mType>, at);
    cout << "double transpose: " << ta << "+" << tat << " = " << ta + tat << " ms, ";
    cout << (a == att ? "OK" : "WRONG") << '\n';

    /*auto[c1, t1] = timeit(1, simpleMul<mType>, a, b);
    cout << "mul_simple: " << t1 << " ms" << '\n';

    auto[c2, t2] = timeit(1, simpleMul_t<mType>, a, b);
    cout << "mul_transpose: " << t2 << " ms, ";
    cout << (double)t1 / t2 << "x " << (c1 == c2 ? "OK" : "WRONG") << '\n';
    */

    auto[c1, t1] = timeit(1, simpleMul_t<mType>, a, b);
    cout << "mul_transpose: " << t1 << " ms \n";

    auto[c3, t3] = timeit(1, simpleMul_avx_t, a, b);
    cout << "mul_AVX-256_t: " << t3 << " ms, ";
    cout << (double)t1 / t3 << "x " << (c1 == c3 ? "OK" : "WRONG") << '\n';

    auto[c4, t4] = timeit(1, simpleMul_avx_t2, a, b);
    cout << "mul_AVX-256_t2: " << t4 << " ms, ";
    cout << (double)t1 / t4 << "x " << (c1 == c4 ? "OK" : "WRONG") << '\n';

    /*auto[c4, t4] = timeit(1, tiledMul<mType, T>, a, b);
    cout << "mul_tiled: " << t4 << " ms, ";
    cout << (double)t1 / t4 << "x " << (c1 == c4 ? "OK" : "WRONG") << '\n';

    auto[c5, t5] = timeit(1, tiledMul_t<mType, T>, a, b);
    cout << "mul_tiled_transpose: " << t5 << " ms, ";
    cout << (double)t1 / t5 << "x " << (c1 == c5 ? "OK" : "WRONG") << '\n';
    */

    /*auto[c6, t6] = timeit(1, tiledMul_avx_t<T>, a, b);
    cout << "mul_tiled_AVX-256_t: " << t6 << " ms, ";
    cout << (double)t1 / t6 << "x " << (c1 == c6 ? "OK" : "WRONG") << '\n';*/
}

/*
http://quick-bench.com/WuDuZCheQQSF8eL0e-MXchnAW_c

N = 2048, T = 32;
simple: 71975.4 ms
transpose:  8687.6 ms, 8.28484x OK
tiled: 33239.0 ms, 2.16539x OK
tiled_transpose: 29047.9 ms, 2.47782x OK


N = 1536, T = 32;
double transpose: 20.6665+20.6521 = 41.3185 ms, OK
mul_simple: 25668.6 ms
mul_transpose:  3597.3 ms, 7.1356x OK
mul_AVX-256_t:   847.4 ms, 30.2897x OK
mul_tiled: 10095.8 ms, 2.5425x OK
mul_tiled_transpose:  9237.7 ms, 2.77868x OK
mul_tiled_AVX-256_t:  1136.3 ms, 22.5905x OK


*/