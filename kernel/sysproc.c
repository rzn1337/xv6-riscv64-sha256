#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

// #include "types.h"
// #include "riscv.h"
// #include "defs.h"
#include "sha256.h"
#include <stddef.h>




void sys_sha256(void) {
    static const char hex_chars[] = "0123456789abcdef";
    const char *input = "Hello world!"; // Replace with your desired string
    uint8_t hash_output[32]; // SHA-256 hash is 32 bytes
    SHA256_CTX ctx;

    // Initialize SHA-256 context
    sha256_init(&ctx);

    // Process the input string
    sha256_update(&ctx, (const uint8_t *)input, strlen(input));

    // Finalize the hash
    sha256_final(&ctx, hash_output);

    printf("SYSTEM CALL IMPLEMENTATION: SHA-256 hash of \"%s\": ", input);
    for (int i = 0; i < 32; i++) {
        char high_nibble = hex_chars[(hash_output[i] >> 4) & 0xF];
        char low_nibble = hex_chars[hash_output[i] & 0xF];
        consputc(high_nibble); // Print high nibble
        consputc(low_nibble);  // Print low nibble
    }
    consputc('\n'); // Print newline
    printf("\n");
}


uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
