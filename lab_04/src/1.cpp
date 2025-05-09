#include <iostream>
#include <chrono>

double formula1(double x) {
    return x*x - x*x + x*4 - x*5 + x + x;
}

double formula2(double x) {
    return x + x;
}

double formula3(double f1, double f2) {
    return f1 + f2 - f1;
}

void run_sequence(int iterations) {
    using namespace std::chrono;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        double f1 = formula1(i);
        double f2 = formula2(i);
        double f3 = formula3(f1, f2);
    }

    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;

    std::cout << iterations << " занимаемое времечко "
              << diff.count() << " секунд\n";
}

int main() {
    run_sequence(10000);
    run_sequence(100000);
    return 0;
}
