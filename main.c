#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HEAP_SIZE 10000

// structure to store the metadata of memory blocks
typedef struct BlockInfo
{
    struct BlockInfo *next;
    int size;
    int allocated; // 1 if allocated and 0 if free
} BlockInfo;

// heap of 10000 bytes of memory
char heapMemory[10000];
void *heapStart = (void *)heapMemory;
void *heapEnd = (void *)(heapMemory + HEAP_SIZE);

// freeblock having starting address of heap memory
BlockInfo *freeBlocks = (BlockInfo *)heapMemory;

// merging adjacent blocks to avoid fragmentation
void mergeBlocks(BlockInfo *block, BlockInfo *previous)
{
    // Try right merge
    if (block->next && (void *)block + block->size + sizeof(BlockInfo) == (void *)block->next)
    {
        block->size += block->next->size + sizeof(BlockInfo);
        block->next = block->next->next;
    }

    // Try left merge
    if (previous && (void *)previous + previous->size + sizeof(BlockInfo) == (void *)block)
    {
        previous->size += block->size + sizeof(BlockInfo);
        previous->next = block->next;
    }
}

void deallocateMemory(void *ptr)
{


    BlockInfo *block = (BlockInfo *)ptr;
    if(!(block->allocated))
    {
        printf("\nError : trying to double free. Block is already free.");
        return;
    }
    block->allocated = 0;

    BlockInfo *temp = freeBlocks, *previous = NULL;

    // Insert block in sorted order into freelist
    while (temp != NULL && ptr > (void *)temp)
    {
        previous = temp;
        temp = temp->next;
    }

    block->next = temp;

    if (previous == NULL)
    {
        // Insert at beginning
        freeBlocks = block;
    }
    else
    {
        previous->next = block;
    }

    // Merge adjacent blocks if possible
    mergeBlocks(block, previous);

    printf("\nMemory deallocated.");
}

// allocating memory in heap
void *allocateMemory(int blockSize)
{
    BlockInfo *temp = freeBlocks, *previous = NULL;

    while (temp != NULL && temp->size < (blockSize + sizeof(BlockInfo)))
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("No sufficient memory available.");
        return NULL;
    }

    BlockInfo *ptr = temp;

    // Create a new free block after allocating 'blockSize' from current block
    BlockInfo *newFree = (BlockInfo *)((void *)temp + sizeof(BlockInfo) + blockSize);
    newFree->size = temp->size - sizeof(BlockInfo) - blockSize;
    newFree->next = temp->next;

    if (previous == NULL)
        freeBlocks = newFree; // we're allocating from the head
    else
        previous->next = newFree; //maintain the link to the next free block

    ptr->size = blockSize;
    ptr->allocated = 1;
    ptr->next = NULL;

    return (void *)ptr;
}

// printing the current status of heap
void printHeapStatus()
{
    printf("------------------------------------------------\n");
    BlockInfo *temp = (BlockInfo *)heapMemory;
    char status[10];
    while ((void *)temp < (void *)(heapMemory + 10000))
    {
        if (temp->allocated)
        {
            strcpy(status, "Allocated");
        }
        else
        {
            strcpy(status, "Free");
        }
        printf("Block Address: %p | Block Size: %d bytes | Status: %s\n",
               temp, temp->size, status);
        temp = (BlockInfo *)((void *)temp + temp->size + sizeof(BlockInfo));
    }
    printf("------------------------------------------------\n");
}

int main()
{
    freeBlocks->next = NULL;
    freeBlocks->size = HEAP_SIZE - sizeof(BlockInfo);
    freeBlocks->allocated = 0;

    int choice = 0, size;
    void *ptr;

    while (choice != 4)
    {
        printf("\nMenu:\n");
        printf("1. Allocate Memory\n");
        printf("2. Deallocate Memory\n");
        printf("3. Print Heap Status\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice)
        {
        case 1:
            printf("Enter size to allocate: ");
            if (scanf("%d", &size) != 1 || size <= 0)
            {
                printf("Invalid size.\n");
                while (getchar() != '\n');
                break;
            }
            ptr = allocateMemory(size);
            if (ptr)
                printf("Memory allocated at address: %p\n", ptr);
            else
                printf("Memory allocation failed.\n");
            break;

        case 2:
            printf("Enter address to deallocate (in hex, e.g., 0x1234): ");
            if (scanf("%p", &ptr) != 1)
            {
                printf("Invalid address format.\n");
                while (getchar() != '\n');
                break;
            }

            // Validate address is inside heap range
            if (ptr == NULL || ptr < heapStart || ptr >= heapEnd)
            {
                printf("Error: Invalid or out-of-bounds address.\n");
                break;
            }

            deallocateMemory(ptr);
            break;

        case 3:
            printHeapStatus();
            break;

        case 4:
            printf("Exiting...\n");
            exit(0);

        default:
            printf("Invalid choice, please try again.\n");
        }
    }

    return 0;
}
