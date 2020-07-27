#include <stdio.h>
#include <stdbool.h>
// up to 100 are used for mamaging the programe. therefore only 24900 is avaliable as memory.
// #define currentBlock memory
// #define preBlock (memory+8)
// #define nextblock (memory+16)
// #define blockSize *(int *)(memory+24)
// #define flagsearch *(int *)(memory+28)
// #define size *(int *)(memory+32)
// #define temp (memory+36)
// #define freeflag (memory+44)
// #define allocatedflag (memory+45)
// #define address (memory+46)
// char *currentBlock=memory;
//  freeflag = 'f';
//  allocatedflag = 'a';
char memory[25000] = {'\0'};
#define base (memory + 100)
char *currentBlock = base;
char freeflag = 'f';
char allocatedflag = 'a';

char *findBlock(size_t size)
{

    char *preBlock;
    preBlock = currentBlock;

    int blockSize = 0;

    int flagsearch = 0;

    while ((currentBlock + 5 <= preBlock) || (flagsearch != 1))
    {
        blockSize = *(int *)(currentBlock + 1);
        if (*currentBlock == freeflag && blockSize >= size + 5)
        {
            return (currentBlock);
        }
        else if (!currentBlock)
        {
            return NULL;
        }
        else
        {
            if (currentBlock + blockSize + size >= base + 24899 && flagsearch == 1)
            {
                printf("Memory Overflow\n");
                currentBlock = base;
                return NULL;
            }

            if (currentBlock + blockSize + size >= base + 24899)
            {
                currentBlock = base;
                flagsearch = 1;
            }
            currentBlock = currentBlock + blockSize + 1;
        }
    }
}

void splitBlock(char *block, size_t size)
{
    char *temp;
    int blockSize = *(int *)(block + 1);

    blockSize = blockSize - size - 5;

    temp = block + size + 5;

    *temp = freeflag;

    *(int *)(temp + 1) = blockSize;
    *(int *)(block + 1) = size + 5;
    *block = allocatedflag;
    currentBlock = temp;
}

char *MyMalloc(size_t size)
{
    if (size <= 0)
    {
        return NULL;
    }

    if (!*base)
    {
        *base = freeflag;
        *(int *)(base + 1) = 24999 - 5;
    }

    char *block = findBlock(size);

    if (block)
    {
        if (*(int *)(block + 1) >= size)
        {
            splitBlock(block, size);
        }
    }
    else
    {
        return NULL;
    }

    return (block);
}

void MyFree(char *address)
{
    char *freemover, *preblock, *nextblock;

    freemover = base;

    int blockSize = *(int *)(freemover + 1);

    while (true)
    {
        blockSize = *(int *)(freemover + 1);

        if (freemover == address)
        {
            preblock = freemover;
            nextblock = freemover;

            break;
        }
        else if (!*freemover)
        {
            break;
        }
        else
        {
            preblock = freemover;
            freemover = freemover + blockSize;
            nextblock = freemover + *(int *)(freemover + 1);
        }
    }

    if (*nextblock == freeflag)
    {
        *freemover = freeflag;
        *(int *)(freemover + 1) = *(int *)(freemover + 1) + *(int *)(nextblock + 1);
    }

    if (*preblock == freeflag)
    {
        *freemover = freeflag;
        *(int *)(preblock + 1) = *(int *)(freemover + 1) + *(int *)(preblock + 1);
    }
    else
    {
        *address = freeflag;
    }
}
