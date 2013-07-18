#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <string.h>

#include "printk.h"

struct multiboot_info *mb_info;

struct kernel_time {
    unsigned long uptime_ms;
};
extern struct kernel_time timekeeper;

/* Kernel panic: print given message and halt the CPU */
void panic(char *msg)
    __attribute__((noreturn));

/* in cpu.c */
void init_cpu(void);

#endif
