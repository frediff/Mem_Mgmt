# Memory Management - Project Description

### The project repository is available at https://github.com/frediff/Mem_Mgmt.

## Memory Management Module



<p align="justify">This assignment is related to virtual memory management inside the operating system. In general, in many large-scale real-world systems (e.g., database servers), the software does not depend on the OS to manage memory. In general, these software requests (right at the beginning) a chunk of memory using malloc (e.g.,500 MB, 1 GB). Then this software's customized memory management module manages the memory (like memory assignment or freeing allocated memory).</p>



#### Functionalities

The library provides the following functionalities:

> ###### `createMem`
>
> <p align="justify">A function to create a memory segment; code written using this library will use this function at the beginning to allocate the space. One can think of the returned memory as an initial large array of memory blocks.</p>

> ###### `createList`
>
> <p align="justify">Using this function, one can create a linked list of elements. One can think of each element as a structure with one integer and two indexes holding addresses of the previous and the next element in the list, that is, pointers (similar to head and tail pointers of a doubly linked list). These variables will reside in the memory created by <code>createMem</code>.</p>
>
> <p align="justify"><code>createList</code> takes in two arguments — the size of the list and the name of the list. Code written using this memory manager function can only use these linked lists of elements. Every time the program calls the <code>createList</code> function, it generates a new local address for the list, which maps the list name to the start address of the linked list. During updation and other operations, this local address is converted to a suitable address within the assigned memory to access the data sequentially. We use the concepts learned in paging and page tables. The program finds a valid free segment of the local address space to store new lists. If no free segment is big enough, then it prints an error and exits.</p>
>
> <p align="justify">Additionally, a function might contain local lists which are not needed when a function returns; just before returning, the library does some book-keeping to free the memory, namely popping the local variables from a global stack (to keep track of obsolete memory) and freeing up memory.</p>

> ###### `assignVal`
>
> <p align="justify">Assign a value to one element of the element lists in our locally managed memory. As input, it takes in the list's name, the element's offset, where we must write the updated value and the value itself. Returns error if the array size does not match the number of elements to be updated.</p>

> ###### `freeElem`
>
> <p align="justify">Using this function, we can free the memory occupied by lists when they are no longer used (as an example, consider freeing local lists when a function returns). One can call `freeElem` without any argument (to free any list not used anymore) or with the list's name to deallocate.</p>
>
> - `freeElem` will be called to free up space for local variables of a function right before return and will run whenever explicitly called.
> - `freeElem` updates the local page table.
> - Calling freedom on an invalid local address will raise an error and exit.

*The library prints out messages in the terminal when calling library functions and creating/updating local page table entries.*

#### Testing

<p align="justify">To demonstrate our library's functionality, we have written a code <code>mergesort.c</code> using calls from the library where applicable. The <code>main()</code> function takes 250 MB of memory. Then it creates a linked list of 50,000 elements and fills the values with random integers between 1 and 100,000. Next, we sort this list using a recursive merge sort algorithm. In the recursive call, the createList call creates additional lists. Note that to support recursion on the heap, one needs to scope the variable names, as different calls to the same function would have the same variable name that needs to be differentiated.</p>

#### Contributors

- Subham Ghosh (Roll No.: 20CS10065)
- Anubhav Dhar (Roll No.: 20CS30004)
- Aritra Mitra (Roll No.: 20CS30006)
- Shiladitya De (Roll No.: 20CS30061)
