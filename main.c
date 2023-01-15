#include "wasm/wasm.h"

int
TestFunction(int A, int B)
{
    Print("__heap_base: %d, HeapMax: %u", (int)&__heap_base, HeapMax);

    int MoreBytes = 200000;
    void *Ptr = malloc(MoreBytes);

    Print("__heap_base: %d, HeapMax: %u", (int)&__heap_base, HeapMax);
    
    int Result = A + B;
    return(Result);
}
