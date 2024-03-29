//
// Created by abdul on 2024-03-17.
//

#ifndef XTRA_TRANSLATOR_H
#define XTRA_TRANSLATOR_H

extern bool debugger;
extern unsigned short translatorPC;
extern void translateOP0(unsigned short *instruction);
extern void translateOP1(unsigned short *instruction);
extern void translateOP2(unsigned short *instruction);
extern void translateOPextended(unsigned short *instruction, unsigned short *extended);

#endif //XTRA_TRANSLATOR_H
