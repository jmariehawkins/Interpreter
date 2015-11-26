/*
*  Jessica Hawkins
*  Assignment 4
*
*  bitvector.h
*/

#ifndef BITVECTOR_H_
#define BITVECTOR_H_

#include <stdbool.h> //is this right?
#include <stdio.h>
#include <stdlib.h>

struct bitvector {
	int *elems; // all 1's and 0's
	int length; //length of array elems
	//int numelems; //number of elements in the set
};

// create and initialize an empty bitvector
struct bitvector bitinit (int size);

// Precondition: bitinit
// add element i to the bitvector
void bitadd (int x, struct bitvector *a);

// remove element i from the bitvector
void bitremove (int x, struct bitvector *a);

// return the larger of the two ints
// assumes x and y are valid ints
int max (int x, int y);

// compute the union of sets a and b
struct bitvector unionbit (struct bitvector a, struct bitvector b);

// compute the intersection of sets a and b
struct bitvector intersection (struct bitvector a, struct bitvector b);

// compute the difference of sets a and b; c = a - b
struct bitvector difference (struct bitvector a, struct bitvector b);

// test whether an int is a member of set a
bool ismember (int x, struct bitvector a);

// test whether set a is a subset of set b
bool issubset (struct bitvector a, struct bitvector b);

// test whether set a is equivalent to set b
bool equalsbit (struct bitvector a, struct bitvector b);

// return the actual size of the set (number of 1's)
int sizeofset (struct bitvector a);

// make a copy of set a
struct bitvector bitcopy (struct bitvector a);

void printbit (struct bitvector a);

#endif // BITVECTOR_H_
