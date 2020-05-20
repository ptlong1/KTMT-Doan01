#include "BitHandle.h"

int Getbit(int x, int i)
{
	return (x >> i) & 1;
}

void Batbit(int& x, int i)
{
	x = x | (1 << i);
}

void Daobit(int& x, int i)
{
	x = x ^ (1 << i);
}

void Daobit(int& x)
{
	x = ~x;
}
