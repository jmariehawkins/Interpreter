/*
*  Jessica Hawkins
*  Assignment 4
*
*  Bit vector
*/

#include "bitvector.h"

// create and initialize an empty bitvector
struct bitvector bitinit (int size)
{
	struct bitvector bitv;

	int* bits;
	bits = (int*) malloc(sizeof(int) *size); //stored on the heap

	int i;
	for (i = 0; i < size; i++)
	{
	   bits[i] = 0;  //probably automatically init to 0 anyway
	}

	bitv.elems = bits;
	bitv.length = size;

	return bitv;
}

// Precondition: bitinit
// add element i to the bitvector
void bitadd (int x, struct bitvector *a)
{
	int ln = (*a).length;
	if (0 <= x && x < ln)
	   (*a).elems[x] = 1;
	else
	   ; //error; element i was out of range
}

// remove element i from the bitvector
void bitremove (int x, struct bitvector *a)
{
	if (0 <= x && x < (*a).length)
	   (*a).elems[x] = 0;
}

// return the larger of the two ints
// assumes x and y are valid ints
int max (int x, int y)
{
	if (x > y)
	   return x;
	return y;
}

// compute the union of sets a and b
struct bitvector unionbit (struct bitvector a, struct bitvector b)
{
	int n = max (a.length, b.length);
	struct bitvector c = bitinit (n);

	if (a.length < b.length)
	{
	   int i;
	   for (i = 0; i < a.length; i++)
	   {
		if (a.elems[i] == 1 || b.elems[i] == 1)
		   bitadd (i, &c);
	   }

	   for (i = a.length; i < b.length; i++)
	   {
		if (b.elems[i] == 1)
		   bitadd (i, &c);
	   }
	}
	else
	{
	   int i;
	   for (i = 0; i < b.length; i++)
	   {
		if (a.elems[i] == 1 || b.elems[i] == 1)
		   bitadd (i, &c);
	   }

	   for (i = b.length; i < a.length; i++)
	   {
		if (a.elems[i] == 1)
		   bitadd (i, &c);
	   }
	}

	return c;
}

// compute the intersection of sets a and b
struct bitvector intersection (struct bitvector a, struct bitvector b)
{
	int n = max (a.length, b.length);
	struct bitvector c = bitinit (n);

	if (a.length < b.length)
	{
	   int i;
	   for (i = 0; i < a.length; i++)
	   {
		if (a.elems[i] == 1 && b.elems[i] == 1)
		   bitadd (i, &c);
	   }
	}
	else
	{
	   int i;
	   for (i = 0; i < b.length; i++)
	   {
		if (a.elems[i] == 1 && b.elems[i] == 1)
		   bitadd (i, &c);
	   }
	}

	return c;
}

// compute the difference of sets a and b; c = a - b
struct bitvector difference (struct bitvector a, struct bitvector b)
{
	int n = max (a.length, b.length);
	struct bitvector c = bitinit (n);

	if (a.length < b.length)
	{
	   int i;
	   for (i = 0; i < a.length; i++)
	   {
		if (a.elems[i] == 1 && b.elems[i] != 1)
		   bitadd (i, &c);
	   }
	}
	else
	{
	   int i;
	   for (i = 0; i < b.length; i++)
	   {
		if (a.elems[i] == 1 || b.elems[i] == 1)
		   bitadd (i, &c);
	   }

	   for (i = b.length; i < a.length; i++)
	   {
		if (a.elems[i] == 1)
		   bitadd (i, &c);
	   }
	}

	return c;
}

// test whether an int is a member of set a
bool ismember (int x, struct bitvector a)
{
	if (0 <= x && x < a.length)
	{
	   if (a.elems[x] == 1)
		return true;
	   else
		return false;
	}
	else
	   return false;
}

// test whether set a is a subset of set b
bool issubset (struct bitvector a, struct bitvector b)
{
	if (a.length > b.length)
	{
	   int i;
	   for (i = 0; i < b.length; i++)
	   {
	   	if (a.elems[i] != b.elems[i])
		   return false;
	   }

	   for (i = b.length; i < a.length; i++)
	   {
		if (a.elems[i] != 0)
		   return false;
	   }
	}
	else  // a.length <= b.length
	{
	   int i;
 	   for (i = 0; i < a.length; i++)
	   {
	   	if (a.elems[i] != b.elems[i])
		   return false;
	   }
	}

	return true;
}

// test whether set a is equivalent to set b
bool equalsbit (struct bitvector a, struct bitvector b)
{
	if (a.length != b.length)
	   return false;

	int i;
	for (i = 0; i < a.length; i++)
	{
	   if (a.elems[i] != b.elems[i])
		return false;
	}

	return true;
}

// return the actual size of the set (number of 1's)
int sizeofset (struct bitvector a)
{
	int s = 0;
	int i;
	for (i = 0; i < a.length; i++)
	{
	   if (a.elems[i] == 1)
		s++;
	}

	return s;
}

//remove an existing element from a non-empty set and produce both
//that element and the resulting set (this allows one to //enumerate
// through all the elemens in a a simple way);  ?
//  - need to pass double pointer?

// make a copy of set a
struct bitvector bitcopy (struct bitvector a)
{
	struct bitvector b;
	b = bitinit (a.length);
	
	int i;
	for (i = 0; i < a.length; i++)
	{
	   if (a.elems[i] == 1)
		b.elems[i] = 1;
	}

	return b;
}

void printbit (struct bitvector a)
{
	printf("[");

	int i;
	for (i = 0; i < a.length; i++)
	{
	   printf ("%d|", a.elems[i]);
	}

	printf("]\n");
}

// need a destroy method to free the memory allocated to the elem
// array?


