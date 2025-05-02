#include <stdio.h>
#include <stdlib.h>

// Структура для элемента двусвязного списка
typedef struct Node {
    int disk;
    char from;
    char to;
    struct Node* prev; // Указатель на предыдущий элемент списка
    struct Node* next;
} Node;

// Глобальные переменные для головы и хвоста списка
Node* head = NULL; // Указатель на первый элемент списка
Node* tail = NULL;

// Функция для добавления хода в список
void add_move(int disk, char from, char to) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->disk = disk;
    new_node->from = from;
    new_node->to = to;
    new_node->prev = tail; // Предыдущий элемент - текущий хвост
    new_node->next = NULL; // Следующего элемента пока нет

    // Обновляем хвост списка
    if (tail == NULL) {
        // Если список пуст, новый элемент становится и головой и хвостом
        head = new_node;
        tail = new_node;
    } else {
        // Иначе добавляем новый элемент в конец
        tail->next = new_node;
        tail = new_node;
    }
}

// Рекурсивная функция решения Ханойской башни
void hanoi(int n, char from_rod, char to_rod, char aux_rod) {
    if (n == 1) {
        // если диск один, просто перемещаем его
        add_move(n, from_rod, to_rod);
        return;
    }
    // Рекурсивно перемещаем n-1 дисков на вспомогательный стержень
    hanoi(n - 1, from_rod, aux_rod, to_rod);
    // Перемещаем самый большой диск на целевой стержень
    add_move(n, from_rod, to_rod);
    // Рекурсивно перемещаем n-1 дисков с вспомогательного на целевой стержень
    hanoi(n - 1, aux_rod, to_rod, from_rod);
}

// Функция для печати списка ходов
void print_moves() {
    Node* current = head;  // Начинаем с головы списка
    int move_num = 1; // Номер хода
    while (current != NULL) {
        printf("Ход %d: Переместить диск %d с %c на %c\n", 
               move_num++, current->disk, current->from, current->to);
        // Переходим к следующему элементу
        current = current->next;
    }
}

// Функция для освобождения памяти списка
void free_list() {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;      // Запоминаем текущий элемент
        current = current->next;   // Переходим к следующему
        free(temp);                // Освобождаем память текущего элемента
    }
    }
// Обнуляем указатели
    head = NULL;
    tail = NULL;
}

int main() {
    int num_disks = 8;  // Количество дисков

    // Вызываем рекурсивное решение
    // Начальные параметры: 8 дисков, с стержня 'A' на 'C', используя 'B' как вспомогательный
    printf("Решение головоломки Ханойская башня с дисками:\n", num_disks);
    hanoi(num_disks, 'A', 'C', 'B');
    print_moves();

    // Освобождаем память
    free_list();
    return 0;
}
