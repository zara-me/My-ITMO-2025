#include <stdio.h>
#include <stdlib.h>
 
void func(int* a);
 
int main(int argc, char* argv[]) {
    int a = 1;
    // int *  — указатель на тип int
    // указатель хранит адрес
    // &a — получить адрес переменной а
    // *a — получить значение по адресу (разыменование указателя)
    func(&a);
    int* a_ptr = &a;
    printf("a = %d; ptr_a = %p; a from ptr = %d\n", a, a_ptr, *a_ptr);
 
 
    // Зачем тип указывать.
    // 1. типизированный код
    // 2. для арифметики указателей
    // int* — динамические массивы
    // malloc — выделяет память (принимает количество байт)
    // calloc — выделяет память и инициализирует элементы
    // (принимает количество элементов и размер элементов в байтах)
    // realloc — изменяет (выделяет новую) область памяти 
    // (принимает указатель на область памяти и размер на который надо изменить)
    int* array = (int*)malloc(10 * sizeof(int));
    free(array);
    array = (int*)calloc(10, sizeof(int));
    // не нужно делать free
    array = realloc(array, 5 * sizeof(int));
 
    // for (int i = 0; i < 9; ++i) {
    //     array[i] = i;
    //     printf("i = %d\n", i);
    //     printf("array[%d] = %d\n", i, array[i]);
    // }
    array[0] = 1;
    *(array) = 1;
 
    array[1] = 2;
    *(array + 1) = 2;
    *(1 + array) = 2;
    1[array] = 2;
 
    // прибавляя значение к указателю мы прибавляем 
    // столько байт сколько занимает этот тип
 
    // array[n] == *(array + n)
    // *(n + array) == n[array]
    5[array] = 4;
 
    for (int i = 0; i < 11; ++i) {
        printf("array[%d] = %d\n", i, array[i]);
    }
 
    free(array);
    // array => указатель на область памяти
    array = NULL;
}
 
// копируем адрес переменной
void func(int* a) {
    // a = 0x77777
    *a = 4;
}