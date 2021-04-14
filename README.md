# Operating System Memory Manager
CSC 300 Assignment

This project is based around the idea of how an operating system might manage its physical memory amongst multiple processes. By default, the operating system will have 1000 words of memory that it can share amongst processes. An input file is needed (by default input.txt) that will contain the commands that will be used to alter the memory.

A command must follow the format:
  \<command\>,\<word number\>,\<process number\>

The following are valid commands that can be used in the input file:
  - 0: create node
  - 1: expand node
  - 2: contract node
  - 3: mark node for deletion
  - 4: clean memory (delete nodes marked for deletion and combine separated, but contiguous processes)
  - 5: print memory usage of all processes
  
The included input.txt file has a basic set of commands for convenience.
