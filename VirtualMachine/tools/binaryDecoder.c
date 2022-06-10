#include "tools.h"

struct BinaryData *binaryDecode(char *name) {
    unsigned int magicNumberBinary = 0;

    FILE *fp = fopen(name, "rb");
    if (fp == NULL) {
        printf("ERROR: cannot open file %s\n", name);
        exit(0);
    }

    if (fread(&magicNumberBinary, sizeof(magicNumberBinary), 1, fp) == 1) {
        if (magicNumberBinary != 19955991)
            exit(0);
    }

    // Read counters
    unsigned int totalNumConstsBinary = 0;
    unsigned int totalStringConstsBinary = 0;
    unsigned int totalNamedLibFuncsBinary = 0;
    unsigned int totalUserFuncsBinary = 0;
    unsigned int codeTableSize = 0;
    unsigned int totalGlobals = 0;
    fread(&totalNumConstsBinary, sizeof(totalNumConstsBinary), 1, fp);
    fread(&totalStringConstsBinary, sizeof(totalStringConstsBinary), 1, fp);
    fread(&totalNamedLibFuncsBinary, sizeof(totalNamedLibFuncsBinary), 1, fp);
    fread(&totalUserFuncsBinary, sizeof(totalUserFuncsBinary), 1, fp);
    fread(&codeTableSize, sizeof(codeTableSize), 1, fp);
    fread(&totalGlobals, sizeof(totalGlobals), 1, fp);

    // Init arrays
    double numConstsBinary[totalNumConstsBinary];
    char *stringConstsBinary[totalStringConstsBinary];
    char *namedLibFuncsBinary[totalNamedLibFuncsBinary];
    struct UserFunc userFuncsBinary[totalUserFuncsBinary];
    struct Instruction codeTable[codeTableSize];

    int currentStringSize = 0;
    int offset = 0;
    int address = 0;
    int localSize = 0;
    int totalArgs = 0;
    char *tmpString = NULL;

    for (int i = 0; i < totalNumConstsBinary; i++) {
        fread(&offset, sizeof(int), 1, fp);
        fread(&numConstsBinary[offset], sizeof(double), 1, fp);
    }

    for (int i = 0; i < totalStringConstsBinary; i++) {
        fread(&currentStringSize, sizeof(int), 1, fp);
        tmpString = calloc(currentStringSize, sizeof(char));

        fread(&offset, sizeof(int), 1, fp);
        fread(tmpString, sizeof(char) * currentStringSize, 1, fp);

        stringConstsBinary[offset] = strdup(tmpString);
        tmpString = NULL;
    }

    for (int i = 0; i < totalUserFuncsBinary; i++) {
        fread(&currentStringSize, sizeof(int), 1, fp);
        tmpString = calloc(currentStringSize, sizeof(char));

        fread(&offset, sizeof(int), 1, fp);
        fread(&address, sizeof(int), 1, fp);
        fread(&localSize, sizeof(int), 1, fp);
        fread(&totalArgs, sizeof(int), 1, fp);
        fread(tmpString, sizeof(char) * currentStringSize, 1, fp);

        userFuncsBinary[offset].id = (char *) strdup(tmpString);
        userFuncsBinary[offset].address = address;
        userFuncsBinary[offset].localSize = localSize;
        userFuncsBinary[offset].totalArgs = totalArgs;

        tmpString = NULL;
    }

    for (int i = 0; i < totalNamedLibFuncsBinary; i++) {
        fread(&currentStringSize, sizeof(int), 1, fp);
        tmpString = calloc(currentStringSize, sizeof(char));

        fread(&offset, sizeof(int), 1, fp);
        fread(tmpString, sizeof(char) * currentStringSize, 1, fp);

        namedLibFuncsBinary[offset] = strdup(tmpString);
        tmpString = NULL;
    }


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
    }

    fclose(fp);

    struct BinaryData *tables;
    tables = malloc(sizeof(struct BinaryData));
    memset(tables, 0, sizeof(struct BinaryData));

    tables->numConstsTable = numConstsBinary;
    tables->totalNumConsts = totalNumConstsBinary;

    tables->strConstsTable = stringConstsBinary;
    tables->totalStrConsts = totalStringConstsBinary;

    tables->userFuncsTable = userFuncsBinary;
    tables->totalUserFuncs = totalUserFuncsBinary;

    tables->libFuncsTable = namedLibFuncsBinary;
    tables->totalLibFuncs = totalNamedLibFuncsBinary;

    tables->code = codeTable;
    tables->codeSize = codeTableSize;

    tables->totalGlobals = totalGlobals;

    return tables;
}


