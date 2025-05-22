#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <fstream>
#include <memory>
#include <mutex>
#include <cstdlib>
#include <ctime>
#include <clocale>

using namespace std;

mutex console_mutex;

// Базовый класс для команд
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

// Команда вывода в консоль
class PrintCommand : public Command {
    string message;
public:
    PrintCommand(const string& msg) : message(msg) {}
    void execute() override {
        lock_guard<mutex> lock(console_mutex);
        cout << "Вывод: " << message << endl;
    }
};

// Команда записи в файл
class FileAppendCommand : public Command {
    string filename, content;
public:
    FileAppendCommand(const string& fname, const string& cont) 
        : filename(fname), content(cont) {}
    
    void execute() override {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            lock_guard<mutex> lock(console_mutex);
            cerr << "Ошибочка: Не удалось открыть файл " << filename << endl;
            return;
        }
        file << content << endl;
    }
};

// Команда арифметических операций
class ArithmeticCommand : public Command {
    double a, b;
    char op;
public:
    ArithmeticCommand(double x, char operation, double y) : a(x), b(y), op(operation) {}
    
    void execute() override {
        double result = 0;
        switch (op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/': 
                if (b == 0) {
                    lock_guard<mutex> lock(console_mutex);
                    cerr << "Ошибочка: Деление на ноль" << endl;
                    return;
                }
                result = a / b; 
                break;
            default:
                lock_guard<mutex> lock(console_mutex);
                cerr << "Ошибочка: Неверный оператор '" << op << "'" << endl;
                return;
        }
        lock_guard<mutex> lock(console_mutex);
        cout << a << " " << op << " " << b << " = " << result << endl;
    }
};

// Команда для вывода случайного анекдота на русском
class JokeCommand : public Command {
    vector<string> jokes = {
        "Почему скелет не пошёл на вечеринку? Потому что у него не было тела, чтобы танцевать!",
        "Как узнать, что кот программировал всю ночь? По следам лапок на клавиатуре!",
        "Почему математик всегда спокоен? Потому что он знает, как свести всё к нулю!",
        "Что сказал Wi-Fi роутер лампочке? Давай зажжём эту сеть!"
    };
public:
    void execute() override {
        lock_guard<mutex> lock(console_mutex);
        cout << "Шутейка: " << jokes[rand() % jokes.size()] << endl;
    }
};

// Команда цикла
class LoopCommand : public Command {
    int iterations;
    vector<unique_ptr<Command>> commands;
public:
    LoopCommand(int n) : iterations(n) {}
    
    void addCommand(unique_ptr<Command> cmd) {
        commands.push_back(move(cmd));
    }
    
    void execute() override {
        for (int i = 0; i < iterations; ++i) {
            for (const auto& cmd : commands) {
                cmd->execute();
            }
        }
    }
};

// Парсер команд
class Parser {
public:
    static vector<vector<unique_ptr<Command>>> parseInput() {
        vector<vector<unique_ptr<Command>>> command_sets;
        string line;
        
        cout << "Введите команды (введите 'КОНЕЦ' для завершения):\n";
        while (getline(cin, line)) {
            if (line == "КОНЕЦ") break;
            
            vector<unique_ptr<Command>> commands;
            istringstream iss(line);
            string token;
            
            while (iss >> token) {
                if (token == "вывод") {
                    string message;
                    getline(iss, message);
                    commands.push_back(unique_ptr<PrintCommand>(new PrintCommand(message)));
                }
                else if (token == "файл") {
                    string filename, content;
                    iss >> filename;
                    getline(iss, content);
                    commands.push_back(unique_ptr<FileAppendCommand>(new FileAppendCommand(filename, content)));
                }
                else if (token == "шутка") {
                    commands.push_back(unique_ptr<JokeCommand>(new JokeCommand()));
                }
                else if (token == "цикл") {
                    int iterations;
                    if (!(iss >> iterations) || iterations < 0) {
                        lock_guard<mutex> lock(console_mutex);
                        cerr << "Ошибочка: Неверное количество итераций\n";
                        continue;
                    }
                    auto loop = unique_ptr<LoopCommand>(new LoopCommand(iterations));
                    
                    string nested_line;
                    cout << "Введите команды цикла (введите 'конец_цикла' для завершения):\n";
                    while (getline(cin, nested_line)) {
                        if (nested_line == "конец_цикла") break;
                        
                        istringstream nested_iss(nested_line);
                        string nested_token;
                        nested_iss >> nested_token;
                        
                        if (nested_token == "вывод") {
                            string message;
                            getline(nested_iss, message);
                            loop->addCommand(unique_ptr<PrintCommand>(new PrintCommand(message)));
                        }
                        else if (nested_token == "файл") {
                            string filename, content;
                            nested_iss >> filename;
                            getline(nested_iss, content);
                            loop->addCommand(unique_ptr<FileAppendCommand>(new FileAppendCommand(filename, content)));
                        }
                        else if (nested_token == "шутейка") {
                            loop->addCommand(unique_ptr<JokeCommand>(new JokeCommand()));
                        }
                        else {
                            double a, b;
                            char op;
                            if (nested_iss >> a >> op >> b) {
                                if (op != '+' && op != '-' && op != '*' && op != '/') {
                                    lock_guard<mutex> lock(console_mutex);
                                    cerr << "Ошибочка: Неверный оператор '" << op << "' в цикле\n";
                                    continue;
                                }
                                loop->addCommand(unique_ptr<ArithmeticCommand>(new ArithmeticCommand(a, op, b)));
                            } else {
                                lock_guard<mutex> lock(console_mutex);
                                cerr << "Ошибочка: Неверное арифметическое выражение в цикле\n";
                            }
                        }
                    }
                    commands.push_back(move(loop));
                }
                else {
                    double a, b;
                    char op;
                    iss.putback(token[0]);
                    if (iss >> a >> op >> b) {
                        if (op != '+' && op != '-' && op != '*' && op != '/') {
                            lock_guard<mutex> lock(console_mutex);
                            cerr << "Ошибочка: Неверный оператор '" << op << "'\n";
                            continue;
                        }
                        commands.push_back(unique_ptr<ArithmeticCommand>(new ArithmeticCommand(a, op, b)));
                    } else {
                        lock_guard<mutex> lock(console_mutex);
                        cerr << "Ошибочка: Неверное арифметическое выражение\n";
                    }
                }
            }
            command_sets.push_back(move(commands));
        }
        return command_sets;
    }
};


void executeCommandSet(int set_id, const vector<unique_ptr<Command>>& commands) {
    auto start = chrono::high_resolution_clock::now();
    {
        lock_guard<mutex> lock(console_mutex);
        cout << "Поток для набора команд " << set_id << " начат в " 
             << chrono::duration_cast<chrono::microseconds>(start.time_since_epoch()).count() << " мкс\n";
    }
    
    for (const auto& cmd : commands) {
        cmd->execute();
    }
    
    auto end = chrono::high_resolution_clock::now();
    {
        lock_guard<mutex> lock(console_mutex);
        cout << "Поток для набора команд " << set_id << " завершен в " 
             << chrono::duration_cast<chrono::microseconds>(end.time_since_epoch()).count() << " мкс\n";
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8"); // Поддержка русского текста
    srand(time(nullptr)); // Инициализация генератора случайных чисел
    

    auto command_sets = Parser::parseInput();
    
    // Проверка ввода
    if (command_sets.empty()) {
        cerr << "Нет команд для выполнения\n";
        return 1;
    }
    
    // Создание потоков
    vector<thread> threads;
    for (size_t i = 0; i < command_sets.size(); ++i) {
        threads.emplace_back(executeCommandSet, i + 1, ref(command_sets[i]));
    }
    
    // Ожидание завершения потоков
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    return 0;
}
