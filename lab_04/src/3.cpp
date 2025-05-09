#include <iostream>
#include <future>
#include <vector>
#include <chrono>

double formula1(double x) {
    return x*x - x*x + x*4 - x*5 + x + x;
}

double formula2(double x) {
    return x + x;
}

void run_process(int iterations) {
    using namespace std::chrono;
    std::vector<double> f1_res(iterations);
    std::vector<double> f2_res(iterations);
    std::vector<double> f3_res(iterations);

    auto start1 = high_resolution_clock::now();

    auto future1 = std::async(std::launch::async, [&]() {
        for (int i = 0; i < iterations; ++i)
            f1_res[i] = formula1(i);
    });

    auto future2 = std::async(std::launch::async, [&]() {
        for (int i = 0; i < iterations; ++i)
            f2_res[i] = formula2(i);
    });

    future1.wait();
    auto mid = high_resolution_clock::now();
    future2.wait();
    auto end2 = high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        f3_res[i] = f1_res[i] + f2_res[i] - f1_res[i];
    }

    auto end3 = high_resolution_clock::now();

    std::cout <<  iterations << " штучки\n";
    std::cout << "  Шаг 1 " << duration<double>(mid - start1).count() << " секунд\n";
    std::cout << "  Шаг 2 " << duration<double>(end2 - mid).count() << " секунд\n";
    std::cout << "  Шаг 2 " << duration<double>(end3 - end2).count() << " секунд\n";
}

int main() {
    run_process(10000);
    run_process(100000);
    return 0;
}
