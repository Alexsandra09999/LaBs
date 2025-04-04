#include <iostream>
#include <chrono>
#include <limits>
#include <filesystem>

using namespace std;
using namespace chrono;

inline double calculateExpression(double x) {
    return (x * x) - (x * x) + (x * 4) - (x * 5) + x + x; 
}

int main() {
    while (true) {
        cout << "Введите пажалуста количество итераций (n): ";
        long long n;
        cin >> n;

        if (cin.fail()) {
            cerr << "Это не число!. Пока" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        if (n < 0) {
            cerr << "Не балуйся! Введи неотрицательное число" << endl;
            continue;
        }

        auto start = high_resolution_clock::now();
        double result = 0.0;
        
        for (long long i = 0; i < n; ++i) {
            result = calculateExpression(i);
        }
        
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        
        cout << "Последний результат вычислений: " << result << endl;
        cout << "Время выполнения: " << duration.count() << " мкс" << endl;

        cout << "Хотите повторить?) (y/n): ";
        char choice;
        cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            break;
        }
    }
    return 0;
}