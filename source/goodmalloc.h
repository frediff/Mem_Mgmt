
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////                                  //////////////////////////////////
//////////////////////////////////      OPERATING SYSTEMS LAB       //////////////////////////////////
//////////////////////////////////           ASSIGNMENT 6           //////////////////////////////////
//////////////////////////////////                                  //////////////////////////////////
//////////////////////////////////    ARITRA  MITRA  (20CS30006)    //////////////////////////////////
//////////////////////////////////    SUBHAM  GHOSH  (20CS10065)    //////////////////////////////////
//////////////////////////////////    SHILADITYA DE  (20CS30061)    //////////////////////////////////
//////////////////////////////////    ANUBHAV  DHAR  (20CS30004)    //////////////////////////////////
//////////////////////////////////                                  //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __INC_THIS_PARTICULAR_MEM_HEADER
#define __INC_THIS_PARTICULAR_MEM_HEADER

#include <string>
#include <string.h>

// for text colors
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define DEFAULT "\033[0m"
#define BOLD "\033[1m"
#define NONBOLD "\033[m"

#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"

// this simulates the NULL pointer
#define NO_PTR ((unsigned int)(-1))


struct list_meta_data{
	unsigned int head_ptr;
	unsigned int curr_ptr;
	unsigned int idx;
	unsigned int n;
};

struct element{ 
	long long val;           // eight bytes
	unsigned int prev_ptr;   // four  bytes 
	unsigned int next_ptr;   // four  bytes 

	// prev_ptr == NO_PTR indicates head
	// next_ptr == NO_PTR indicates tail
};

typedef element _page;


extern char * caller_func;


////////////////////////////// function prototypes //////////////////////////////

// creates memory so that N element nodes can be accomodated
// returns 0 on success, non-zero on error;
// createMem can be called per process only once
int createMem(unsigned int N);

// creates a list of n elements
// list_name must be a null terminanted non-empty string
// head pointer returned through ptr_to_head;
// ptr_to_head == NO_PTR on error
void createList(std::string list_name_string, unsigned int n, unsigned int * ptr_to_head);

// assigns idx-th element list_name with val
// crawling starts from last current pointer
// returns 0 on success, non-zero on error
int assignVal(std::string list_name_string, unsigned int idx, long long val);

// reads idx-th element list_name and assigns to val
// crawling starts from last current pointer
// returns 0 on success, non-zero on error
int readVal(std::string list_name_string, unsigned int idx, long long * val);

// frees a single list 
// the second argument is not visible to user
void freeElem(std::string list_name_string, bool called_from_just_free_elem = false);

// frees all lists
void freeELem();

// called when return is called
// calls freeElem() and deletes symbol table
void freeElemReturn();

// prints EVERYTHING
void dbg(int line = -1);



#ifndef __IS_OUR_MEM_C_FILE

#define createList caller_func=strdup(__func__);createList
#define freeElem caller_func=strdup(__func__);freeElem
#define return caller_func=strdup(__func__);freeElemReturn();return

#endif

#endif