#include "longNumber.h"

int main(){
    longNumber a = createLongNumber(156420);
    // longNumber b = createLongNumber(50);
    // mul(&a, &b);
    printf("%s\n", longNumberToStr(&a));
}
// 200000000000000000
// 100000000000000000
// 15241578780673483700809383936