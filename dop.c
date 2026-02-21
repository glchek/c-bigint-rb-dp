#include <stdio.h>

#include "longNumber.h"
#define TREE_SIZE 1100
// Я считаю, что все пустые листы — чёрные

// B[u][h] — количество способов раскрасить поддерево с корнем в u(черное), что
// черная высота будет h R[u][h] — количество способов раскрасить поддерево с
// корнем в u(красное), что черная высота будет h

int maxBH = 1100;
longNumber *B[TREE_SIZE] = {};  // node, black h
longNumber *R[TREE_SIZE] = {};

int childs[TREE_SIZE][2] = {};
int parents[TREE_SIZE] = {};

longNumber one;

void dfsSize(int node, int step) {
    if (node == 0) {
        maxBH = (step < maxBH ? step : maxBH);
        return;
    }
    dfsSize(childs[node][0], step + 1);
    dfsSize(childs[node][1], step + 1);
}

void dfs(int node) {
    if (node == 0) {
        assigneLongNumber(&B[node][1], &one);
        return;
    }
    dfs(childs[node][0]);
    dfs(childs[node][1]);
    longNumber lb;
    longNumber rb;
    for (int h = 1; h < maxBH; h++) {
        lb = createLongNumberCopy(&B[childs[node][0]][h - 1]);
        rb = createLongNumberCopy(&B[childs[node][1]][h - 1]);
        add(&lb, &R[childs[node][0]][h - 1]);
        add(&rb, &R[childs[node][1]][h - 1]);
        mul(&lb, &rb);
        assigneLongNumber(&B[node][h], &lb);
        destroyLongNumber(&rb);
        destroyLongNumber(&lb);
        // B[node][h] =
        //     (B[childs[node][0]][h - 1] + R[childs[node][0]][h - 1]) *
        //     (B[childs[node][1]][h - 1] + R[childs[node][1]][h - 1]);
        lb = createLongNumberCopy(&B[childs[node][0]][h]);
        mul(&lb, &B[childs[node][1]][h]);
        assigneLongNumber(&R[node][h], &lb);
        destroyLongNumber(&lb);
        // R[node][h] = B[childs[node][0]][h] * B[childs[node][1]][h];
    }
}

void createAndInitLongNumberArray(longNumber **arr, size_t size) {
    *arr = (longNumber *)malloc(size * sizeof(longNumber));
    for (size_t i = 0; i < size; i++) (*arr)[i] = createLongNumber(0);
}

void freeAndDestroyLongNumberArray(longNumber **arr, size_t size) {
    for (size_t i = 0; i < size; i++) destroyLongNumber((*arr) + i);
    free(*arr);
}

int main() {
    one = createLongNumber(1);
    int n;
    scanf("%d", &n);
    int a, b;
    for (int i = 1; i <= n; i++) {
        scanf("%d %d", &a, &b);
        // a = 2 * i;
        // b = 2 * i + 1;
        // if (a > 1000) a = 0;
        // if (b > 1000) b = 0;
        childs[i][0] = a;
        childs[i][1] = b;
        parents[a] = i;
        parents[b] = i;
    }
    int root = -1;
    for (int i = 1; i <= n; i++) {
        if (parents[i] == 0) {
            root = i;
            break;
        }
    }
    dfsSize(root, 1);
    maxBH += 1;
    for (int i = 0; i < TREE_SIZE; i++) {
        createAndInitLongNumberArray(B + i, maxBH);
        createAndInitLongNumberArray(R + i, maxBH);
    }
    dfs(root);
    longNumber ansB = createLongNumber(0);
    longNumber ansR = createLongNumber(0);
    for (int h = 0; h < maxBH; h++) {
        add(&ansB, &B[root][h]);
        add(&ansR, &R[root][h]);
    }
    longNumber sum = createLongNumberCopy(&ansB);
    add(&sum, &ansR);
    printf("%s", longNumberToStr(&sum));
    // printf("black root: %s\nRed root: %s\nTotal: %s", longNumberToStr(&ansB),
    //        longNumberToStr(&ansR), longNumberToStr(&sum));
    for (int i = 0; i < TREE_SIZE; i++) {
        freeAndDestroyLongNumberArray(&B[i], maxBH);
        freeAndDestroyLongNumberArray(&R[i], maxBH);
    }
    return 0;
}