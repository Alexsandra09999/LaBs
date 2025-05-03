#include <iostream>

// Узел двусвязного списка
struct Node {
    int disk;
    char from;
    char to;
    Node* prev;
    Node* next;
    
    Node(int d, char f, char t) 
        : disk(d), from(f), to(t), prev(nullptr), next(nullptr) {}
};

// Менеджер списка
class HanoiList {
    Node* head;
    Node* tail;
    
public:
    HanoiList() : head(nullptr), tail(nullptr) {}
    
    // Добавление в конец списка
    void add_move(int disk, char from, char to) {
        Node* new_node = new Node(disk, from, to);
        
        if(!head) {
            head = tail = new_node;
        } else {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
    }
    
    // Вывод списка
    void print() const {
        Node* current = head;
        int counter = 1;
        while(current) {
            std::cout << "Шаг " << counter++ 
                      << ": Диск " << current->disk 
                      << " из " << current->from 
                      << " в " << current->to << "\n";
            current = current->next;
        }
    }
    
    // Очистка памяти
    void clear() {
        Node* current = head;
        while(current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = tail = nullptr;
    }
    
    ~HanoiList() {
        clear();
    }
};

// Рекурсивное решение Ханойской башни
void hanoi(int n, char from, char to, char aux, HanoiList& list) {
    if(n == 1) {
        list.add_move(n, from, to);
        return;
    }
    
    hanoi(n-1, from, aux, to, list);
    list.add_move(n, from, to);
    hanoi(n-1, aux, to, from, list);
}

int main() {
    const int disks = 8;
    HanoiList solution;
    
    // Решаем головоломку
    hanoi(disks, 'A', 'C', 'B', solution);
    
    // Выводим результат
    std::cout << "Последовательность перемещений для " << disks << " дисков:\n";
    solution.print();
    
    // Память очищается автоматически в деструкторе
    return 0;
}