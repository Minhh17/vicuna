#include <stdint.h>
#include <uart.h>

volatile int32_t data_v0[16] __attribute__ ((aligned (64))) = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
volatile int32_t data_v1[16] __attribute__ ((aligned (64))) = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
volatile int32_t data_v2[16] __attribute__ ((aligned (64))) = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};
volatile int32_t data_v3[16] __attribute__ ((aligned (64))) = {300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300};
volatile int32_t data_v4[16] __attribute__ ((aligned (64))) = {400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400};
volatile int32_t data_v5[16] __attribute__ ((aligned (64))) = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
volatile int32_t data_v6[16] __attribute__ ((aligned (64))) = {600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600};
volatile int32_t data_v7[16] __attribute__ ((aligned (64))) = {700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700};
volatile int32_t data_v8[16] __attribute__ ((aligned (64))) = {800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800};
volatile int32_t data_v9[16] __attribute__ ((aligned (64))) = {900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900};
volatile int32_t data_v10[16] __attribute__ ((aligned (64))) = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
volatile int32_t data_v11[16] __attribute__ ((aligned (64))) = {1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100};
volatile int32_t data_v12[16] __attribute__ ((aligned (64))) = {1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200};
volatile int32_t data_v13[16] __attribute__ ((aligned (64))) = {1300, 1300, 1300, 1300, 1300, 1300, 1300, 1300, 1300, 1300, 1300, 1300, 1300, 1300, 1300, 1300};
volatile int32_t data_v14[16] __attribute__ ((aligned (64))) = {1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400};
volatile int32_t data_v15[16] __attribute__ ((aligned (64))) = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};

volatile int32_t check_input[16][16] __attribute__ ((aligned (64)));
volatile int32_t result[16][16] __attribute__ ((aligned (64)));

int main(void) {
    uart_printf("Starting Echo module test in Vicuna (Zve32x)...\n");

    // In địa chỉ của các mảng
    uart_printf("Addresses of data arrays:\n");
    uart_printf("data_v0: 0x%x\n", (uint32_t)data_v0);
    uart_printf("data_v1: 0x%x\n", (uint32_t)data_v1);
    uart_printf("data_v2: 0x%x\n", (uint32_t)data_v2);
    uart_printf("data_v3: 0x%x\n", (uint32_t)data_v3);
    uart_printf("data_v4: 0x%x\n", (uint32_t)data_v4);
    uart_printf("data_v5: 0x%x\n", (uint32_t)data_v5);
    uart_printf("data_v6: 0x%x\n", (uint32_t)data_v6);
    uart_printf("data_v7: 0x%x\n", (uint32_t)data_v7);
    uart_printf("data_v8: 0x%x\n", (uint32_t)data_v8);
    uart_printf("data_v9: 0x%x\n", (uint32_t)data_v9);
    uart_printf("data_v10: 0x%x\n", (uint32_t)data_v10);
    uart_printf("data_v11: 0x%x\n", (uint32_t)data_v11);
    uart_printf("data_v12: 0x%x\n", (uint32_t)data_v12);
    uart_printf("data_v13: 0x%x\n", (uint32_t)data_v13);
    uart_printf("data_v14: 0x%x\n", (uint32_t)data_v14);
    uart_printf("data_v15: 0x%x\n", (uint32_t)data_v15);

    // In dữ liệu từ data_v0 đến data_v15
    uart_printf("Data in memory before loading:\n");
    uart_printf("data_v0: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v0[i]);
    uart_printf("\n");
    uart_printf("data_v1: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v1[i]);
    uart_printf("\n");
    uart_printf("data_v2: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v2[i]);
    uart_printf("\n");
    uart_printf("data_v3: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v3[i]);
    uart_printf("\n");
    uart_printf("data_v4: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v4[i]);
    uart_printf("\n");
    uart_printf("data_v5: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v5[i]);
    uart_printf("\n");
    uart_printf("data_v6: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v6[i]);
    uart_printf("\n");
    uart_printf("data_v7: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v7[i]);
    uart_printf("\n");
    uart_printf("data_v8: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v8[i]);
    uart_printf("\n");
    uart_printf("data_v9: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v9[i]);
    uart_printf("\n");
    uart_printf("data_v10: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v10[i]);
    uart_printf("\n");
    uart_printf("data_v11: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v11[i]);
    uart_printf("\n");
    uart_printf("data_v12: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v12[i]);
    uart_printf("\n");
    uart_printf("data_v13: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v13[i]);
    uart_printf("\n");
    uart_printf("data_v14: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v14[i]);
    uart_printf("\n");
    uart_printf("data_v15: ");
    for (int i = 0; i < 16; i++) uart_printf("%d ", data_v15[i]);
    uart_printf("\n");

    uart_printf("Checkpoint 0...\n");
    // Đặt vector type: SEW = 32-bit, LMUL = 1
    int block_size_p;
    asm volatile(        
        "vsetvli %0, %1, e32, m1, ta, ma"    // SEW = 32-bit, LMUL = 1
        : "=r"(block_size_p)
        : "r" (16)        
    );

    uart_printf("Checkpoint 1...\n");
    // Load từng thanh ghi một cách thủ công
    asm volatile(
        "vle32.v v0, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v0)
        : "memory"
    );
    asm volatile(
        "vle32.v v1, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v1)
        : "memory"
    );
    asm volatile(
        "vle32.v v2, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v2)
        : "memory"
    );
    asm volatile(
        "vle32.v v3, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v3)
        : "memory"
    );
    asm volatile(
        "vle32.v v4, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v4)
        : "memory"
    );
    asm volatile(
        "vle32.v v5, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v5)
        : "memory"
    );
    asm volatile(
        "vle32.v v6, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v6)
        : "memory"
    );
    asm volatile(
        "vle32.v v7, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v7)
        : "memory"
    );
    asm volatile(
        "vle32.v v8, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v8)
        : "memory"
    );
    asm volatile(
        "vle32.v v9, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v9)
        : "memory"
    );
    asm volatile(
        "vle32.v v10, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v10)
        : "memory"
    );
    asm volatile(
        "vle32.v v11, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v11)
        : "memory"
    );
    asm volatile(
        "vle32.v v12, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v12)
        : "memory"
    );
    asm volatile(
        "vle32.v v13, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v13)
        : "memory"
    );
    asm volatile(
        "vle32.v v14, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v14)
        : "memory"
    );
    asm volatile(
        "vle32.v v15, (%0)\n"
        "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
        :
        : "r"(data_v15)
        : "memory"
    );

    uart_printf("Checkpoint 2...\n");
    // Kiểm tra dữ liệu đã load vào v0 đến v15 bằng cách lưu từng thanh ghi riêng lẻ
    asm volatile("vse32.v v0, (%0)" : : "r"(check_input[0]) : "memory");
    asm volatile("vse32.v v1, (%0)" : : "r"(check_input[1]) : "memory");
    asm volatile("vse32.v v2, (%0)" : : "r"(check_input[2]) : "memory");
    asm volatile("vse32.v v3, (%0)" : : "r"(check_input[3]) : "memory");
    asm volatile("vse32.v v4, (%0)" : : "r"(check_input[4]) : "memory");
    asm volatile("vse32.v v5, (%0)" : : "r"(check_input[5]) : "memory");
    asm volatile("vse32.v v6, (%0)" : : "r"(check_input[6]) : "memory");
    asm volatile("vse32.v v7, (%0)" : : "r"(check_input[7]) : "memory");
    asm volatile("vse32.v v8, (%0)" : : "r"(check_input[8]) : "memory");
    asm volatile("vse32.v v9, (%0)" : : "r"(check_input[9]) : "memory");
    asm volatile("vse32.v v10, (%0)" : : "r"(check_input[10]) : "memory");
    asm volatile("vse32.v v11, (%0)" : : "r"(check_input[11]) : "memory");
    asm volatile("vse32.v v12, (%0)" : : "r"(check_input[12]) : "memory");
    asm volatile("vse32.v v13, (%0)" : : "r"(check_input[13]) : "memory");
    asm volatile("vse32.v v14, (%0)" : : "r"(check_input[14]) : "memory");
    asm volatile("vse32.v v15, (%0)" : : "r"(check_input[15]) : "memory");

    // In dữ liệu từ v0 đến v15 để kiểm tra
    uart_printf("Data loaded into v0 to v15:\n");
    for (int i = 0; i < 16; i++) {
        uart_printf("Vector register v%d: ", i);
        for (int j = 0; j < 16; j++) {
            uart_printf("%d ", check_input[i][j]);
        }
        uart_printf("\n");
    }
    
    uart_printf("Checkpoint 3...\n");

    // active module Echo in test.c
    uart_printf("Activating Echo module...\n");
    uint32_t dummy = 0xCAFEBABE;  // random, not important 

    asm volatile (
        "vrgather.vx v0, v0, %0\n"
        :                 
        : "r"(dummy)      
        : "v0", "memory"  
    );
    uart_printf("Checkpoint 4...\n");

	asm volatile (".word 0x66A28257"); // opcode tương ứng vcompress.vv v0, v1, v0

	uart_printf("Checkpoint 5...\n");
	
    // Store dữ liệu từ v16 đến v31 bằng cách tách riêng từng lệnh
    asm volatile("vse32.v v16, (%0)" : : "r"(result[0]) : "memory");
    asm volatile("vse32.v v17, (%0)" : : "r"(result[1]) : "memory");
    asm volatile("vse32.v v18, (%0)" : : "r"(result[2]) : "memory");
    asm volatile("vse32.v v19, (%0)" : : "r"(result[3]) : "memory");
    asm volatile("vse32.v v20, (%0)" : : "r"(result[4]) : "memory");
    asm volatile("vse32.v v21, (%0)" : : "r"(result[5]) : "memory");
    asm volatile("vse32.v v22, (%0)" : : "r"(result[6]) : "memory");
    asm volatile("vse32.v v23, (%0)" : : "r"(result[7]) : "memory");
    asm volatile("vse32.v v24, (%0)" : : "r"(result[8]) : "memory");
    asm volatile("vse32.v v25, (%0)" : : "r"(result[9]) : "memory");
    asm volatile("vse32.v v26, (%0)" : : "r"(result[10]) : "memory");
    asm volatile("vse32.v v27, (%0)" : : "r"(result[11]) : "memory");
    asm volatile("vse32.v v28, (%0)" : : "r"(result[12]) : "memory");
    asm volatile("vse32.v v29, (%0)" : : "r"(result[13]) : "memory");
    asm volatile("vse32.v v30, (%0)" : : "r"(result[14]) : "memory");
    asm volatile("vse32.v v31, (%0)" : : "r"(result[15]) : "memory");

    // In kết quả từ v16 đến v31
    uart_printf("Results from Echo module (v16 to v31):\n");
    for (int i = 0; i < 16; i++) {
        uart_printf("Vector register v%d: ", i + 16);
        for (int j = 0; j < 16; j++) {
            uart_printf("%d ", result[i][j]);
        }
        uart_printf("\n");
    }

    while (1) {}
    return 0;
}

