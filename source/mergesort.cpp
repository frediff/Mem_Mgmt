
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


#include <iostream>


#include "goodmalloc.h"
using namespace std;

// each 
#define N_MAX 50000

void merge_sort(string og_list, int l, int r){
	int mid = (l + r) / 2;
	unsigned head_ptr;
	int r_sz = r - mid, l_sz = mid + 1 - l;
	int m_ptr = r, l_ptr = l_sz - 1, r_ptr = r_sz - 1;
	string l_list = "left_list_name";
	string r_list = "right_list_name";
	createList(l_list, l_sz, &head_ptr);
	createList(r_list, r_sz, &head_ptr);
	long long l_val, r_val;

	if(r == l){ // base case
		return;
	}

	// cerr << "MERGESORT(" << l << ", " << r << ")\n";
	// dbg(__LINE__); cin >> l_val;
	merge_sort(og_list, l, mid);

	// copy the contents to l_list and r_list
	for(int i = l; i <= mid; ++i){
		readVal(og_list, i, &l_val);
		assignVal(l_list, i - l, l_val);
	}

	merge_sort(og_list, mid + 1, r);
	for(int i = mid + 1; i <= r; ++i){
		readVal(og_list, i, &r_val);
		assignVal(r_list, i - (mid + 1), r_val);
	}

	// merge into merger_list
	while(l_ptr >= 0 && r_ptr >= 0){
		readVal(l_list, l_ptr, &l_val);
		readVal(r_list, r_ptr, &r_val);
		if(l_val > r_val){
			assignVal(og_list, m_ptr, l_val);
			--l_ptr;
		}else{
			assignVal(og_list, m_ptr, r_val);
			--r_ptr;		
		}
		--m_ptr;
	}

	while(l_ptr >= 0){
		readVal(l_list, l_ptr, &l_val);
		assignVal(og_list, m_ptr, l_val);
		--l_ptr;		
		--m_ptr;
	}

	while(r_ptr >= 0){
		readVal(r_list, r_ptr, &r_val);
		assignVal(og_list, m_ptr, r_val);
		--r_ptr;		
		--m_ptr;
	}

	return;
}

int main(){

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	cerr.tie(NULL);

	unsigned head_ptr;
	createMem(250000000 / 16);
	string og_list = "original_list_name";
	createList(og_list, N_MAX, &head_ptr);

	for(int i = 0; i < N_MAX; ++i){
		// cout << "Enter value of original_list[" << i << "]: ";
		// long long x;
		// cin >> x;
		long long x = rand() % (N_MAX * 2) + 1;
		assignVal(og_list, i, x);
	}

	cout << "Unsorted list is: \n{ ";
	for(int i = 0; i < N_MAX; ++i){
		long long x;
		readVal(og_list, i, &x);
		cout << x << ' ';
	}
	cout << "}\n";

	merge_sort(og_list, 0, N_MAX - 1);

	cout << "Sorted list is: \n{ ";
	for(int i = 0; i < N_MAX; ++i){
		long long x;
		readVal(og_list, i, &x);
		cout << x << ' ';
	}
	cout << "}\n";

	return 0;
}