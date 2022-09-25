# Sistemul Biju

November 2021

<https://ocw.cs.pub.ro/courses/iocla/teme/tema-1>

Student: Maciuca Alexandru-Petru 324CA

Libraries used for the implementation:
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "linkedlist.h"
#include "stack.h"


## Implementation

	This is an idea of implementing a file system using simply linked lists where instead of a list, there is a root directory called "home" which points to different types of nodes. In this specific project nodes could be either files or directories.

### Structure
 
	There are two main structures :

(1)Dir -> stores a string(name), a File pointer(points to the first file
stored), and three Dir pointers(one to its parent directory, one to the next
directory stored on the same level and one to the first directory stored in it)

(2)File -> stores a string(name), a File pointer(points to the next file 
stored on the same level) and a Dir pointer(points to its parent directory)

	There are additional structures that help in solving PWD :

(3)ll_node_t -> stores a ll_node_t pointer to the next ll_node_t and some
generic void data.
(4)linked_list_t -> stores 2 ints(size = number of elements and datasize =
the length of the void data stored in ll_node_t) and a linkedlist_t pointer
to its head.
(5)stack_t -> stores a linkedlist type of pointer.

	The functions that are being used are mainly generical bash commands
such as touch, mkdir, ls, rm, rmdir, cd, tree, pwd and mv. 
	I choose an iterative implementation for functions such as touch, mkdir,
ls, rm, cd rmdir and mv, whereas tree, free, and pwd are being implemented 
recursively.
	The free function is being named free_system and it is also used in the
implementation of rmdir.
	An important thing is the way I choose to implement pwd given the fact that
it had to return a string. Since the memory for all strings had to be
dynamically allocated I thought it was rather ugly to reallocate the memory
in every recursive call so I choose to make my own stack. With the help of
this structure, the function could not be any simpler.
	
#### Difficulties
	
	Before deciding to make my own stack, I had some problems regarding
how I should do the PWD function. The Tree function was also troublesome
since in its original form it was also printing the "home" directory with 
some unnecessary indentations. Fortunately, I have managed to solve all
the tasks including the bonus.

##### Disclaimer

	Initially I had both the linkedlist functions and the stack functions
split in two different headers and two different .c sources but since I could
only send one .c file (tema1.c) I had to merge everything into this uglyness.
I hope it is still readable and decent looking.
