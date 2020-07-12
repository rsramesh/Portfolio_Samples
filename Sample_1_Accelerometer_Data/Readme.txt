This is an ANSI C program which takes as arguments the name of a binary input file and a text output file.  The binary input file will contain 12 bit unsigned values.  The output file should contain the 32 largest values from the input file.  It should also contain the last 32 values read from the input file.  Try to write the portions of the code that are not related to IO if you would port it to a memory and speed limited device.  Attached you will find three sample input (.bin) and output (.out) files which you can use for testing. 

 
Notes about the implementation:

1. If there are an odd number of 12bit readings in the file, then the last nibble in the file will be zero and can be ignored.

2. The file format will be as follows

   - Start with “--Sorted Max 32 Values--"

   - The 32 largest values in the file, one value per line.  Duplicates are allowed if a large value appears multiple times.  The list should be sorted smallest to largest.

   - Output “--Last 32 Values--" on its own line

   - The last 32 values read, one per line.    They should appear in the order they were read.  The last value read from the file will appear last.

3. If there are fewer than 32 values in the file then whatever is read should be output

4. Your output files should match the output files provided with the given input.

5. If the program is passed bogus values, it should not core dump.

6. test#.bin is the binary file that corresponds with test#.out   Three test inputs and outputs are provided for validation.