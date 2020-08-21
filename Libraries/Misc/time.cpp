#include <chrono>
#include <functional>

/*
Run function with argument 'repeat' times.
Calculates duration = average running time in miliseconds. 
Return (result, duration) if 'function' returns a value else duration.
*/
template<typename Function, class ... Types>
decltype(auto) timeit(unsigned repeat, Function&& f, Types&& ... args) {
    using namespace chrono;
    constexpr auto get_duration = [&](const auto& start) -> double {
        return (duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6) / repeat;
    };

    const auto start = high_resolution_clock::now();
    if constexpr(std::is_same_v<std::invoke_result_t<Function, Types...>, void>) {
        for (unsigned i = 0; i < repeat; ++i)
            std::invoke(std::forward<Function>(f), std::forward<Types>(args) ...);
        return get_duration(start);
    }
    else {
        auto result = std::invoke(std::forward<Function>(f), std::forward<Types>(args) ...);
        for (unsigned i = 1; i < repeat; ++i)
            std::invoke(std::forward<Function>(f), std::forward<Types>(args) ...);
        auto duration = get_duration(start);
        return std::make_pair(std::move(result), duration);
    }
}
