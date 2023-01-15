#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

extern unsigned char __heap_base;
extern unsigned char __data_end;

extern void printw(int Ptr, int Bytes);
extern int GetMaxMemory(void);
extern int GrowMemory(int RequestedBytes);

void *
memset(void *Ptr, int Value, unsigned long Size)
{
    char *Base = Ptr;
    for(int ByteIndex = 0;
	ByteIndex < Size;
	ByteIndex++)
    {
	Base[ByteIndex] = (char)Value;
    }
    return(Ptr);
}

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

#define Print(String, ...)						\
{									\
    char Buffer[256];							\
    int Length = stbsp_snprintf(Buffer, 256, String, ##__VA_ARGS__);	\
    printw((int)Buffer, Length);					\
}

unsigned int HeapBase = (unsigned int)&__heap_base;
unsigned int HeapUsed = 0;
unsigned int HeapMax  = 0;

void
InitMemory(int MaxSize)
{
    HeapMax = MaxSize;
}

void *
malloc(unsigned long Size)
{
    void *Result = 0;

    Print("malloc: Bytes requested: %lu, Bytes available: %d", HeapUsed + Size, HeapMax - HeapUsed);
    
    if((HeapUsed + Size) >= HeapMax)
    {
	int NeededBytes = (HeapUsed + Size) - HeapMax;
	int NewMax = GrowMemory(NeededBytes);

	if(NewMax == HeapMax)
	{
	    Print("malloc: Memory Allocation Failed");
	    return(Result);
	}
	else
	{
	    Print("malloc: Memory grown to %d", NewMax);
	    HeapMax = NewMax;
	}
    }
    
    Result = (void *)HeapBase + HeapUsed;
    HeapUsed += Size;

    return(Result);
}
