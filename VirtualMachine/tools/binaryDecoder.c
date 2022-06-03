#include "tools.h"


char *opcodeToString[] = {
        "assign", "add", "sub",
        "mul", "div", "mod",
        "jump", "jeq", "jne",
        "jle", "jge", "jlt",
        "jgt", "call", "pusharg",
        "funcenter", "funcexit", "newtable",
        "tablegetelem", "tablesetelem", "nop"
};

char *typeToStringArray[] = {
        "0  (label), ",
        "1  (global), ",
        "2  (formal), ",
        "3  (local), ",
        "4  (number), ",
        "5  (string), ",
        "6  (bool), ",
        "7  (nil), ",
        "8  (userfunc), ",
        "9  (libfunc), ",
        "10 (retval), "
};

char *typeToString(int type) {
    if (type > 10 || type < 0) {
        return "";
    } else {
        return typeToStringArray[type];
    }
}

char *offsetPrinter(int offset) {
    if (offset == -1) {
        return "";
    } else {
        char *iTs = calloc(1, sizeof(char *));
        sprintf(iTs, "%d", offset);
        return iTs;
    }
}

struct DataTables *decode(int visualize, char *name) {
    unsigned int magicNumberBinary = 0;
    unsigned int totalNumConstsBinary = 0;
    unsigned int totalStringConstsBinary = 0;
    unsigned int totalNamedLibFuncsBinary = 0;
    unsigned int totalUserFuncsBinary = 0;
    unsigned int codeTableSize = 0;
    unsigned int totalGlobals = 0;

    FILE *fp = fopen(name, "rb");
    if (fp == NULL) {
        printf("Failed to open file File a.bin\n");
        exit(0);
    }

    if (fread(&magicNumberBinary, sizeof(magicNumberBinary), 1, fp) == 1) {
        if (magicNumberBinary == 19955991) {
            if (visualize)
                printf("\n> MagicNumber is correct\n");
        } else exit(0);
    }

    fread(&totalNumConstsBinary, sizeof(totalNumConstsBinary), 1, fp);
    fread(&totalStringConstsBinary, sizeof(totalStringConstsBinary), 1, fp);
    fread(&totalNamedLibFuncsBinary, sizeof(totalNamedLibFuncsBinary), 1, fp);
    fread(&totalUserFuncsBinary, sizeof(totalUserFuncsBinary), 1, fp);
    fread(&codeTableSize, sizeof(codeTableSize), 1, fp);
    fread(&totalGlobals, sizeof(totalGlobals), 1, fp);

    double numConstsBinary[totalNumConstsBinary];
    char *stringConstsBinary[totalStringConstsBinary];
    char *namedLibFuncsBinary[totalNamedLibFuncsBinary];
    struct UserFunc userFuncsBinary[totalUserFuncsBinary];
    struct Instruction codeTable[codeTableSize];

    int currStringSize = 0;
    int offset = 0;
    int address = 0;
    int localSize = 0;
    int totalargs = 0;
    char *str = NULL;

    if (visualize)
        printf("\n> Table of arithmetic constants\n");

    for (int i = 0; i < totalNumConstsBinary; i++) {
        fread(&offset, sizeof(int), 1, fp);
        fread(&numConstsBinary[offset], sizeof(double), 1, fp);
        if (visualize) {
            printf("|");
            printf("%d", offset);
            printf("| %f\n", numConstsBinary[offset]);
        }
    }

    if (visualize)
        printf("\n> Table of string constants\n");


    for (int i = 0; i < totalStringConstsBinary; i++) {
        fread(&currStringSize, sizeof(int), 1, fp);
        str = calloc(currStringSize, sizeof(char));

        fread(&offset, sizeof(int), 1, fp);
        fread(str, sizeof(char) * currStringSize, 1, fp);

        stringConstsBinary[offset] = strdup(str);
        if (visualize) {
            printf("|");
            printf("%d", offset);
            printf("| %s\n", stringConstsBinary[offset]);
        }
        str = NULL;
    }

    if (visualize)
        printf("\n> Table of user functions\n");

    for (int i = 0; i < totalUserFuncsBinary; i++) {
        fread(&currStringSize, sizeof(int), 1, fp);
        str = calloc(currStringSize, sizeof(char));

        fread(&offset, sizeof(int), 1, fp);
        fread(&address, sizeof(int), 1, fp);
        fread(&localSize, sizeof(int), 1, fp);
        fread(&totalargs, sizeof(int), 1, fp);
        fread(str, sizeof(char) * currStringSize, 1, fp);

        userFuncsBinary[offset].id = (char *) strdup(str);
        userFuncsBinary[offset].address = address;
        userFuncsBinary[offset].localSize = localSize;
        userFuncsBinary[offset].totalargs = totalargs;

        if (visualize) {
            printf("|");
            printf("%d", offset);
            printf("| address %d, localSize %d, totalargs %d, id %s\n",
                   userFuncsBinary[offset].address, userFuncsBinary[offset].localSize,
                   userFuncsBinary[offset].totalargs, userFuncsBinary[offset].id);
        }
        str = NULL;
    }

    if (visualize)
        printf("\n> Table of library functions\n");

    for (int i = 0; i < totalNamedLibFuncsBinary; i++) {
        fread(&currStringSize, sizeof(int), 1, fp);
        str = calloc(currStringSize, sizeof(char));

        fread(&offset, sizeof(int), 1, fp);
        fread(str, sizeof(char) * currStringSize, 1, fp);

        namedLibFuncsBinary[offset] = strdup(str);
        if (visualize) {
            printf("|");
            printf("%d", offset);
            printf("| %s\n", namedLibFuncsBinary[offset]);
        }
        str = NULL;
    }


    if (visualize)
        printf("\n%-s%-16s%-30s%-30s%-s\n", "#|\t", "OP|", "RES|", "ARG1|", "ARG2|");
    if (visualize)
        printf("-----------------------------------------------------------------------------------------\n");

    struct InstructionToBinary instr;
    for (int i = 0; i < codeTableSize; i++) {
        fread(&instr, sizeof(instr), 1, fp);

        codeTable[i].opcode = instr.instrOpcode;

        codeTable[i].result.type = instr.resultType;
        codeTable[i].result.val = instr.resultOffset;

        codeTable[i].arg1.type = instr.arg1Type;
        codeTable[i].arg1.val = instr.arg1Offset;

        codeTable[i].arg2.type = instr.arg2Type;
        codeTable[i].arg2.val = instr.arg2Offset;

        codeTable[i].srcLine = instr.instrLine;

        if (visualize)
            printf("%-d%-s%-12s\t", instr.instrLine, ":\t", opcodeToString[instr.instrOpcode]);
        if (visualize)
            printf("%-3s%-17s%-3s%-17s%-3s%-2s\n",
                   typeToString(instr.resultType), offsetPrinter(instr.resultOffset),
                   typeToString(instr.arg1Type), offsetPrinter(instr.arg1Offset),
                   typeToString(instr.arg2Type), offsetPrinter(instr.arg2Offset));
    }

    fclose(fp);

    struct DataTables *tables;
    tables = malloc(sizeof(struct DataTables));
    memset(tables, 0, sizeof(struct DataTables));

    tables->numConstsTable = (double *) malloc(sizeof(double) * totalNumConstsBinary);
    memcpy(tables->numConstsTable, numConstsBinary, (sizeof(double) * totalNumConstsBinary));
    tables->totalNumConsts = totalNumConstsBinary;

    tables->strConstsTable = (char **) malloc(sizeof(char *) * totalStringConstsBinary);
    memcpy(tables->strConstsTable, stringConstsBinary, (sizeof(char *) * totalStringConstsBinary));
    tables->totalStrConsts = totalStringConstsBinary;

    tables->userFuncsTable = malloc(sizeof(struct UserFunc) * totalUserFuncsBinary);
    memcpy(tables->userFuncsTable, userFuncsBinary, (sizeof(struct UserFunc) * totalUserFuncsBinary));
    tables->totalUserFuncs = totalUserFuncsBinary;

    tables->libFuncsTable = (char **) malloc(sizeof(char *) * totalNamedLibFuncsBinary);
    memcpy(tables->libFuncsTable, namedLibFuncsBinary, (sizeof(char *) * totalNamedLibFuncsBinary));
    tables->totalLibFuncs = totalNamedLibFuncsBinary;

    tables->code = malloc(sizeof(struct Instruction) * codeTableSize);
    memcpy(tables->code, codeTable, sizeof(struct Instruction) * codeTableSize);
    tables->codeSize = codeTableSize;

    tables->totalGlobals = totalGlobals;

    return tables;
}


