
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

#define __IS_OUR_MEM_C_FILE

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <map>
#include <utility>
#include <string>
#include "goodmalloc.h"

using namespace std;


// string to store the caller function name 
// from which createmem() or freeElem was called
char * caller_func;

// vector of symbol tables, list_name -> list meta data
vector<map<string, list_meta_data>> symbol_tables; 

// function name to index in symbol_table vector
stack<pair<string, int>> caller_stack;

// free list: indices in MEM which are free
stack<unsigned int> free_list;
// This is a stack because we want to use the memory 
// which was freed recently. This might already been 
// in cache, so cache hit rate increases.

// list name to indices in symbol_tables (top being the youngest)
map<string, stack<int>> identifier_to_ST;

// the pointer to the first element in malloc
_page * MEM = NULL;

// creates memory so that N element nodes can be accomodated
// returns 0 on success, non-zero on error;
// createMem can be called per process only once
int createMem(unsigned int N){
	// malloc N pages
	if(MEM != NULL){ // not the first time createMem is called
		return 1;
	}
	if(N == 0){
		return 2;
	}
	MEM = (_page *)malloc(sizeof(_page) * N);
	if(MEM == NULL){ // error
		return 4;
	}
	for(unsigned int i = N - 1; i != 0; --i){ // is not time consuming in amortized sense
		free_list.push(i);
	}
	free_list.push(0);
	return 0;
}

// creates a list of n elements
// list_name must be a null terminanted non-empty string
// head pointer returned through ptr_to_head;
// ptr_to_head == NO_PTR on error
void createList(string list_name_string, unsigned int n, unsigned int * ptr_to_head){
	
	// store variables in c++ strings
	string caller_func_string;
	for(char * c = caller_func; *c != '\0'; ++c){
		caller_func_string.push_back(*c);
	} 
	free(caller_func);

	// debug
	// fprintf(stderr, "createList called from function %s\n", caller_func_string.c_str());

	// either empty string given or free list is insufficient
	if(list_name_string.size() == 0 || n > free_list.size() || n == 0){
		*ptr_to_head = NO_PTR;
		return;
	}

	// if no function with such name exists, new symbol table needed for this function
	// if names repeat, then this is a recursed function; new symbol table needed
	if(caller_stack.empty() || caller_stack.top().first != caller_func_string || 
		symbol_tables.back().find(list_name_string) != symbol_tables.back().end()){

		// add one entry to symbol_table & point it to, at caller_stack
		caller_stack.push({caller_func_string, (int)symbol_tables.size()});
		symbol_tables.resize(symbol_tables.size() + 1);

	}

	// allocate using first fit
	unsigned int prev_elem = NO_PTR;
	for(unsigned int i = 0; i < n; ++i){
		// pop from free list
		unsigned int curr_elem = free_list.top();
		free_list.pop();

		// manage pointer to add this to the list
		MEM[curr_elem].prev_ptr = prev_elem;
		(prev_elem == NO_PTR) ? (*ptr_to_head = curr_elem) : (MEM[prev_elem].next_ptr = curr_elem); 

		// update prev_elem
		prev_elem = curr_elem;

	}
	MEM[prev_elem].next_ptr = NO_PTR; // tail

	// add this to symbol table
	symbol_tables[caller_stack.size() - 1][list_name_string] = list_meta_data{*ptr_to_head, *ptr_to_head, 0, n};
	identifier_to_ST[list_name_string].push(symbol_tables.size() - 1);

	return;

}

// assigns idx-th element list_name with val
// crawling starts from last current pointer
// returns 0 on success, non-zero on error
int assignVal(string list_name_string, unsigned int idx, long long val){
	if(symbol_tables.empty()){
		return 1;
	}
	if(identifier_to_ST[list_name_string].empty()){ 
		return 2;
	}
	list_meta_data & temp_lmd = symbol_tables[identifier_to_ST[list_name_string].top()][list_name_string];
	if(idx >= temp_lmd.n){ // index out of bound
		return 8;
	}

	// optimization: if idx is skewed to the beginning, start from head
	// if(idx * 2 < temp_lmd.idx){
	// 	temp_lmd.idx = 0;
	// 	temp_lmd.curr_ptr = temp_lmd.head_ptr;
	// }

	while(temp_lmd.idx > idx){
		temp_lmd.idx--;
		temp_lmd.curr_ptr = MEM[temp_lmd.curr_ptr].prev_ptr;
	}

	while(temp_lmd.idx < idx){
		temp_lmd.idx++;
		temp_lmd.curr_ptr = MEM[temp_lmd.curr_ptr].next_ptr;
	}

	MEM[temp_lmd.curr_ptr].val = val;
	cerr << list_name_string << "[" << idx << "] := " << val << "\n";

	return 0;

}


// reads idx-th element list_name and assigns to val
// crawling starts from last current pointer
// returns 0 on success, non-zero on error
int readVal(string list_name_string, unsigned int idx, long long * val){
	if(symbol_tables.empty()){
		return 1;
	}
	if(identifier_to_ST[list_name_string].empty()){ 
		return 2;
	}
	list_meta_data & temp_lmd = symbol_tables[identifier_to_ST[list_name_string].top()][list_name_string];
	if(idx >= temp_lmd.n){ // index out of bound
		return 4;
	}

	// optimization: if idx is skewed to the beginning, start from head
	// if(idx * 2 < temp_lmd.idx){
	// 	temp_lmd.idx = 0;
	// 	temp_lmd.curr_ptr = temp_lmd.head_ptr;
	// }

	while(temp_lmd.idx > idx){
		temp_lmd.idx--;
		temp_lmd.curr_ptr = MEM[temp_lmd.curr_ptr].prev_ptr;
	}

	while(temp_lmd.idx < idx){
		temp_lmd.idx++;
		temp_lmd.curr_ptr = MEM[temp_lmd.curr_ptr].next_ptr;
	}

	*val = MEM[temp_lmd.curr_ptr].val;
	cerr << list_name_string << "[" << idx << "] == " << *val << "\n";
	
	return 0;

}

// frees element 
void freeElem(string list_name_string, bool called_from_just_free_elem){

	// store variables in c++ strings
	string caller_func_string;
	for(char * c = caller_func; *c != '\0'; ++c){
		caller_func_string.push_back(*c);
	}
	if(!called_from_just_free_elem){
		free(caller_func);
	}

	// debug
	// fprintf(stderr, "freeElem(...) called from function %s\n", caller_func_string.c_str());

	// if no function with such name exists, ignore
	// if no listname exists, ignore
	if(caller_stack.empty() || caller_stack.top().first != caller_func_string || 
		symbol_tables[caller_stack.top().second].find(list_name_string) == symbol_tables[caller_stack.top().second].end()){

		return;

	}

	// find meta data of list to be erased
	list_meta_data & temp_lmd = symbol_tables.back()[list_name_string];

	// crawl from head, add these to free list;
	while(temp_lmd.head_ptr != NO_PTR){
		free_list.push(temp_lmd.head_ptr); // add to free list
		temp_lmd.head_ptr = MEM[temp_lmd.head_ptr].next_ptr;
	}

	// delete this entry from symbol table
	symbol_tables.back().erase(symbol_tables.back().find(list_name_string));
	identifier_to_ST[list_name_string].pop();

}


// frees all lists
void freeElem(){
	// store variables in c++ strings
	string caller_func_string;
	for(char * c = caller_func; *c != '\0'; ++c){
		caller_func_string.push_back(*c);
	} 

	// debug
	// fprintf(stderr, "freeElem() called from function %s\n", caller_func_string.c_str());

	// if no function with such name exists, ignore
	if(caller_stack.empty() || caller_stack.top().first != caller_func_string){
		free(caller_func);
		return;
	}

	// free elements one by one
	while(!symbol_tables.back().empty()){
		freeElem(symbol_tables.back().begin() -> first, true);		
	}

	free(caller_func);
}


// called when return is called
// calls freeElem() and deletes symbol table
void freeElemReturn(){
	// store variables in c++ strings
	string caller_func_string;
	for(char * c = caller_func; *c != '\0'; ++c){
		caller_func_string.push_back(*c);
	} 

	// debug
	// fprintf(stderr, "Return called from function %s\n", caller_func);

	// if no function with such name exists, ignore
	if(caller_stack.empty() || caller_stack.top().first != caller_func_string){
		free(caller_func);
		return;
	}
	
	// call freeElem()
	freeElem();

	// pop the caller stack and delete the symbol table
	caller_stack.pop();
	symbol_tables.pop_back();

}


void dbg(int line){

	cerr << "\n\n\n ===================================================== \n";
	cerr << "\n LINE : " << line << '\n';
	cerr << "\n ##################################################### \n";
	cerr << "\n LIST OF SYMBOL TABLES [" << symbol_tables.size() << "]:\n";
	for(map<string, list_meta_data> &ST : symbol_tables){
		cerr << "\n ------------------------------------------------------------ \n";
		for(auto mp : ST){
			cerr << "\t" << mp.first << " : \t{" << mp.second.head_ptr << ", " << mp.second.curr_ptr << ", " << mp.second.idx << ", " << mp.second.n << "}\n";
		}
		cerr << " ------------------------------------------------------------ \n";
	}


	cerr << "\n ##################################################### \n";
	cerr << "\n YOUNGEST ST's [" << identifier_to_ST.size() << "]:\n";
	for(auto &item : identifier_to_ST){
		cerr << "\t" << item.first << "\t";
		if(item.second.empty()){
			cerr << "x\n";
		}else{
			cerr << item.second.top() << '\n';
		}
	}
	cerr << "\n ===================================================== \n\n\n";
}

