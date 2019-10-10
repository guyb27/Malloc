#include <malloc/malloc.h>
#include <stdlib.h>
#include <stdio.h>

int main(int ac, char **av)
{
	printf("%zu",(size_t)malloc_size(malloc(atoi(av[1]))));
}
