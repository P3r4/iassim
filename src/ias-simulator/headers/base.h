#ifndef base_h
#define base_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define MBR_AC 1
#define AC_MBR 2
#define ADD 3
#define SUB 4
#define MQ_AC 5
#define MBR_MQ 6
#define MUL 7
#define DIV 8
#define LSH 9
#define RSH 10
#define AC_MBR_L 11
#define AC_MBR_R 12
#define MAR_PC 13
#define MAR_PC_C 14

#define LCLEAN 0xff000fffff
#define RCLEAN 0xfffffff000

#define MAG 0x8000000000
#define NUM 0x7fffffffff
#define OPL 0xff00000000
#define OPR 0xff000
#define ADL 0xfff00000
#define ADR 0xfff
#define INSTR 0xfffff

#endif
