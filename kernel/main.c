#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "sha256.h"
// #include <stdint.h>
// #include <stddef.h>
#include <stddef.h>
#include <stdint.h>

volatile static int started = 0;

// void kernelsha256_test() {
//     // Hardcoded string
//     const char *message = "Hello, xv6!";
//     uint8_t hash[32]; // SHA-256 produces a 32-byte hash
//     SHA256_CTX ctx;

//     // Initialize SHA-256 context
//     sha256_init(&ctx);

//     // Compute hash for the hardcoded string
//     sha256_update(&ctx, (uint8_t *)message, strlen(message));
//     sha256_final(&ctx, hash);

//     // Print the computed hash
//     printf("SHA-256 hash of hardcoded string\n");
//     for (int i = 0; i < 32; i++) {
//         printf("%02x", hash[i]);
//     }
//     printf("\n"); 
// }

void print_hash(uint8_t hash[]) {
    static const char hex_chars[] = "0123456789abcdef";
    for (int i = 0; i < 32; i++) {
        char high_nibble = hex_chars[(hash[i] >> 4) & 0xF];
        char low_nibble = hex_chars[hash[i] & 0xF];
        consputc(high_nibble); // Print high nibble
        consputc(low_nibble);  // Print low nibble
    }
    consputc('\n'); // Print newline
}

void kernelsha256_test() {
    // Hardcoded string
    const char *message = "Hello, xv6!";
    uint8_t hash[32]; // SHA-256 produces a 32-byte hash
    SHA256_CTX ctx;

    // Initialize SHA-256 context
    sha256_init(&ctx);

    // Compute hash for the hardcoded string
    sha256_update(&ctx, (uint8_t *)message, strlen(message));
    sha256_final(&ctx, hash);

    // Print the computed hash in hexadecimal format
    // printf("SHA-256 hash of \"%s\": ", message);
    // for (int i = 0; i < 32; i++) {
    //     printf("%02x", hash[i]);  // Print each byte in hexadecimal
    // }

    printf("KERNEL SPACE IMPLEMENTATION: SHA-256 hash of \"%s\": ", message);

    print_hash(hash);   

    printf("\n\n");
}


void main() {
    if (cpuid() == 0) {
        consoleinit();
        printfinit();
        printf("\n");
        printf("xv6 kernel is booting\n");
        printf("\n");
        kinit();         // physical page allocator
        kvminit();       // create kernel page table
        kvminithart();   // turn on paging
        procinit();      // process table
        trapinit();      // trap vectors
        trapinithart();  // install kernel trap vector
        plicinit();      // set up interrupt controller
        plicinithart();  // ask PLIC for device interrupts
        binit();         // buffer cache
        iinit();         // inode table
        fileinit();      // file table
        virtio_disk_init(); // emulated hard disk
        userinit();      // first user process
        __sync_synchronize();
        started = 1;

        // Print the SHA-256 hash
        kernelsha256_test();
    } else {
        while (started == 0)
            ;
        __sync_synchronize();
        printf("hart %d starting\n\n", cpuid());
        kvminithart();    // turn on paging
        trapinithart();   // install kernel trap vector
        plicinithart();   // ask PLIC for device interrupts
    }

    scheduler();        
}