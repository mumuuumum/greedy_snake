//
// Created by 张淞钦 on 2021/12/25.
//

#include <stdio.h>
#include <math.h>
/*
 * easy : 150 0.98 70   31
 * medium :130 0.97 62  20
 * difficult :100 0.95 50 12
 */
int main () {
    int a = 100;
    for (int i = 0;;i++) {
        a = a * 0.95;
        if (a - 50 < 1) {
            printf("%d",i);
            break;
        }
    }

    return 0;
}