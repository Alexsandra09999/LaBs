#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

// Функция сложения
int add(int a, int b) {
    return a + b;
}

// Функция вычитания
int subtract(int a, int b) {
    return a - b;
}

// Функция возведения в степень (только целые степени)
int power(int base, int exponent) {
    int result = 1;
    bool negative = false;

    if (exponent < 0) {
        // Для простоты ограничимся: степень не может быть отрицательной
        cout << "Ошибочка: отрицательная степень не поддерживается." << endl;
        return 0;
    }

    for (int i = 0; i < exponent; ++i) {
        result = result * base;
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Использование: CALC <чиселко_раз> <оператор> <чиселко_два>" << endl;
        return 1;
    }

    // Преобразуем аргументы
    int a = atoi(argv[1]);
    string op = argv[2];
    int b = atoi(argv[3]);

    // Выбор операции
    if (op == "+") {
        cout << "Результат: " << add(a, b) << endl;
    } else if (op == "-") {
        cout << "Результат: " << subtract(a, b) << endl;
    } else if (op == "^") {
        cout << "Результат: " << power(a, b) << endl;
    } else {
        cout << "Ошибояка! неподдерживаемый оператор '" << op << "'." << endl;
        return 1;
    }

    return 0;
}
