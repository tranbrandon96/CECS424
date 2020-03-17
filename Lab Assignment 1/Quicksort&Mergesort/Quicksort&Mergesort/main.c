//
//  main.c
//  Quicksort&Mergesort
//
//  Created by Brandon Tran on 2/5/20.
//  Copyright © 2020 Brandon Tran. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// -- Quicksort Implementation -----------------------------------------------------------------------------------------
// QuickSort
// Parameters:
//          int *a -> Pointer to the array from main.
//          int n -> Length of the array.
//
// ---------------------------------------------------------------------------------------------------------------------

void *qsort2(int *a, int n)
{
  // Base Case
  if (n <= 1) // If the array only contains 1 value,
    return a; // Return the array because it is already sorted.

  int *curr = a + 1; // Dealing with the current array

  int *left = malloc(sizeof(a) - 1); // Allocate a single large block of memory for left side of the array.
  int *right = malloc(sizeof(a) - 1); // Allocate a single large block of memory for right side of the array

  int iCount = 0; // i markers for the for loops
  int jCount = 0; // j markers for the for loops

  int pivotValue = a[0]; // Assign the pivot value from the first part of the array.

  // Create a for loop to check each value of the array
  int index;
  for (index = 0; index < n - 1; index++)
  {
      if (curr[index] <= pivotValue) // If the current index value is greater than the pivot value
      {
          left[iCount++] = curr[index]; // The current value will be assigned to the left when its incremented in its place.
      }
      else // Otherwise
      {
          right[jCount++] = curr[index]; // The current value will be assigned to the right when its incremented in its place.
      }
  }

  left = qsort2(left, iCount); // Recursively call the quicksort function for i
  right = qsort2(right, jCount); // Recursively call the quicksort function for j

  // Traverse through the entire array to set up the correct sorted array
  for (index = 0; index < iCount; index++)
  {
    a[index] = left[index];
  }
    
  a[index++] = pivotValue;

  // Traverse through the entire array to set up the correct sorted array
  int jIndex;
  for (jIndex = 0; jIndex < jCount; jIndex++)
  {
    a[jIndex+index] = right[jIndex];
  }
    
  free(left); // Deallocates the memory
  free(right); // Deallocates the memory
  return a; // Return sorted array
}

// -- Mergesort Implementation -----------------------------------------------------------------------------------------
// MergeSort
// Parameters:
//          int *a -> Pointer to the array from main.
//          int n -> Length of the array.
// Merge
// Parameters:
//          int *a -> Pointer to the array from main.
//          int sizeOfArray -> Size of the array
//          int middleIndex -> Middle Index of the current array
//
// ---------------------------------------------------------------------------------------------------------------------

void merge(int *a, int sizeOfArray, int middleIndex)
{
    // Use these variables as iterators for traversal of the array.
    int iMarker;
    int jMarker;
    int kMarker;
    
    int *curr = malloc(sizeOfArray * sizeof(int)); // Allocate a single large block of memory
    
    // Traverse through both the array passed on by the msort function as well as the current array
    for(iMarker = 0, jMarker = middleIndex, kMarker = 0; kMarker < sizeOfArray; kMarker++)
    {
        // Condition Statements to deal with separate arrays in regards to organizing the array.
        if(jMarker == sizeOfArray)
        {
            curr[kMarker] = a[iMarker++];
        }
        else if (iMarker == middleIndex)
        {
            curr[kMarker] = a[jMarker++];
        }
        
        else if (a[jMarker] < a[iMarker])
        {
            curr[kMarker] = a[jMarker++];
        }
        else
        {
            curr[kMarker] = a[iMarker++];
        }
    }
    
    // Traverse through the entire array for current.
    for(iMarker = 0; iMarker < sizeOfArray; iMarker++)
    {
        a[iMarker] = curr[iMarker]; // Array returned back to the main will be replaced with the values that are currently in the array for this function.
    }
    
    free(curr); // Deallocates the memory
    
}

void msort(int *a, int n)
{
    // Base Case
    if (n <= 1) // If the array only contains 1 value,
        return; // Return the array because it is already sorted.
    
    int middleIndex = n / 2; // Determine the middle index of the array
    
    msort(a, middleIndex); // Recursively calls the msort implementation to deal with the left side of the array.
    msort(a + middleIndex, n - middleIndex); // Recursively calls the msort implementation to deal with the right side of the array.
    merge(a, n, middleIndex); // Comebine the left and right side array together to form one array.
}

// -- Print Array ------------------------------------------------------------------------------------------------------
// Print Array
// Parameters:
//          int *a -> Pointer to the array from main.
//          int n -> Length of the array.
//
// ---------------------------------------------------------------------------------------------------------------------
void printArray(int *a, int n)
{
    int i;
    // Traverse through entire array
    for (i = 0; i < n; i++)
    {
      printf("%i ", (int)a[i]); // Print each index one by one.
    }
    printf("\n");
}
// Instructions: Create a Mergesort and Quicksort implementation to test if the sort works
//               as well as understand how the implementation works.
int main()
{
  int n = 10; // Size of the array
 
  // Initilize the array with the given values from the assignment instructions
  int *a1 = malloc(n);
  a1[0] = 4;
  a1[1] = 65;
  a1[2] = 2;
  a1[3] = -31;
  a1[4] = 0;
  a1[5] = 99;
  a1[6] = 2;
  a1[7] = 83;
  a1[8] = 782;
  a1[9] = 1;

  printf("Unsorted Array using QuickSort\n");
  printArray(a1, n); // Print Unsorted Array using Quicksort
    
  qsort2(a1, n); // Call QuickSort Algorithm

  printf("\nSorted Array using QuickSort\n");
  printArray(a1, n); // Print Sorted Array using Quicksort
    
  // Initialize the array with the given values of the assignment instructions
  int *a2 = malloc(n);
  a2[0] = 4;
  a2[1] = 65;
  a2[2] = 2;
  a2[3] = -31;
  a2[4] = 0;
  a2[5] = 99;
  a2[6] = 2;
  a2[7] = 83;
  a2[8] = 782;
  a2[9] = 1;
    
  printf("\nUnsorted Array using MergeSort\n");
  printArray(a2, n); // Print Unsorted Array using MergeSort
    
  msort(a2, n); // Call MergeSort Algorithm
    
  printf("\nSorted Array using MergeSort\n");
  printArray(a2, n); // Print Sorted Array using Mergesort
  return 0;
}

