//
// Created by abdul on 2024-03-17.
//
#include <stdio.h>
#include <stdbool.h>
#include "translator.h"

bool debugger = false;
unsigned short translatorPC;
// Define static arrays for x86 registers
static char regx86 [][255] = {"%rax","%rbx","%rcx","%rdx","%rsi","%rdi","%r8","%r9","%r10","%r11","%r12","%r13","%r14","%r15","%rbp","%rsp"};
// Define static arrays for 8-bit x86 registers
static char bit8regx86 [][255] = {"%al","%bl","%cl","%dl","%sil","%dil","%r8b","%r9b","%r10b","%r11b","%r12b","%r13b","%r14b","%r15b","%bpl","%spl"};

// Define function to translate opcode 0 instructions
extern void translateOP0(unsigned short *instruction){
    unsigned short byte1Instruction;
    byte1Instruction = (*instruction << 8);
    byte1Instruction = (byte1Instruction >> 8);

    if(byte1Instruction == 1){
        printf("    ret\n");
    }
    else if(byte1Instruction == 2){
        debugger = false;
    }
    else if(byte1Instruction == 3){
        debugger = true;
    }
}

// Define function to translate opcode 1 instructions
extern void translateOP1(unsigned short *instruction){
    // Extract byte 1 and byte 2 from the instruction
    unsigned short byte1Instruction;
    byte1Instruction = (*instruction << 8);
    byte1Instruction = (byte1Instruction >> 8);

    unsigned short byte2Instruction;
    byte2Instruction = (*instruction >> 8);

    // Extract if it is register/imm from the instruction
    unsigned short regOrImm;
    regOrImm = (*instruction << 8);
    regOrImm = (regOrImm >> 15);

    unsigned short reg;
    unsigned short immValue;
    // Determine whether the value is register or immediate value
    if(regOrImm == 0){
        reg = (byte2Instruction >> 4);
    }

    if(byte1Instruction == 65){
        printf("    neg %s\n",regx86[reg]);
    }
    else if(byte1Instruction == 66){
        printf("    not %s\n",regx86[reg]);
    }
    else if(byte1Instruction == 72){
        printf("    inc %s\n",regx86[reg]);
    }
    else if(byte1Instruction == 73){
        printf("    dec %s\n",regx86[reg]);
    }
    else if(byte1Instruction == 67){
        printf("    push %s\n",regx86[reg]);
    }
    else if(byte1Instruction == 68){
        printf("    pop %s\n",regx86[reg]);
    }
    else if(byte1Instruction == 71){
        printf("    mov %s, %s\n",regx86[reg],regx86[5]);
        printf("    call outchar\n");
    }
    else if(byte1Instruction == 97){
        unsigned short immValue = (translatorPC + byte2Instruction) - 0x0002;
        printf("    and $%d, %s\n",0x0001,regx86[13]);
        printf("    cmp $%d, %s\n",0x001,regx86[13]);
        printf("    je .L%04x\n",immValue);
    }
    else if(byte1Instruction == 98){
        printf("    jr .L%04x\n",byte1Instruction);
    }
}

// Define function to translate opcode 2 instructions
extern void translateOP2(unsigned short *instruction){
    // Extract byte 1 from the instruction
    unsigned short byte1Instruction;
    byte1Instruction = (*instruction << 8);
    byte1Instruction = (byte1Instruction >> 8);

    // Extract register values from the instruction
    unsigned short register1;
    register1 = (*instruction >> 12);

    unsigned short register2;
    register2 = (*instruction << 4);
    register2 = (register2 >> 12);

    if(byte1Instruction == 129){
        printf("    add %s, %s\n",regx86[register1],regx86[register2]);
    }
    else if(byte1Instruction == 130){
        printf("    sub %s, %s\n",regx86[register1],regx86[register2]);
    }
    else if(byte1Instruction == 131){
        printf("    imul %s, %s\n",regx86[register1],regx86[register2]);
    }
    else if(byte1Instruction == 133){
        printf("    and %s, %s\n",regx86[register1],regx86[register2]);
    }
    else if(byte1Instruction == 134){
        printf("    or %s, %s\n",regx86[register1],regx86[register2]);
    }
    else if(byte1Instruction == 135){
        printf("    xor %s, %s\n",regx86[register1],regx86[register2]);
    }
    else if(byte1Instruction == 138){
        printf("    test %s, %s\n",regx86[register1],regx86[register2]);
        printf("    setnz %sb\n",regx86[13]);
    }
    else if(byte1Instruction == 139){
        printf("    cmp %s, %s\n",regx86[register1],regx86[register2]);
        printf("    setnle %sb\n",regx86[13]);
    }
    else if(byte1Instruction == 140){
        printf("    cmp %s, %s\n",regx86[register1],regx86[register2]);
        printf("    sete %sb\n",regx86[13]);
    }
    else if(byte1Instruction == 141){
        printf("    mov %s, %s\n",regx86[register1],regx86[register2]);
    }
    else if(byte1Instruction == 142){
        printf("    mov (%s), %s\n",regx86[register1],regx86[register2]);
    }
    else if(byte1Instruction == 143){
        printf("    mov %s, (%s)\n",regx86[register1],regx86[register2]);
    }
    else if(byte1Instruction == 144){
        printf("    movb (%s), %s\n",regx86[register1],bit8regx86[register2]);
    }
    else if(byte1Instruction == 145){
        printf("    movb %s, (%s)\n",bit8regx86[register1],regx86[register2]);
    }
}

// Define function to translate extended opcode instructions
extern void translateOPextended(unsigned short *instruction,unsigned short *extended){
    // Extract byte 1 from the instruction
    unsigned short byte1Instruction;
    byte1Instruction = (*instruction << 8);
    byte1Instruction = (byte1Instruction >> 8);

    // Extract byte 2 from the instruction
    unsigned short byte2Instruction;
    byte2Instruction = (*instruction >> 8);

    // Extract extended instruction value
    unsigned short extendedInstruction = (*extended >> 8);
    extendedInstruction = (*extended << 8) | extendedInstruction;

    // Extract register values from the instruction
    unsigned short regOrImm;
    regOrImm = (*instruction << 8);
    regOrImm = (regOrImm >> 15);

    unsigned short reg;
    unsigned short immValue;
    // Determine whether the value is register or immediate value
    if(regOrImm == 0){
        immValue = extendedInstruction;
    }
    else if(regOrImm == 1){
        reg = (byte2Instruction >> 4);
    }

    if(byte1Instruction == 193){
        printf("    jmp .L%04hx\n",extendedInstruction);
    }
    else if(byte1Instruction == 194){
        printf("    call .L%04hx\n",extendedInstruction);
    }
    else if(byte1Instruction == 225){
        printf("    mov $%hu, %s\n",extendedInstruction,regx86[reg]);
    }
}
