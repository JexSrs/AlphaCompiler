#include "targetCode.h"

void createBinaryFile(char *name) {
    FILE *fp = fopen(name, "wb");
    if (fp == NULL) {
        printf("Cannot open binary file");
        return;
    }

    unsigned int totalGlobalsNo = totalGlobals();

    char *value = NULL;
    unsigned int currStringSize = 0;

    unsigned int magicNumber = 19955991;
    fwrite(&magicNumber, sizeof(magicNumber), 1, fp);  // MagicNumber
    fwrite(&totalNumConsts, sizeof(totalNumConsts), 1, fp);  // totalNumConsts
    fwrite(&totalStringConsts, sizeof(totalStringConsts), 1, fp);  // totalStringConsts
    fwrite(&totalNamedLibFuncs, sizeof(totalNamedLibFuncs), 1, fp);  // totalNamedLibFuncs
    fwrite(&totalUserFuncs, sizeof(totalUserFuncs), 1, fp);  // totalUserFuncs
    fwrite(&totalInstructions, sizeof(totalInstructions), 1, fp);  // totalInstructions
    fwrite(&totalGlobalsNo, sizeof(totalGlobalsNo), 1, fp);  // totalGlobals

    // numConsts
    for (int i = 0; i < totalNumConsts; i++) {
        fwrite(&i, sizeof(int), 1, fp);
        fwrite(&numConsts[i], sizeof(double), 1, fp);
    }

    // stringConsts
    for (int i = 0; i < totalStringConsts; i++) {
        currStringSize = strlen(stringConsts[i]) + 1;

        fwrite(&currStringSize, sizeof(int), 1, fp);

        value = calloc(currStringSize, sizeof(char));
        strcpy(value, stringConsts[i]);

        fwrite(&i, sizeof(int), 1, fp);
        fwrite(value, sizeof(char) * currStringSize, 1, fp);

        value = NULL;
    }

    // userFuncs
    for (int i = 0; i < totalUserFuncs; i++) {
        currStringSize = strlen(userFuncs[i].id) + 1;
        fwrite(&currStringSize, sizeof(int), 1, fp);

        value = calloc(currStringSize, sizeof(char));
        strcpy(value, userFuncs[i].id);

        fwrite(&i, sizeof(int), 1, fp);
        fwrite(&userFuncs[i].address, sizeof(int), 1, fp);
        fwrite(&userFuncs[i].localSize, sizeof(int), 1, fp);
        fwrite(&userFuncs[i].totalArgs, sizeof(int), 1, fp);
        fwrite(value, sizeof(char) * currStringSize, 1, fp);

        value = NULL;
    }

    // namedLibFuncs
    for (int i = 0; i < totalNamedLibFuncs; i++) {
        currStringSize = strlen(namedLibFuncs[i]) + 1;
        fwrite(&currStringSize, sizeof(int), 1, fp);

        value = calloc(currStringSize, sizeof(char));
        strcpy(value, namedLibFuncs[i]);

        fwrite(&i, sizeof(int), 1, fp);
        fwrite(value, sizeof(char) * currStringSize, 1, fp);

        value = NULL;
    }

    struct InstructionToBinary instr;
    for (int i = 0; i < nextInstructionLabel(); i++) {
        instr.instrOpcode = instructions[i].opcode;
        instr.instrLine = (int) instructions[i].srcLine;

        instr.resultType = instructions[i].result.type;
        instr.resultOffset = (int) instructions[i].result.val;

        instr.arg1Type = instructions[i].arg1.type;
        instr.arg1Offset = (int) instructions[i].arg1.val;

        instr.arg2Type = instructions[i].arg2.type;
        instr.arg2Offset = (int) instructions[i].arg2.val;

        fwrite(&instr, sizeof(instr), 1, fp);
    }

    fclose(fp);
}