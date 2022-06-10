#include "tools/tools.h"

struct AvmMemCell amc1;
struct AvmMemCell amc2;
struct AvmMemCell returnValue;
unsigned int top;
unsigned int top_sp; // Not initialized

unsigned char running;
unsigned int pc;

struct BinaryData* binaryData;

unsigned int totalActuals; // Not initialized
struct AvmMemCell stack[AVM_STACK_SIZE];

int main(int argc, char **argv) {
    char *name = argv[1];

    binaryData = binaryDecode(name);
    running = 1;
    pc = 0;
    top = AVM_STACK_SIZE - 1 - binaryData->totalGlobals;

    initialize();

    while(running)
        startCycle();
}