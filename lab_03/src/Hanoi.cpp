#include <stdio.h>
#include <stdlib.h>

// Структура для элемента двусвязного списка
typedef struct Node {
    int disk;
    char from;
    char to;
    struct Node* prev;
    struct Node* next;
} Node;

// Глобальные переменные для головы и хвоста списка
Node* head = NULL;
Node* tail = NULL;

// Функция для добавления хода в список
void add_move(int disk, char from, char to) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->disk = disk;
    new_node->from = from;
    new_node->to = to;
    new_node->prev = tail;
    new_node->next = NULL;

    if (tail == NULL) {
        head = new_node;
        tail = new_node;
    } else {
        tail->next = new_node;
        tail = new_node;
    }
}

// Рекурсивная функция решения Ханойской башни
void hanoi(int n, char from_rod, char to_rod, char aux_rod) {
    if (n == 1) {
        add_move(n, from_rod, to_rod);
        return;
    }
    hanoi(n - 1, from_rod, aux_rod, to_rod);
    add_move(n, from_rod, to_rod);
    hanoi(n - 1, aux_rod, to_rod, from_rod);
}

// Функция для печати списка ходов
void print_moves() {
    Node* current = head;
    int move_num = 1;
    while (current != NULL) {
        printf("Ход %d: Переместить диск %d с %c на %c\n", 
               move_num++, current->disk, current->from, current->to);
        current = current->next;
    }
}

// Функция для освобождения памяти списка
void free_list() {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    tail = NULL;
}

int main() {
    int num_disks = 8;
    
    printf("Решение головоломки Ханойская башня с дисками:\n", num_disks);
    hanoi(num_disks, 'A', 'C', 'B');
    print_moves();
    
    free_list();
    return 0;
}
