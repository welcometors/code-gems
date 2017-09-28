#include <chrono>

template<typename Function, class ... Types>
decltype(auto) timeit(Function f, Types ... args) {
    using namespace chrono;
    auto start = high_resolution_clock::now();
    auto result = f(args...);
    double duration = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6;
    return std::make_pair(result, duration);
}