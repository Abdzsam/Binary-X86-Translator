#include <stdio.h>
#include <stdbool.h>
#include "translator.h"

int main(int argc, char **argv) {
        unsigned short buffer;
        FILE *f = fopen(argv[1], "r");

            printf(".globl test\n"
                   "test:\n"
                   "    push %rbp\n"
                   "    mov %rsp, %rbp\n");

            unsigned short PC = 0x0000;
            // Loop until the end of the file is reached
            while (feof(f) == 0) {
                // Read the next 16-bit instruction from the file
                fread(&buffer, sizeof(unsigned short), 1, f);

                // Extract operand from the instruction
                unsigned short operand;
                operand = (buffer << 8);
                operand = (operand >> 14);

                // Update program counter based on the size of the instruction
                if(operand == 3){
                    printf(".L%04x:\n",PC);
                    PC = PC + 0x0004;
                    translatorPC = PC;
                } else{
                    printf(".L%04x:\n",PC);
                    PC = PC + 0x0002;
                    translatorPC = PC;
                }

                // Check if debugger flag is set and call debug function
                if(debugger == true){
                    printf("    call debug\n");
                }

                // Translate and print x86 code based on the operand
                 if(operand == 0){
                    translateOP0(&buffer);
                }
                else if(operand == 1){
                    translateOP1(&buffer);
                }
                else if(operand == 2){
                    translateOP2(&buffer);
                }
                else if(operand == 3){
                     // Read the extended part of the instruction
                    unsigned short extended;
                    fread(&extended, sizeof(unsigned short), 1, f);
                     // Translate and print x86 code for extended instructions
                    translateOPextended(&buffer,&extended);
                }

                // Break loop if buffer is 0
                if(buffer == 0){
                    break;
                }
            }

            printf("    pop %rbp\n"
                   "    ret");
        fclose(f);
        return 0;
}
