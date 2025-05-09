#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

void calc_formula1(int start, int end, std::vector<double>& results) {
    for (int i = start; i < end; ++i) {
        results[i] = i*i - i*i + i*4 - i*5 + i + i;
    }
}

void calc_formula2(int start, int end, std::vector<double>& results) {
    for (int i = start; i < end; ++i) {
        results[i] = i + i;
    }
}

void run_threads(int iterations) {
    using namespace std::chrono;
    std::vector<double> f1_res(iterations);
    std::vector<double> f2_res(iterations);
    std::vector<double> f3_res(iterations);

    auto start1 = high_resolution_clock::now();

    std::thread t1(calc_formula1, 0, iterations, std::ref(f1_res));
    std::thread t2(calc_formula2, 0, iterations, std::ref(f2_res));

    t1.join();
    auto mid = high_resolution_clock::now();
    t2.join();
    auto end2 = high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        f3_res[i] = f1_res[i] + f2_res[i] - f1_res[i];
    }

    auto end3 = high_resolution_clock::now();

    std::cout << iterations << " сколько штучек\n";
    std::cout << "  Шаг 1 " << duration<double>(mid - start1).count() << " секунд\n";
    std::cout << "  Шаг 2 " << duration<double>(end2 - mid).count() << " секунд\n";
    std::cout << "  Шаг 3 " << duration<double>(end3 - end2).count() << " секунд\n";
}

int main() {
    run_threads(10000);
    
    run_threads(100000);
    return 0;
}
