#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


unsigned long get_addr_idt (void)
{
    printf("skdkflsjdf");
    unsigned char idtr[6];
    unsigned long idt;
    __asm__ volatile ("sidt %0": "=m" (idtr));
    idt = *((unsigned long *) &idtr[2]);
    printf("skdkflsjdf");
    return(idt);
}

int main(void) {
    printf("skdkflsjdf");
    unsigned long idtp = get_addr_idt();
    printf("unsigned %lu\n", idtp);
    printf("hexa %lx\n", idtp);
    return 0;
}
