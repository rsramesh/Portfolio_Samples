/*************************************************************************************************************************** 
**  Program to Read Packed 12 bit values from Input File and 
**  generate output file
**  a) File Containing
**  #32 Most Recent Values
**  #32 Largest Values
**  
**  Program Reads in 3 bytes at a time, so that we can parse 2 12-bit values (2*12 =24bits = 3 bytes).Appropriate handling 
**  is provided for EOF and when we have less than 3 bytes remaining to be read. Once 12-Bit values are passed, we store 
**  the most recent 32 values in a circular buffer array, and to capture the largest 32 bit values, we use a Min Heap, 
**  each time a value is obtained, it is comapred against the root of Min Heap, if its larger then we replace the root 
**  with the value and heapify it, so that Min value in the Min-heap now again resides at root. 
**  
**  Once we have read all values, we dump the Most recent 32 values and Largest 32 values to Output File 
**  
**  Complex Datastructures used : Min Heap to store largest 32 values 
**  					 Complexity(Min-Heap) : Insert - O(log(n))
**  										  : Remove - O(Log(n))
**  										  : Build Heap - O(n)
**  Sorting Used :        Merge Sort          : Complexity : 0(nlogn) 
**  
**  Bit Operations are used to Optimize the Extraction of values, and Optimal Sorting Technique(Merge Sort) and 
**  Optimal Datastructure(Min-Heap) for easy and Fast Data Manipulation is used. All Debug prints removed for reducing Latency 
**  in Code Execution. Merge Sort offers better Time Complexity versus any other sort like Quick(n^2), Insertion(n^2) etc. but its
**  also requires O(n) space, tradeoff was made for speed versus space. Min-Heap is the easiet way to store the largest 32 values
**  as once heap is built,it only requires one insert at root and re-heapification which reduces the time complexity of the process.
**  
**  Bit Extraction Algorithm:  Read 3 bytes at a time, which contains 24bits into a char array, parse the first set of 12bit
**  and 2nd set of 12bits and store in Array, which is used to store the last 32 values, while simultaneously inserting into
**  Min Heap Structure.
**  
**  Min Heap offers good time complexity, as once heap is built min value is present at top(root)of heap, we compare new values
**  against the root, and if its greater we insert into root and re-heapify, which brings the smallest element in heap to
**  the root of heap.
**  
**  Modularized program for Readbility and Re-usability. Used Defines to reduce usage of Magic Numbers in Code for readability.
**  
**  Once all values in file are read, we sort the Min-Heap Array and write outputs to File.
**  To print values to output File, we use fprintf function, with appropriate format specifiers for Integers/Char
**  
**  If Number of Bytes in Input File is not a multiple of 3, we get the final number of bytes using fread().
**  If Final number of bytes read is 1, we assume that this is remaining value, by appending upper Nibble from bits
**  8-11 are all 0's.
**  If Final number of bytes read is 2, we assume that these are remaining values. First value is obatined from first 12 bits of these
**  2 bytes.  The remaining 4 bits are assumed to be the 2nd value.
**  
**  Owner - Ravi Shankar Ramesh - 2016 
********************************************************************************************************************************************/

/* Header File Inclusions */
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdbool.h>


/* Defines used in Program */
#define SIZE_OF_HEAP                32U
#define SIZE_OF_RECENT_ARRAY_VALUES 32U
#define NUM_BYTES_TO_READ           3U
#define SHIFT_BY_FOUR               4U
#define SHIFT_BY_EIGHT              8U
#define MIN_HEAP_ROOT               0U


/* Forward Declarations of Functions */
void heapify(int *array, int num_of_elements, int idx);
void create_min_heap(int *array, int num_of_elements);
void insert_min_heap(int *min_heap_array, int val);
void merge(int *array, int L, int M, int R);
void merge_sort(int *array, int L, int R);

/*===========================================================================
FUNCTION heapify

DESCRIPTION
Function to go heapify the given array. (Reorders Min-Heap)

DEPENDENCIES
None

RETURN VALUE
None 

SIDE EFFECTS
None
===========================================================================*/
void heapify(int *array, int num_of_elements, int idx)
{
  /* Local Variables */
  int smallest = 0;
  int L = 2*idx +1;
  int R = 2*idx +2;
  int temp =0;

  /*Determine Smallest Idx */
  if((L < num_of_elements) && (array[L] < array[idx]))
  {
    smallest = L;
  }
  else
  {
    smallest = idx;
  }

  if((R < num_of_elements) && (array[R] < array[smallest]))
  {
    smallest = R;
  }

  /* If Smallest is not Equal to Input Index, Swap and Heapify*/
  if(smallest != idx)
  {
	/* Perform Swap */
    temp = array[smallest];
    array[smallest] = array[idx];
    array[idx] = temp;
    /* Re-Heapify */
    heapify(array, num_of_elements, smallest);
  }
}

/*===========================================================================
FUNCTION create_min_heap

DESCRIPTION
Function called to Build Heap, once we have accumulated necessary values

DEPENDENCIES
None

RETURN VALUE
None 

SIDE EFFECTS
None
===========================================================================*/
void create_min_heap(int *array, int num_of_elements)
{
  int idx = 0;

  /* Traverse Length/2 of Heap array */
  for(idx = num_of_elements/2; idx>=0; idx--)
  {
	/* Call Heapify Function */
    heapify(array, num_of_elements, idx);
  }
}

/*===========================================================================
FUNCTION insert_min_heap

DESCRIPTION
Function that is used to Add value to Min Heap

DEPENDENCIES
None

RETURN VALUE
None 

SIDE EFFECTS
None
===========================================================================*/
void insert_min_heap(int *min_heap_array, int val)
{
    /* Static variables to maintain values between Function calls */
    static int min_heap_idx =0;
    static bool heapified = false;

	/* Add to Min Heap Array until we have accumulated 32 values */
    if(min_heap_idx < SIZE_OF_HEAP)
    {
      min_heap_array[min_heap_idx] = val;
      min_heap_idx++;
    }
    else
    {

      if(heapified == false)
      {
		/* Generate Min Heap, once we have accumulated necessary amount of values into array */
        create_min_heap(min_heap_array, SIZE_OF_HEAP);
	    heapified = true;
      }

	  /* Check that value is greater than root of min heap
	  if so, insert into root and heapify to bubble min value to root of Min Heap*/
      if(val > min_heap_array[MIN_HEAP_ROOT]) 
      {
        min_heap_array[0] = val;
		/*Heapify after Root is modified */
	    heapify(min_heap_array, SIZE_OF_HEAP, MIN_HEAP_ROOT);
      }
    }
}

/*===========================================================================
FUNCTION merge

DESCRIPTION
Function used by Merge Sort to Re-merge Sorted Values.

DEPENDENCIES
None

RETURN VALUE
None 

SIDE EFFECTS
None
===========================================================================*/
void merge(int *array, int L, int M, int R)
{
  /* Local Variables */
  int n1 = M-L+1;
  int n2 = R-M;
  
  /* Temp Array's for Left and Right Sorted Parts */
  int temp_array1[n1];
  int temp_array2[n2];
  int idx1 = 0, idx2 = 0, idx3 = 0;

  for (idx1 = 0; idx1 < n1; idx1++)
  {
	temp_array1[idx1] = array[L + idx1];
  }

  for (idx2 = 0; idx2< n2; idx2++)
  {
	temp_array2[idx2] = array[M + 1 + idx2];
  }

  idx1 =0;
  idx2 =0;
  idx3 =L;
  
  /* Merge Sorted values to Main array */
  while ((idx1 < n1) && (idx2 < n2))
  {
	if (temp_array1[idx1] < temp_array2[idx2])
	{
	  array[idx3++] = temp_array1[idx1++];
	}
	else
	{
      array[idx3++] = temp_array2[idx2++];
	}
  }

  while (idx1 < n1)
  {
    array[idx3++] = temp_array1[idx1++];
  }

  while (idx2 < n2)
  {
    array[idx3++] = temp_array2[idx2++];
  }
}

/*===========================================================================
FUNCTION merge_sort

DESCRIPTION
Function used to Merge Sort Given Input Array: Complexity O(nlogn)

DEPENDENCIES
None

RETURN VALUE
None 

SIDE EFFECTS
None
===========================================================================*/
void merge_sort(int *array, int L, int R)
{
  if (L < R)
  {
	 int M = L + ((R - L)/2);
	 merge_sort(array, L, M);
	 merge_sort(array, M+1, R);
	 merge(array, L, M, R);
  }
}

/*===========================================================================
FUNCTION main

DESCRIPTION
Main Function used to parse the Bit values and Entry point to Program

DEPENDENCIES
None

RETURN VALUE
None 

SIDE EFFECTS
None
===========================================================================*/
/* Main Program */
int main(int argc, char *argv[])
{

  /* File pointers for input and output files */
  FILE *fp, *fp2;
  int array[2] ={'\0', '\0'};                             // Array for Temp storage of values
  int idx =0;                                             // Index Counter
  int g_idx =0;                                           // Global Counter for Number of values parsed
  int num_bytes = NUM_BYTES_TO_READ;                      // Local to store Bytes Read, init. to 3
  int counter = 0;                                        // Counter for Loop Termination
  int min_heap[SIZE_OF_HEAP];                             // Min Heap Array
  char read_value[NUM_BYTES_TO_READ] ={'\0', '\0', '\0'}; //Array to read bytes from File
  int last_32_val_array[SIZE_OF_RECENT_ARRAY_VALUES];     //Array which Stores last 32 values parsed
  
  /* Initialize Array's */
  for(idx =0; idx<SIZE_OF_RECENT_ARRAY_VALUES; idx++)
  {
    last_32_val_array[idx] = 0;
  }

  /* Initialize Array's */
  for(idx =0; idx < SIZE_OF_HEAP; idx++)
  {
    min_heap[idx] = 0;
	if (idx < 2) 
	{
	 array[idx] = 0;
	}
  }

  /* Check for Correct Number of Arguments Passed */
  if (argc != 3)
  {
	printf("Incorrect number of Arguments Entered\n");
	printf("Please Enter in Following Format \n");
	printf("Eg: %s.exe test1.bin test1.out, %d \n", argv[0], argc);
	return(0);
  }
  /* Open the Input Binary File in Read Mode */
  fp = fopen(argv[1], "rb");
  if(fp == NULL)
  {
    printf("Unable to open Input file \n");
    return(0);
  }
  else
  {
	/* Bit manipulation logic to read from file 3 Bytes at a time, and parse the
	   12bit values and store it in corresponding arrays */
    while((num_bytes != 0) && (num_bytes != EOF))
    {
	  /* Reset the Read Array */
	  memset(read_value, '\0', NUM_BYTES_TO_READ);
      /* Read from file 3 bytes at a time */
      num_bytes = fread(read_value, 1, NUM_BYTES_TO_READ, fp);
      /* 3 bytes Available */
      if(num_bytes == NUM_BYTES_TO_READ)
      {
		/* Parse the First 12-Bit Value */
        array[0] = (read_value[0]<<SHIFT_BY_FOUR) | ((read_value[1] & 0xF0)>>SHIFT_BY_FOUR);
		/* Store in Array containg 32 recent values */
        last_32_val_array[g_idx % SIZE_OF_RECENT_ARRAY_VALUES] = array[0];
		/* Insert into Min Heap */
        insert_min_heap(min_heap, last_32_val_array[g_idx % SIZE_OF_RECENT_ARRAY_VALUES]);
		/* Increment Global Counter */
        g_idx++;

        /* Parse the 2nd 12 bit value */
        array[1] = ((read_value[1] & 0x0F)<<SHIFT_BY_EIGHT) | (read_value[2]);
		/* Store in Array containg 32 recent values */
        last_32_val_array[g_idx % SIZE_OF_RECENT_ARRAY_VALUES] = array[1];
		 /* Insert into Min Heap */
        insert_min_heap(min_heap, last_32_val_array[g_idx % SIZE_OF_RECENT_ARRAY_VALUES]);
		/* Increment Global Counter */
        g_idx++;
      }
      else
      {
		/* If number of bytes read was less than 3, due to lack of input values or EOF */
        if(num_bytes == 1)
        {
		  /* Parse the First 12-Bit Value */
          array[0] = read_value[0];
		  /* Store in Array containg 32 recent values */
    	  last_32_val_array[g_idx % SIZE_OF_RECENT_ARRAY_VALUES] = array[0];
		  /* Insert into Min Heap */
	      insert_min_heap(min_heap, last_32_val_array[g_idx % SIZE_OF_RECENT_ARRAY_VALUES]);
		  /* Increment Global Counter */
  	      g_idx++;
        }
        else if(num_bytes == 2)
        {
		  /* Parse the First 12-Bit Value */
          array[0] = (read_value[0]<<SHIFT_BY_FOUR) | ((read_value[1] & 0xF0)>>SHIFT_BY_FOUR);
		  /* Store in Array containg 32 recent values */
	      last_32_val_array[g_idx % SIZE_OF_RECENT_ARRAY_VALUES] = array[0];
		   /* Insert into Min Heap */
	      insert_min_heap(min_heap, last_32_val_array[g_idx % SIZE_OF_RECENT_ARRAY_VALUES]);
		  /* Increment Global Counter */
	      g_idx++;

          /* Parse the Second 12-Bit Value */
	      array[1] = ((read_value[1] & 0x0F));
		  /* Store in Array containg 32 recent values */
	      last_32_val_array[g_idx % SIZE_OF_RECENT_ARRAY_VALUES] = array[1];
		  /* Insert into Min Heap */
	      insert_min_heap(min_heap, last_32_val_array[g_idx % SIZE_OF_RECENT_ARRAY_VALUES]);
		  /* Increment Global Counter */
          g_idx++;
        }
      }
    }

	/* Check for Empty File */
	if (g_idx == 0)
	{
	  printf("No Values in Input File, please select valid File \n");
	  return(0);
	}

	/* Open File for output */
	fp2 = fopen(argv[2], "wb");
    if(fp2 == NULL)
    {
      printf("Unable to open file for output \n");
      return(0);
    }

	/* If Values read from file total less than 32 ouput to Disk */
	if (g_idx < SIZE_OF_RECENT_ARRAY_VALUES)
	{
	  /* Merge Sort the Min Heap for Disk Dump of Largest 32 Values */
      merge_sort(min_heap, 0, g_idx - 1);

	  /*Output to Disk */
	  fprintf(fp2, "%s\n", "--Sorted Max 32 Values--");
	  /*Output Largest 32 values */
      for(idx =0; idx<g_idx; idx++)
      {
	   /*Output to Disk */
	   fprintf(fp2,"%d\n",min_heap[idx]);
      }
	   /*Output to Disk */
      fprintf(fp2, "%s\n", "--Last 32 Valeus--");

	  for(idx = 0; idx < g_idx; idx++)
      {
		 /*Output to Disk */
	    fprintf(fp2,"%d\n",last_32_val_array[idx]);
      }
	}
	else
	{
      /* Merge Sort the Min Heap for Disk Dump of Largest 32 Values */
	  merge_sort(min_heap, 0, SIZE_OF_HEAP - 1);
	  fprintf(fp2, "%s\n", "--Sorted Max 32 Values--");

	  /*Output Largest 32 values */
      for(idx =0; idx<SIZE_OF_HEAP; idx++)
      {
		/*Output to Disk */
	    fprintf(fp2,"%d\n",min_heap[idx]);
      }
       /* Init Counter(Loop Termination) for Disk Dump*/
	   counter = 0;
	   /* Output to Disk */
	   fprintf(fp2, "%s\n", "--Last 32 Valeus--");
	   /* Output Most recent 32 values */
	   for(idx = g_idx % SIZE_OF_RECENT_ARRAY_VALUES; counter <  SIZE_OF_RECENT_ARRAY_VALUES; idx = ((idx + 1) % SIZE_OF_RECENT_ARRAY_VALUES))
       {
	    counter++;
        /* Output to Disk */
	    fprintf(fp2,"%d\n",last_32_val_array[idx]);
       }
	 }
   }

    /* Close Output File Handles */
  	fclose(fp);
	fclose(fp2);

	/* Terminate Function */
    return(0);
}

