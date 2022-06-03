#include "tools/tools.h"

struct AvmMemCell ax, bx;
struct AvmMemCell retval;
unsigned int top;
unsigned int topsp; // Not initialized

unsigned char executionFinished;
unsigned int pc;

struct DataTables* tables;

unsigned int totalActuals; // Not initialized
struct AvmMemCell stack[AVM_STACKSIZE];

int main(int argc, char **argv) {
    char *name = argv[1];

    // You can see in a visualized way all the data stored in the executable when the VM is running it.
    int visualize = 0;

    if(argc > 2 && !strcmp(argv[2], "--visualize"))
        visualize = 1;

    tables = decode(visualize, name);

    executionFinished = 0;
    pc = 0;
    top = AVM_STACKSIZE - 1 - tables->totalGlobals;

    avm_initialize();

    while(executionFinished == 0) {
//         printf("Executing instruction %d\n", pc);
        execute_cycle();
    }
}