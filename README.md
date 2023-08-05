# Assembler-Project
This is a **two-pass** assembler designed for a fictional computer model. 
It was developed using the C language as an assignment for a course at OpenU during my computer science studies.

***Course number:*** ```20465``` - ***Grade:*** ```100```.
## Fictional Computer Properties
- Includes 16 general registers marked r0-15 and a PSW register. The size of each one is 20 bits.
- The size of the memory is 8192 bits and the size of each cell is 20 bits.
## The Assembler Passes
**First Pass**

In the first pass of the assembler, the program identifies symbols (labels) used in the code and assigns each symbol a corresponding numerical value, representing its memory address.

**Second Pass**

During the second pass, the assembler utilizes the symbol values, operation codes, and register numbers to construct the machine code, resulting in the final executable output. 
## Build

1. Executing the  ```make``` command (makefile).
2. To execute the assembler, you need to provide a list of source file names as command-line arguments. These files are plain text files containing programs written in the specified assembly language syntax.  
The command pattern is:``` ./text_ana <Parameters>```.  
For example: ```./text_ana file1.as file2.as```
## Examples
The project comes with examples for valid and wrong inputs in the input directory. For everyone there is an appropriate output file.  
Additionally, you will receive standard output that provides details about the compilation process, such as:

  - ```Starting compliation of file "file1.as" ```
  * ```Complication of file "file1.as" finished successfully``` 
  * In case of input errors, it will display the error message ```Error in line X: 'shortly description...' ```
## Feedback
If you have any feedback, feel free to contact [me](https://github.com/yarintabashi) at yarintabashi@gmail.com or visit the "issues" section for further assistance.
