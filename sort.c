#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

static double abs_d(double a) { //взятие модуля
    return (a > 0.00000000001) ? a : -a;
}

static void gen_mas_1(double *a, int n) { //по неубыванию
    for (int i = 0; i < n; ++i) {
        *(a + i) = ((double)rand() / 10000000000 + rand()) * ((rand() % 2) ? -1 : 1);
        int j = i;
        while (j && (abs_d(*(a + j - 1)) - abs_d(*(a + j)) >  0.00000000001)) { //цикл для перемещения нового элемента пока массив не станет неубываниющим
            double vr = *(a + j - 1);
            *(a + j - 1) = *(a + j);
            *(a + j) = vr;
            --j;
        }
    }
}

static void gen_mas_2(double *a, int n) { //по невозрастанию
    for (int i = 0; i < n; ++i) {
        *(a + i) = ((double)rand() / 10000000000 + rand()) * ((rand() % 2) ? -1 : 1);
        int j = i;
        while (j && (abs_d(*(a + j)) - abs_d(*(a + j - 1)) >  0.00000000001)) { //цикл для перемещения нового элемента пока массив не станет невозрастающим
            double vr = *(a + j - 1);
            *(a + j - 1) = *(a + j);
            *(a + j) = vr;
            --j;
        }
    }
}

static void gen_mas_34(double *a, int n) { //случайный порядок
    for (int i = 0; i < n; ++i) *(a + i) = ((double)rand() / 10000000000 + rand()) * ((rand() % 2) ? -1 : 1);
}

static void count_out(int k) { //функция счётчика и вывода их значений
    static int count_comp = 0, count_perm = 0;

    if (k == 1) ++count_comp;
    if (k == 2) ++count_perm;
    if (!k) {
        printf("Число сравнений: %d\nЧисло перемещений: %d\n", count_comp, count_perm);
        count_comp = 0;
        count_perm = 0;
    }
}

static bool comp(double x, double y) { //сравнение
    count_out(1);
    return abs_d(x) - abs_d(y) >  0.00000000001;
}

static void perm(double *x, double *y) { //перестановка
    count_out(2);
    double vr = *x;
    *x = *y;
    *y = vr;
}

static void bub_sort(double *a, int n) { //сортировка пузырьком
    for (int i = 0; i < n; ++i) {
        for (int j = n - 1; j > i; --j) {
            if (comp(*(a + j - 1), *(a + j))) perm(a + j - 1, a + j);
        }
    }
}

static void heap_sort(double *a, int n) { //сортировка кучей
    double *s = (double *) malloc(n * sizeof(double));

    for (int i = 0; i < n; ++i) { //вставка
        s[i] = a[i];
        int j = i;
        while (j && comp(s[j], s[(j - 1) / 2])) { //просеивание нового элемента
            perm(s + (j - 1) / 2, s + j);
            j = (j - 1) / 2;
        }
    }

    for (int i = n - 1; i >= 0; --i) { //взятие максимума
        double vr = s[0];
        s[0] = s[i];
        s[i] = vr;

        int j = 0;
        while (1) { //просеивание элемента
            int k = j;
            if (2 * j + 1 < i && comp(s[2 * j + 1], s[k])) k = 2 * j + 1;
            if (2 * j + 2 < i && comp(s[2 * j + 2], s[k])) k = 2 * j + 2;

            if (k == j) break;
            perm(s + j, s + k);
            j = k;
        }
    }

    for (int i = 0; i < n; ++i) *(a + i) = s[i];
    free(s);
}

static bool test(double *a, double *an, int n) { //функция проверяющая корректность сортировки
    bool ans = 1;
    bool *t = (bool *) malloc(n * sizeof(bool));

    for (int i = 0; i < n; ++i) t[i] = 0;

    for (int i = 1; i < n; ++i) { //проверка на упорядоченность
        if (abs_d(a[i - 1]) - abs_d(a[i]) > 0.00000000001) {
            ans = 0;
            break;
        }
    }

    for (int i = 0; i < n; ++i) { //проверка на идентичность элементов
        if (!ans) break;
        for (int j = 0; j < n; ++j) {
            if (!t[j] && abs_d(an[i] - a[j]) < 0.00000000001) {
                t[j] = 1;
                break;
            }
            if (j == n - 1) ans = 0;
        }
    }

    free(t);
    return ans;
}

int main(void) {
    srand(time(NULL));

    char s[100]; //считаем вводится число типа unsigned int
    if (scanf("%s", s) != 1) return 1;
    int n = 0, l = strlen(s);
    if (l > 10) return 1;
    for (int i = 0; i < l; ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            n *= 10;
            n += s[i] - '0';
        }
        else return 1;
    }

    double *a1 = (double *) malloc(n * sizeof(double)), *a1n = (double *) malloc(n * sizeof(double));
    double *a2 = (double *) malloc(n * sizeof(double)), *a2n = (double *) malloc(n * sizeof(double));
    double *a3 = (double *) malloc(n * sizeof(double)), *a3n = (double *) malloc(n * sizeof(double));
    double *a4 = (double *) malloc(n * sizeof(double)), *a4n = (double *) malloc(n * sizeof(double));

    gen_mas_1(a1, n);
    gen_mas_2(a2, n);
    gen_mas_34(a3, n);
    gen_mas_34(a4, n);

    for (int i = 0; i < n; ++i) a1n[i] = a1[i]; //копирование элементов
    for (int i = 0; i < n; ++i) a2n[i] = a2[i]; //в массивы над котрыми
    for (int i = 0; i < n; ++i) a3n[i] = a3[i]; //не будет производиться
    for (int i = 0; i < n; ++i) a4n[i] = a4[i]; //сортировка

    printf("В случае сортировки методом \"пузырька\" имеем:\nЕсли массив был уже упорядочен, то\n");
    bub_sort(a1, n);
    count_out(0);
    printf("Вердикт тестирующей системы: ");
    if (test(a1, a1n, n)) printf("массив отсортирован верно\n");
    else printf("массив отсортирован неверно\n");

    printf("Если элементы массива упорядочены в обратном порядке, то\n");
    bub_sort(a2, n);
    count_out(0);
    printf("Вердикт тестирующей системы: ");
    if (test(a2, a2n, n)) printf("массив отсортирован верно\n");
    else printf("массив отсортирован неверно\n");

    printf("Если расстановка элементов массива случайна (1), то\n");
    bub_sort(a3, n);
    count_out(0);
    printf("Вердикт тестирующей системы: ");
    if (test(a3, a3n, n)) printf("массив отсортирован верно\n");
    else printf("массив отсортирован неверно\n");

    printf("Если расстановка элементов массива случайна (2), то\n");
    bub_sort(a4, n);
    count_out(0);
    printf("Вердикт тестирующей системы: ");
    if (test(a4, a4n, n)) printf("массив отсортирован верно\n\n");
    else printf("массив отсортирован неверно\n\n");

    for (int i = 0; i < n; ++i) a1[i] = a1n[i]; //возвращаем
    for (int i = 0; i < n; ++i) a2[i] = a2n[i]; //исходным массивам
    for (int i = 0; i < n; ++i) a3[i] = a3n[i]; //элементы в неотсортированном
    for (int i = 0; i < n; ++i) a4[i] = a4n[i]; //порядке

    printf("В случае пирамидальной сортировки имеем:\nЕсли массив был уже упорядочен, то\n");
    heap_sort(a1, n);
    count_out(0);
    printf("Вердикт тестирующей системы: ");
    if (test(a1, a1n, n)) printf("массив отсортирован верно\n");
    else printf("массив отсортирован неверно\n");

    printf("Если элементы массива упорядочены в обратном порядке, то\n");
    heap_sort(a2, n);
    count_out(0);
    printf("Вердикт тестирующей системы: ");
    if (test(a2, a2n, n)) printf("массив отсортирован верно\n");
    else printf("массив отсортирован неверно\n");

    printf("Если расстановка элементов массива случайна (1), то\n");
    heap_sort(a3, n);
    count_out(0);
    printf("Вердикт тестирующей системы: ");
    if (test(a3, a3n, n)) printf("массив отсортирован верно\n");
    else printf("массив отсортирован неверно\n");

    printf("Если расстановка элементов массива случайна (2), то\n");
    heap_sort(a4, n);
    count_out(0);
    printf("Вердикт тестирующей системы: ");
    if (test(a4, a4n, n)) printf("массив отсортирован верно\n");
    else printf("массив отсортирован неверно\n");

    free(a1);
    free(a2);
    free(a3);
    free(a4);
    free(a1n);
    free(a2n);
    free(a3n);
    free(a4n);
    return 0;
}
