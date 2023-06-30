#ifndef ASSEMBLE_H
#define ASSEMBLE_H
#endif // ASSEMBLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NORTYPE 5
#define NOITYPE 8
#define NOJTYPE 2
#define RTYPE 0
#define ITYPE 1
#define JTYPE 2
struct symbolTable{
   int value;
   int intFillValue;
   char symbol[6];
   char valueForFill[7];
};
struct instruction{
   int instType; // 0 means r type, 1 means i type and 2 means j type
   int opCode;;
   char mnemonic[6];
   int rs;
   int rt;
   int rd;
   int imm=0;
   int space=0;
   int fill=0;
};
char *instructions[]={"add","sub","slt","or","nand",
   "addi","slti","ori","lui","lw","sw","beq","jalr",
   "j","halt"};
char hexTable[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
int countOfSymbolsFirstRead(FILE*);
int findSymTabLen(FILE *);
int fillSymTab(struct symbolTable *,FILE *);
void formInst(struct instruction *,FILE *);
int charArray2int(char);
int hex2int(char *);
void int2hex16(char *,int);
void secondRead(FILE* );
void firstReadForinstructions(FILE* ,instruction* ,symbolTable* ,int  );
int findLabelAddress(symbolTable* ,int ,char* );
void secondRead(FILE* outputFile,instruction* instructions,int size,symbolTable* symT,int countOfSymbols);
void fillHexCode(int x,char* hexCode,char* temp);
int loadWordFunction(instruction ins);

