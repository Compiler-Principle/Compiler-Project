#include <stdio.h>

int a[25][25];
int b[25][25];
int c[25][25];
int ma, na, mb, nb;

int main() {
    scanf("%d %d", &ma, &na);
    for(int i = 0;i < ma;i++) {
        for(int j = 0;j < na;j++) {
            scanf("%d", &a[i][j]);
        }
    }

    scanf("%d %d", &mb, &nb);
    for(int i = 0;i < mb;i++) {
        for(int j = 0;j < nb;j++) {
            scanf("%d", &b[i][j]);
        }
    }

    if(na != mb) {
        printf("Incompatible Dimensions\n");
        return 0;
    }

    for(int i = 0;i < ma;i++) {
        for(int j = 0;j < nb;j++) {
            c[i][j] = 0;
        }
    }

    for(int i = 0;i < ma;i++) {
        for(int j = 0;j < nb;j++) {
            for(int k = 0;k < na;k++) {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
            }
        }
    }

    for(int i = 0;i < ma;i++) {
        for(int j = 0;j < nb;j++) {
            printf("%10d", c[i][j]);
        }
        printf("\n");
    }

    return 0;
}