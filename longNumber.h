#ifndef LONG_NUMBER_H
#define LONG_NUMBER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// must be more than 18
#define DEFAULT_LONG_NUMBER_SIZE 25
#if DEFAULT_LONG_NUMBER_SIZE <= 18
#error "DEFAULT_LONG_NUMBER_SIZE must be more than 18"
#endif
#define BASE 100
#define LOGBASE10 2
typedef struct {
    char *digits;   // smallest at left, not symbols — bytes
    size_t size;    // number of allocated bytes for digits
    size_t length;  // number of used digits
    int sign;       // 1 , 0, -1
} longNumber;

unsigned long long max(unsigned long long x, unsigned long long y) {
    return (x > y ? x : y);
}

void add(longNumber *a, const longNumber *b);
void increaseSize(longNumber *a);
void setZero(longNumber *a);


// dynamic, returned str is responsibility of the caller
char *longNumberToStr(longNumber *a) {
    char *str = (char *)calloc(a->length * LOGBASE10 + 10, sizeof(char));
    if (str == NULL) {
        printf("malloc failed");
        exit(1);
    }
    size_t j = 0;
    int flag = 0;
    for (size_t i = 0; i < a->length; i++) {
        str[j] = a->digits[a->length - i - 1] / 10 + '0';
        if (str[j] != '0') flag = 1;
        if (str[j++] == '0' && !flag) j--;
        str[j] = a->digits[a->length - i - 1] % 10 + '0';
        if (str[j++] == '0' && !flag) j--;
        if (str[j] != '0') flag = 1;
    }

    return str;
}

// ub если ullNum это LLONG_MIN
// creates Zero
longNumber createLongNumber(long long ullNum) {
    longNumber newNum;
    newNum.size = DEFAULT_LONG_NUMBER_SIZE;
    newNum.digits = (char *)calloc(newNum.size, sizeof(char));
    if (newNum.digits == NULL) {
        printf("calloc failed\n");
        exit(1);
    }
    newNum.sign = (ullNum == 0 ? 0 : (ullNum > 0 ? 1 : -1));
    ullNum *= newNum.sign;
    newNum.length = 0;
    if (ullNum == 0) newNum.length = 1;
    for (size_t i = 0; ullNum > 0; i++) {
        newNum.digits[i] = ullNum % BASE;
        ullNum /= BASE;
        newNum.length++;
    }
    return newNum;
}

longNumber createLongNumberCopy(const longNumber *a) {
    longNumber newNum = createLongNumber(0);
    add(&newNum, a);
    while (newNum.size < a->size) increaseSize(&newNum);
    return newNum;
}

void assigneLongNumber(longNumber *a, const longNumber *b) {
    setZero(a);
    add(a, b);
}

void negLongNumber(longNumber *a) { a->sign = -1 * a->sign; }

void destroyLongNumber(longNumber *a) {
    free(a->digits);
    a->digits = NULL;
    a->length = 1;
    a->sign = 0;
    a->size = 0;
}

int getDigit(const longNumber *num, int i) {
    if (num->size <= i) {
        printf("index overbound%d \n", i);
        exit(1);
    }
    return num->digits[i];
}

int less(const longNumber *a, const longNumber *b) {
    if (a->length < b->length) return 1;
    for (size_t i = a->length - 1; i >= 0; i--) {
        if (getDigit(a, i) < getDigit(b, i))
            return 1;
        else if (getDigit(a, i) > getDigit(b, i))
            return 0;
    }
    return 0;
}

void increaseSize(longNumber *a) {
    a->digits = (char *)realloc(a->digits, a->size * 2);  //
    if (a->digits == NULL) {
        printf("calloc failed\n");
        exit(1);
    }
    memset(a->digits + a->size, 0, a->size * sizeof(char));
    a->size *= 2;
}

void setZero(longNumber *a) {
    memset(a->digits, 0, a->size);
    a->length = 1;
    a->sign = 0;
}

void add(longNumber *a, const longNumber *b) {
    while (a->size <= b->length) increaseSize(a);
    int sgn = a->sign * b->sign;
    if (sgn == -1) {
        printf("cannot add different signes\n");
        exit(1);
    }
    int r = 0;
    int temp = 0;
    for (size_t i = 0; i < b->length; i++) {
        temp = (getDigit(a, i) + getDigit(b, i) + r);
        a->digits[i] = temp % BASE;
        r = temp / BASE;
    }
    if (r != 0) {
        if (a->length <= b->length) a->length = b->length + 1;
        a->digits[b->length] = getDigit(a, b->length) + r;
    }
    a->length = max(a->length, b->length);
    if (sgn == -1)
        a->sign = (less(a, b) ? -1 * a->sign : (less(b, a) ? a->sign : 0));
}

void mul(longNumber *a, const longNumber *b) {
    while (a->size <= b->length + a->length) increaseSize(a);
    int r = 0;
    longNumber currentProd = createLongNumberCopy(a);
    longNumber c = createLongNumber(0);
    int temp = 0;
    for (size_t i = 0; i < b->length; i++) {
        setZero(&currentProd);
        currentProd.length = 0;
        r = 0;
        for (size_t j = 0; j < a->length; j++) {
            temp = (getDigit(a, j) * getDigit(b, i) + r);
            currentProd.digits[j + i] = temp % BASE;
            r = temp / BASE;
            currentProd.length = max(currentProd.length, i + j + 1);
        }
        if (r) {
            currentProd.digits[i + a->length] += r;
            currentProd.length = max(currentProd.length, i + a->length + 1);
        }
        add(&c, &currentProd);
    }
    setZero(a);
    add(a, &c);

    destroyLongNumber(&currentProd);
    a->sign = a->sign * b->sign;
}

#endif  // LONG_NUMBER_H