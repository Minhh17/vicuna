#include <stdint.h>
#include <uart.h>
#include <stddef.h>

int8_t array[16] __attribute__ ((aligned (4))) = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};

__attribute__((noinline)) void test_echo() {
    asm volatile(".word 0x50000057");
}

int main(void) {
    uart_printf("HaiMinh hello Vicuna Vectorized Program!\n");

    int8_t target[sizeof(array)];
    int8_t *src = array;
    int8_t *dst = target;
    size_t vl;

    asm volatile (
        "vsetvli %0, %1, e8, m1"
        : "=r"(vl)
        : "r"(sizeof(array))
    );

    uart_printf("VL = %d\n", vl);
    uart_printf("Checkpoint 1\n");

    for (int i = 0; i < sizeof(array); i += vl) {
        asm volatile (
            "vle8.v v0, (%0)"
            :
            : "r"(&src[i])
            : "v0"
        );
        uart_printf("Checkpoint 2\n");
        
        uint32_t val = 0xABCD1234;
        asm volatile (
            "vminu.vx v2, v0, %0\n"  // echo test
            :
            : "r"(val)
            : "v2"
        );
        uart_printf("Checkpoint 3\n");

        asm volatile (
            "vadd.vv v2, v0, v0"
            :
            :
            : "v2"
        );
        uart_printf("Checkpoint 32\n");

        asm volatile (
            "vse8.v v2, (%0)"
            :
            : "r"(&dst[i])
            : "v2"
        );
        uart_printf("Checkpoint 4\n");
    }
    uint32_t val = 0xABCD1234;
	asm volatile (
            "vminu.vx v2, v0, %0\n"  // echo test
            :
            : "r"(val)
            : "v2"
        );
    uart_printf("Checkpoint 5\n");
    for (int i = 0; i < sizeof(array); i++) {
        uart_printf("array[%d] = %d, target[%d] = %d\n", i, src[i], i, target[i]);
    }
    test_echo();
    while (1);
    uart_printf("Checkpoint 6\n");

    asm volatile("jr x0");
    return 0;
}

