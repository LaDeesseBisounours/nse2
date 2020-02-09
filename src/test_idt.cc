#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct idt_record
{
    uint16_t  limit;      /* Size of IDT array - 1 */
    //uintptr_t base;       /* Pointer to IDT array  */
    uint8_t v1;
    uint8_t v2;
    uint8_t v3;
    uint8_t v4;
    uint8_t v5;
    uint8_t v6;
} __attribute__((packed));


void load_idt (struct idt_record *idt_r) {
    __asm__ volatile ("sidt %0" :: "m"(*idt_r));
}

int main(void) {
    struct idt_record *idtr = (struct idt_record*)malloc(sizeof(struct idt_record));
    if (!idtr)
        exit(1);
    load_idt(idtr);
    unsigned long limit = idtr->limit;
    printf("limit %x\n", limit);

    unsigned long v1 = idtr->v1;
    unsigned long v2 = idtr->v2;
    unsigned long v3 = idtr->v3;
    unsigned long v4 = idtr->v4;

    printf("v1 %x\n", v1);
    printf("v2 %x\n", v2);
    printf("v3 %x\n", v3);
    printf("v4 %x\n", v4);

    unsigned long v5 = idtr->v5;
    printf("v5 %x\n", v5);

    unsigned long v6 = idtr->v6;
    printf("v6 %x\n", v6);

    free(idtr);
    return 0;
}
