//
//  main.c
//  HeapManager
//
//  Created by Brandon Tran on 3/11/20.
//  Copyright © 2020 Brandon Tran. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>

// -- Objective ----------------------------------------------------------------------------
//
// In this lab, you will implement your own dynamic memory allocator (heap manager) in C.
// We will use the simple (but somewhat inefficient) free list system of heap allocation.
// You will demonstrate how to use your allocator to allocate dynamic memory for a variety
// of C types.
//
// -----------------------------------------------------------------------------------------

// -- Step 1 -------------------------------------------------------------------------------
//
// Define a struct to represent an allocation block:
//
// -----------------------------------------------------------------------------------------


struct Block
{
    int block_size; // # of bytes in the data section
    struct Block *next_block; // pointer to the next block
    
};
// The data section follows in memory after the above struct

// -----------------------------------------------------------------------------------------

// -- Step 2 -------------------------------------------------------------------------------
//
// Determine the size of a Block value using sizeof(struct Block) and assign it to a global
// const variable. We'll refer to this value as the “overhead size”.
//
// -----------------------------------------------------------------------------------------

const int overhead_size = sizeof(struct Block);

// -----------------------------------------------------------------------------------------

// -- Step 3 -------------------------------------------------------------------------------
//
// Determine the size of a void* and save it in another const global.
//
// -----------------------------------------------------------------------------------------

const int void_size = sizeof(void*);

// -----------------------------------------------------------------------------------------

// -- Step 4 -------------------------------------------------------------------------------
//
// Create a global pointer struct Block *free_head, which will always point to the first
// free block in the free list.
//
// -----------------------------------------------------------------------------------------

struct Block *free_head;
struct Block *head;

// -----------------------------------------------------------------------------------------

// -- Step 5 -------------------------------------------------------------------------------
//
// Create a function void my_initialize_heap(int size), which uses malloc to initialize a
// buffer of the given size to use in your custom allocator. (This is the only time you can
// use malloc in the entire program.) Your global free_head should point to this buffer,
// and you should initialize the head with appropriate values for block_size and next_block.
//
// -----------------------------------------------------------------------------------------

void my_initialize_heap(int size)
{
    free_head = (struct Block*) malloc(size);
    free_head->block_size = size - overhead_size;
    free_head->next_block = NULL;
    
} // end of "my_initialize_heap" function

// -----------------------------------------------------------------------------------------

// -- Step 6 -------------------------------------------------------------------------------
// Create a function void* my_malloc(int size), which fills an allocation request of size
// bytes and returns a pointer to the data portion of the block used to satisfy the request.
//
// (a) Walk the free list starting at free_head, looking for a block with a large enough
//     size to fit the request. If no blocks can be found, returns 0 (null). Use the first
//     fit heuristic.
// (b) size can be any positive integer value, but any block you use must have a data size
//     that is a multiple of your void* size. So if a void* is 4 bytes, and the function is
//     told to allocate a 2 byte block, you would actually find a block with 4 bytes of data
//     and use that, with 2 bytes being fragmentation.
// (c) Once you have found a block to fit the data size, decide whether you need to split
//     that block.
//     i. If you do, then find the byte location of where the new block will start, based on
//        the size of the block you are splitting and the size of the allocation request.
//        Initialize a new block at that location by assigning its block_size and setting
//        its next_block pointer to null. Reduce the size of the original block
//        appropriately.
//     ii. If you cannot split the block, then you need to redirect pointers to the block to
//         point to the block that follows it, as if you are removing a node from a singly
//         linkedlist. WARNING: The logic for a node in a linked list is different depending
//         on whether or not the node is the head of the list. Draw it out and convince
//         yourself why you need to account for this.
//     iii. A block needs to be split if its data portion is large enough to fit the size
//          being allocated, AND the excess space in the data portion is sufficient to fit
//          another block with overhead and a minimum block size of sizeof(void*).
// (d) Return a pointer to the data region of the block, which is “overhead size” bytes past
//     the start of the block. Use pointer arithmetic.
//
// -----------------------------------------------------------------------------------------

int multipleOf(int size)
{
    int remainder = abs(size) % void_size;
    
    if(remainder == 0)
    {
        return size;
    }
    
    return size + void_size - remainder;
}

void removeblock_front()
{
    
    // Create a temporary block to point to the free head
    struct Block *temporary_block = free_head;

    // Point the free head to the next block of the current free head.
    free_head =  free_head->next_block;

    // Update the next block of the temporary block to NULL
    temporary_block->next_block = NULL;
}

void removeblock_back()
{
    // Create a temporary block to point to the free head
    struct Block *temporary_block = free_head;
    
    // Create a temporary block to check for the tail
    struct Block *temporary_block_back = NULL;
    
    // Traverse through the free list until you found the tail
    while(temporary_block->next_block != NULL)
    {
        temporary_block_back = temporary_block;
        temporary_block = temporary_block->next_block;
    }
    
    // If we are able to traverse through the free list, back will not equal null.
    if(temporary_block_back != NULL)
    {
        temporary_block_back->next_block = NULL;
    }
        
}

// Remove a block from the free list which varies whether it is in the front, middle or back.
void removeblock(struct Block* current_block){
    
        // Check if the current block is the head of the free list
        if(current_block == free_head)
        {
            removeblock_front();
        }

        // Check if the current block is the tail of the free list
        if(current_block->next_block == NULL)
        {
            removeblock_back();
        }
        
        // Check if the current block is the middle of the free list
        struct Block* block_marker = free_head;
        while(block_marker != NULL)
        {
            if(block_marker->next_block == current_block)
            {
                break;
            }
            block_marker = block_marker->next_block;
        }
    
        if(block_marker != NULL)
        {
            struct Block* temporary_block = block_marker->next_block;
            block_marker->next_block = temporary_block->next_block;
            temporary_block->next_block = NULL;
        }
}
void* my_malloc(int size)
{
    
    // Part B
    size = multipleOf(size);
    
    // Part A
    struct Block *current_block = free_head;
    
    int foundBlock = 0;
    while(current_block != NULL)
    {
        if (size <= current_block->block_size)
        {
            foundBlock = 1;
            break;
        }
        else
        {
            current_block = current_block->next_block;
        }
    }
    
    // Part C
    // if foundBlock = 0, no blocks can be found
    // else foundBlock = 1, blocks can be found
    if(foundBlock == 0)
    {
        return 0;
    }
    
    else
    {
        
        // Found Block is bound enough to fit the data size being allocated
        if( (current_block->block_size - size - overhead_size - void_size) > 0 )
        {

            // Deciding whether or not to split
            // After the split, calculate the remaining size
            int remaining = current_block->block_size - size;

            // Declare a temporary block address for a new block that is equal to:
            // current_block + overhead_size + size
            struct Block* temporary_block = (void*) current_block + overhead_size + size;
            
            // Set the next block equal to the current blocks next block
            // Update the next block of the temporary block to the current block next block
            temporary_block->next_block = current_block->next_block;

            // Update the block size of the temporary block to:
            // remaining - overhead_size
            temporary_block->block_size = remaining - overhead_size;
            free_head = temporary_block;

            // Update the blocksize of the current block to the size
            current_block->block_size = size;

            // The nextblock of the current block is not pointing to anything. Not part
            // of the free_list
            current_block->next_block = NULL;
            
        }
        else
        {
            // Remove the block
            removeblock(current_block);

        }
        // Return the address of the new block as well as add the bytes of the overhead_size
        // to get the data portion of the block.
        return (void*) current_block + overhead_size;
    }

} // end of "my_malloc" function

// -----------------------------------------------------------------------------------------

// -- Step 7 -------------------------------------------------------------------------------
//
// Create a function void my_free(void *data), which deallocates a value that was allocated
// on the data heap. The pointer will be to the data portion of a block; move backwards in
// memory to find the block's overhead information, and then link it into the free list.
//
// -----------------------------------------------------------------------------------------

void my_free(void *data)
{
    // Point the next block of the temporary block to the free head.
    struct Block* temporary_block = (void*) data - overhead_size;
    temporary_block->next_block = free_head;

    // Update free head to the temporary block
    free_head = temporary_block;
    
} // end of "my_free" function

// -----------------------------------------------------------------------------------------

// -- Main ---------------------------------------------------------------------------------
// Test your code thoroughly by allocating values of various types. You should write (and
// turn in) your own testing main, which at least includes the following tests:
//      1. Allocate an int; print the address of the returned pointer. Free the int, then
//         allocate another int and print its address. The addresses should be the same.
//      2. Allocate two individual int values and print their addresses; they should be
//         exactly the size of your overhead plus the size of a void* apart.
//      3. Allocate three int values and print their addresses, then free the second of the
//         three. Allocate a double and print its address; verify that the address is the
//         same as the int that you freed.
//      4. Allocate one char, then allocate one int, and print their addresses. They should
//         be exactly the same distance apart as in test #2.
//      5. Allocate space for a 100-element int array, then for one more int value. Verify
//         that the address of the int value is 100 * sizeof(int) + the size of your header
//         after the array's address. Free the array. Verify that the int's address and
//         value has not changed.
//      6. Write a program to use your tested allocator to solve a classic problem requiring
//         dynamic storage:
//          1. Ask the user to enter a positive integer n.
//          2. Allocate space for an array of n integers.
//          3. Read n integers from standard in, into the array you allocated.
//          4. Calculate and print the standard deviation of the integers entered,
//             using the formula
// -----------------------------------------------------------------------------------------

int main()
{
    my_initialize_heap(1000);
    void* ptr1; void* ptr2; void* ptr3; void* ptr4; void* ptr5;

    int choice = 0;
    printf("\nTest Case (1-5) or Compute Standard Deviation (6): ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
             printf("Test Case #1\n");
             ptr1 = my_malloc(sizeof(int));
             printf("ptr1 address: %p\n", ptr1);
             my_free(ptr1);
             ptr2 = my_malloc(sizeof(int));
             printf("ptr2 address: %p\n", ptr2);
             break;
        case 2:
             printf("Test Case #2\n");
             ptr1 = my_malloc(sizeof(int));
             ptr2 = my_malloc(sizeof(int));
             printf("ptr1 address: %p\n", ptr1);
             printf("ptr2 address: %p\n", ptr2);
             break;
        case 3:
             printf("Test Case #3\n");
             ptr1 = my_malloc(sizeof(int));
             ptr2 = my_malloc(sizeof(int));
             ptr3 = my_malloc(sizeof(int));
             printf("ptr1 address: %p\n", ptr1);
             printf("ptr2 address: %p\n", ptr2);
             printf("ptr3 address: %p\n", ptr3);
             my_free(ptr2);
             ptr4 = my_malloc(sizeof(double));
             printf("ptr4 address: %p\n", ptr4);
             ptr5 = my_malloc(sizeof(int));
             printf("ptr5 address: %p\n", ptr5);
             break;
        case 4:
             printf("Test Case #4\n");
             ptr1 = my_malloc(sizeof(char));
             ptr2 = my_malloc(sizeof(int));
             printf("ptr1 address: %p\n", ptr1);
             printf("ptr2 address: %p\n", ptr2);
             break;
        case 5:
             printf("Test Case #5\n");
             ptr1 = my_malloc(sizeof(int[100]));
             ptr2 = my_malloc(sizeof(int));
             printf("ptr1 address: %p\n", ptr1);
             printf("ptr2 address: %p\n", ptr2);
             my_free(ptr1);
             printf("ptr2 address: %p\n", ptr2);
             break;
        case 6:
            printf("\nStandard Deviation\n");
           
            // 1. Ask the user to enter a positive integer n.
            int n;
            printf("Enter a positive integer n: ");
            scanf("%d", &n);
            
            // 2. Allocate space for an array of n integers.
            int* array;
            array = (int*)my_malloc(n* sizeof(int));
            
            // 3. Read n integers from standard in, into the array you allocated.
            int num;
            int sum = 0;
            for(int index = 0; index < n; index++)
            {
                printf("Insert an integer into element %d of the array: ", index);
                scanf("%d", &num);
                array[index] = num;
                sum = sum + num;
            }
            // 4. Calculate and print the standard deviation of the integers entered, using
            //    the formula (given on assignment page) where xi are the integers entered,
            //    and μ is the arithmetic mean of those numbers.
            double standardDeviation;
            double average;
            double sigma = 0;
            
            // Average = sum/n -> Double Type Casting
            average = ((double)sum)/((double) n);
            
            for(int index = 0; index < n; index++)
            {
                // Sigma = (array[index] - average)^2 -> Double Type Casting
                sigma = sigma + pow(((double) array[index]) - average,2);
            }
            
            // Standard Deviation = (sigma/n)^(1/2) -> Double Type Casting
            standardDeviation = sqrt(sigma/((double) n));
            
            printf("Standard deviation %f  ", standardDeviation);
            printf("\n");
            break;
         default:
             printf("\n");
     }
     free(head);
     return 0;
}

// -----------------------------------------------------------------------------------------

