#include <unistd.h>
//#include <stdlib.h>
#include "malloc.c"
#include <time.h>

int main()
{
	int i = 1;
	int j = 0;
	char *s;
	//time_t sc;
	while (i < 10)
	{
		//sc = time(NULL);
		j = 0;
		s = malloc(sizeof(char) * ((i) + 1));
		while (j < i)
			s[j++] = 'b';
	//	s[0] = '\n';
		s[j] = '\0';
		write(1, s, i + 1);
		i+=1;
	}
	return (0);
}
