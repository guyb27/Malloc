/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   test.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/18 20:29:04 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/13 07:55:18 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <sys/mman.h>

#define TINY_MAX 256
#define SMALL_MAX 512
#include <unistd.h>


typedef struct			s_block
{
	size_t				start_address;//I : [√]
	size_t				octets_used;//I : [√]
	size_t				octets_available;
	size_t				mmap_number;
	struct s_block		*prev;//I : [√]
	struct s_block		*next;//I : [√]
}						t_block;

typedef struct			s_hitoric
{
	size_t				start_address;
	size_t				end_address;
	int					action_type;
	struct tm			*timeInfos;
}						t_historic;

typedef struct			s_areas
{
	t_block				*tiny;//I : [√]
	t_block				*small;
	t_block				*large;
	size_t				total_use;
	int					page_size;//[X]SIZE_T
	size_t				nb_mmap;
	t_historic			historic[100];
}						t_areas;

t_areas			g_areas;

int				g_i = 0;
int				g_attrib_mem = 0;
int				g_next = 0;
int				g_new_alloc = 0;

/*void	*ft_memset(void *b, int c, size_t len)
{
	int i;

	i = 0;
	while (len--)
		((unsigned char *)b)[i++] = (unsigned char)c;
	return (b);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}

void	ft_putstr(const char *s)
{
	int i = 0;
	if (s)
	{
		while (s[i])
			write(1, &s[i], 1);
	}
}

size_t		ft_nbrlen(int n)
{
	size_t i;

	i = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		n /= 10;
		++i;
	}
	return (i);
}

static void		ft_print_itoa(int n, size_t len)
{
	char		str[len + 1];
	size_t		i;
	long int	nb;

	i = -1;
	ft_bzero(str, sizeof(str));
	while (str[++i])
		str[i] = '\0';
	nb = (long int)n;
	i = n < 0 ? (ft_nbrlen(n) + 1) : ft_nbrlen(n);
	nb *= n < 0 ? -1 : 1;
	while (nb != 0 && i != 0)
	{
		str[--i] = '0' + (nb % 10);
		nb /= 10;
	}
	n < 0 ? str[0] = '-' : 0;
	n == 0 ? str[0] = '0' : 0;
	ft_putstr(str);
}

void			ft_putnbr(int nb)
{
	size_t len;

	len = 0;
	len = nb < 0 ? ft_nbrlen(nb * -1) : ft_nbrlen(nb);
	if (nb >= 2147483647)
		ft_putstr("2147483647");
	else if (nb <= -2147483648)
		ft_putstr("-2147483648");
	else
		nb < 0 ? ft_print_itoa(nb, len + 1) : ft_print_itoa(nb, len);
}
void	ft_putendl(const char *s)
{
	ft_putstr(s);
	write(1, "\n", 1);
}
*/
void		*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*dest;
	int		i;

	dest = dst;
	i = -1;
	while (++i < (int)n)
		dest[i] = ((char*)src)[i];
	return (dest);
}

size_t		next_hexa_base(size_t size)
{
	return (size % 16 ? size + 16 - (size % 16) : size);
}

int		next_page_size(size_t size, size_t page_size)
{
	return (size % page_size ? size + page_size - (size % page_size) : size);
}

void	show_alloc_mem()
{
/*	
	   printf("=================================RENDU=============================\n");
	   printf("TINY : 0x%zu\n", (size_t)g_areas.tiny);
	   t_block	*tiny = g_areas.tiny, *small = g_areas.small, *large = g_areas.large;
	   int j = 0;
	   while (tiny)
	   {
	   printf("[%d]0x%zu - 0x%zu : %zu octets_available, %zu octets_used\n", j, tiny->start_address, tiny->start_address + tiny->octets_used, tiny->octets_available, tiny->octets_used);
	   tiny = tiny->next;
	   j++;
	   }
	   j = 0;
	   printf("SMALL : 0x%zx\n", (size_t)g_areas.small);
	   while (small)
	   {
	   printf("[%d] 0x%zx - 0x%zx : %zu octets_available, %zu octets_used\n", j, small->start_address, small->start_address + small->octets_used, small->octets_available, small->octets_used);
	   j++;
	   small = small->next;
	   }
	   j = 0;
	   printf("LARGE : 0x%zx\n", large ? (size_t)large : 0);
	   while (large)
	   {
	   printf("[%d] 0x%zx - 0x%zx : %zu octets_available, %zu octets_used\n", j, large->start_address, large->start_address + large->octets_used, large->octets_available, large->octets_used);
	   j++;
	   large = large->next;
	   }
	   printf("Total : %zu octets\n", g_areas.total_use);
*/	  
}

void	init_areas()
{
	size_t t_block_size;

	g_areas.page_size = getpagesize();
	t_block_size = next_hexa_base(sizeof(t_block));
	size_t tiny_size = (TINY_MAX * 100) + (t_block_size * 101);
	size_t small_size = (SMALL_MAX * 100) + (t_block_size * 101);
	//size_t used = 0;
	//size_t	total_size;

	if (tiny_size % g_areas.page_size)
		tiny_size += g_areas.page_size - (tiny_size % g_areas.page_size);
	if (small_size % g_areas.page_size)
		small_size += g_areas.page_size - (small_size % g_areas.page_size);
	//TINY
	g_areas.tiny = mmap(0, small_size + tiny_size, PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_PRIVATE, -1, 0);
	// PK LE NEXT_HEXA_BASE ? LES LARGES ? SI OUI JE CROIS QUE CE N EST PAS BON CAR ON LUI ATTRIBUT DE LA MEMOIRE UNIQUEMENT SUR LE TAS
	g_areas.tiny->octets_used = 0;
	g_areas.tiny->start_address = (size_t)g_areas.tiny + t_block_size;
	g_areas.tiny->octets_available = tiny_size - t_block_size;
	g_areas.tiny->octets_used = 0;
	g_areas.tiny->prev = NULL;
	g_areas.tiny->next = NULL;
	g_areas.tiny->mmap_number = 0;

	//SMALL
	g_areas.small = (void*)((size_t)g_areas.tiny + tiny_size);
	g_areas.small->octets_used = 0;
	g_areas.small->start_address = (size_t)g_areas.small + t_block_size;
	g_areas.small->octets_available = small_size - t_block_size;
	g_areas.small->octets_used = 0;
	g_areas.small->prev = NULL;
	g_areas.small->next = NULL;
	g_areas.small->mmap_number = 1;

	g_areas.large = NULL;
	g_areas.total_use = 0;
	g_areas.nb_mmap = 1;

	t_block * tiny = g_areas.tiny;
//	   printf("DEBUT 0x%zu - 0x%zu : %zu octets_available, %zu octets_used\n", tiny->start_address, tiny->start_address + tiny->octets_used, tiny->octets_available, tiny->octets_used);
	//PRINT
/*	
	   printf("\t\tGLOBAL\n");
	   printf("START ADDRESS : [0x%zu]\n", (size_t)g_areas.tiny);
	   printf("END ADDRESS : [0x%zu]\n", (size_t)g_areas.tiny + tiny_size + small_size);
	   printf("---------------------------------------------------------------\n");
	   printf("\t\tTINY\n");
	   printf("TINY_SIZE : [%zi]\n", tiny_size);
	   printf("TINY.BLOCK (ADDRESS) : [%zu]\n", (size_t)g_areas.tiny);
	   printf("T_BLOCK_SIZE : [%zi]\n", next_hexa_base(sizeof(t_block)));
	   printf("OCTETS_AVAILABLE(T_BLOCK) : [%zi]\n", (size_t)g_areas.tiny->octets_available);
	   printf("ADDRESS TINY[0] + T_BLOCK_SIZE : [%zi]\n", next_hexa_base(sizeof(t_block)) + (size_t)g_areas.tiny);
	   printf("TINY.BLOCK[0]->START_ADDRESS : [0x%zu]\n", (size_t)g_areas.tiny->start_address);
	   printf("TINY.BLOCK[0]->END_ADDRESS : [0x%zu]\n", (size_t)g_areas.tiny->start_address + g_areas.tiny->octets_available);
	   printf("---------------------------------------------------------------\n");
	   printf("\t\tSMALL\n");
	   printf("SMALL_SIZE : [%zi]\n", small_size);
	   printf("SMALL (ADDRESS) : [%zu]\n", (size_t)g_areas.small);
	   printf("OCTETS_AVAILABLE(T_ZONE) : [%zi]\n", (size_t)g_areas.small->octets_available);
	   printf("SMALL BLOCK[0]->START_ADDRESS : [0x%zu]\n", (size_t)g_areas.small->start_address);
	   printf("SMALL BLOCK[0]->END_ADDRESS : [0x%zu]\n", (size_t)g_areas.small->start_address + g_areas.small->octets_available);
	   printf("---------------------------------------------------------------\n");
	   */
}

void	*new_alloc(t_block **original_block, size_t size)
{
	t_block		*block;
	t_block		*new_block;
	size_t		new_size;
	size_t		t_block_size;

	t_block_size = next_hexa_base(sizeof(t_block));
	block = *original_block;
	while (block->next)
		block = block->next;
	//new_size = next_hexa_base((size * 100) + (t_block_size * 100));//DE BASE  C EST CA MAIS JE TEST AVEC LA SUiVANTE LIGNE
	new_size = next_page_size((size * 100) + (t_block_size * 100), g_areas.page_size);
	new_block = mmap(0, new_size,
			PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	g_areas.nb_mmap += 1;
	new_block->mmap_number = g_areas.nb_mmap;
	block->next = new_block;
	new_block->prev = block;
	new_block->next = NULL;
	new_block->octets_used = 0;
	new_block->octets_available = new_size - t_block_size;
	new_block->start_address = (size_t)new_block + t_block_size;
	return (new_block);
}

void	*attrib_memory(t_block **original_block, size_t size)
{
	t_block		*block;
	t_block		*new_block;
	size_t		memory_rest;

	block = *original_block;
	memory_rest = block->octets_available - size;
	g_areas.total_use += size;
	block->octets_used = size;
	block->octets_available = 0;
	if (memory_rest > next_hexa_base(sizeof(t_block)))
	{
		new_block = (void*)(block->start_address + block->octets_used);
		new_block->next = block->next;
		new_block->prev = block;
		block->next = new_block;
		new_block->start_address = (size_t)new_block + next_hexa_base(sizeof(t_block));
		new_block->octets_used = 0;
		new_block->mmap_number = block->mmap_number;
		new_block->octets_available = memory_rest - next_hexa_base(sizeof(t_block));
	}
	else if (memory_rest < next_hexa_base(sizeof(t_block)) && !block->next)//A VERIFIER
	{
		block->octets_used += memory_rest;
		new_alloc(&block, size <= TINY_MAX ? TINY_MAX : SMALL_MAX);
	}
	return ((void*)block->start_address);
}

void	*search_block(t_block **original_block, size_t size)
{
	int			found;
	t_block		*block;
	int	i =	0;

	found = 0;
	block = *original_block;
	while (!found && block)
	{
		if ((!block->octets_used && block->octets_available >= size))
		{
			g_attrib_mem += 1;
			found = 1;
			return (attrib_memory(&block, size));
		}
		else if (!block->next)
		{
			g_new_alloc += 1;
			new_alloc(&block, size <= TINY_MAX ? TINY_MAX : SMALL_MAX);
		}
		else
		{
			g_next += 1;
			block = block->next;
		}
		i++;
	}
	return (NULL);
}

void	*new_large(size_t size)
{
	t_block	*new_large;
	t_block	*tmp_block;
	size_t	t_block_size;

	t_block_size = next_hexa_base(sizeof(t_block));
	size = size + t_block_size;
	tmp_block = g_areas.large;
	if (size % g_areas.page_size)
		size += g_areas.page_size - (size % g_areas.page_size);	
	if(!((void*)(new_large = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON |
						MAP_PRIVATE, -1, 0)) == MAP_FAILED))
	
	//if((new_large = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON |
	//					MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	//{
	//	printf("NEWLARGE == [%zx]\n", (size_t)new_large);
	//	return NULL;
	//}
	{
		while (tmp_block && tmp_block->next)
			tmp_block = tmp_block->next;
		if (tmp_block)
		{
			tmp_block->next = new_large;
			new_large->prev = tmp_block;
		}
		else
		{
			g_areas.large = new_large;
			new_large->prev = NULL;
		}
		new_large->start_address = (size_t)new_large + t_block_size;
		new_large->octets_available = 0;
		new_large->octets_used = size - t_block_size;
		new_large->next = NULL;
		g_areas.total_use += size - t_block_size;
	}
	else
		return (NULL);
	return ((void*)new_large->start_address);
}

int		free_large(t_block **original_block)
{
	t_block *large;
	int		ret;

	large = *original_block;
	g_areas.total_use -= large->octets_used;
	if (large->next)
		large->next->prev = large->prev;
	if (large->prev)
		large->prev->next = large->next;
	if (!large->next && !large->prev)
	{
		ret = munmap(g_areas.large, large->octets_used +
				next_hexa_base(sizeof(t_block)));
		g_areas.large = NULL;
		return (ret);
	}
	if (!large->prev)
		g_areas.large = g_areas.large->next;
	ret = munmap(large, large->octets_used +
			next_hexa_base(sizeof(t_block)));
	return (ret);
}

void	free_tiny_and_small(t_block **original_block, size_t size)//PB AVEC REALLOC
{
	t_block *block;

	block = *original_block;
	size = size + 3;
	g_areas.total_use -= block->octets_used;
	if (block->next && block->mmap_number == block->next->mmap_number &&
			block->next->octets_available)
	{
//		printf("BLOCK->NEXT\n");
//		printf("0OCTETS_AVAILABLE : [%zi]\n", block->octets_available);
		block->octets_available = block->octets_used + block->next->octets_available + next_hexa_base(sizeof(t_block));
		block->octets_used = 0;
		if (block->next && block->next->next)
		{
			block->next->next->prev = block;
			block->next = block->next->next;
		}
		else
			block->next = NULL;
//		printf("1OCTETS_AVAILABLE : [%zi]\n", block->octets_available);
	}
	else
	{
		block->octets_available = block->octets_used;
	block->octets_used = 0;
//		printf("!BLOCK->NEXT\n");
	}
	if (block->prev && block->prev->octets_available && block->prev->mmap_number
			== block->mmap_number)
	{
//		printf("BLOCK->PREV\n");
		block->prev->octets_available += block->octets_available +
			block->octets_used + next_hexa_base(sizeof(t_block));
		block->prev->next = block->next;
		if (block->next)
			block->next->prev = block->prev;
		block = block->prev;
	}
	else if (!block->prev)
	{
//		printf("!BLOCK->PREV");
		block->octets_available += block->octets_used;
	}
//	if (!block->prev && block->octets_available)
//	{
//		block->octets_available += size;
//	}
	block->octets_used = 0;
}

t_block	*used_search(void *ptr, t_block **original_block)
{
	t_block	*block;

	block = *original_block;
	while (block)
	{
		if (block->start_address == (size_t)ptr && block->octets_used > 0)
			return (block);
		else
			block = block->next;
	}
	return (NULL);
}

void		free(void *ptr)
{
	t_block		*block;
	//NE PAS OUBLIER SI C EST UN MAILLON LIBRE DE NE PAS LE FREE ! ! !
	if (ptr != MAP_FAILED)
	{
		if ((block = used_search(ptr, &g_areas.tiny)) ||
				(block = used_search(ptr, &g_areas.small)))
			free_tiny_and_small(&block, block->octets_used);
		else if ((block = used_search(ptr, &g_areas.large)))
			free_large(&block);
	}
}

void	*malloc(size_t size)
{
	//int						page_size;
	void					*ret;

	//int i = 0;
	g_attrib_mem = 0;
	g_next = 0;
	g_new_alloc = 0;
	size = next_hexa_base(size);
	ret = NULL;
	if (!g_areas.tiny)
		init_areas();
	if (size <= SMALL_MAX)
		ret = search_block(size <= TINY_MAX ? &g_areas.tiny : &g_areas.small, size);
	else if (size)
		ret = new_large(size);
	return (ret);
}

int		is_concat_struct(t_block *block, size_t size)
{
	if (((block->octets_used <= TINY_MAX && size <= TINY_MAX) ||
				(block->octets_used <= SMALL_MAX && size <= SMALL_MAX)) &&
			block->next && block->next->octets_available >= size)
		return (1);
	else
		return (0);
}

void	*realloc(void *ptr, size_t size)
{
	t_block		*block;
	t_block		*new_block;
	int			i;
	size_t		new_size;
//	size_t		memory_rest;

	i = 0;
	if (ptr && size > 0)
	{
		size = next_hexa_base(size);
		if ((block = used_search(ptr, &g_areas.tiny)) ||
				(block = used_search(ptr, &g_areas.small)))
		{
			//	if (is_concat_struct(block, size))
			//	{

			//	}
			//	else
			//	{
			new_size = block->octets_used <= size ? block->octets_used : size;
			if ((new_block = malloc(size)) != NULL)
			{
				ft_memcpy(new_block, (void*)block->start_address, new_size);
				free((void*)block->start_address);
			}
//			else
//				printf("MALLOC FOIRE\n");
			//	}
			return (new_block);
		}
		else if ((block = used_search(ptr, &g_areas.large)))
		{
//			//printf("REALLOC1\n");
			new_size = block->octets_used <= size ? block->octets_used : size;
			if ((new_block = malloc(size)) != NULL)
			{
				ft_memcpy(new_block, (void*)block->start_address, new_size);
			//			printf("SIZE : [%zi], NEW _ADDRESS: [%zi], ADDRESS: [%zi]\n", size, (size_t)new_block, (size_t)block->start_address);
			//free((void*)block->start_address);
			free_large(&block);
			//			printf("END REALLOC\n");
			}
//			else
//				printf("MALLOC FOIRE\n");
			return (new_block);
		}
	}
	else if (ptr == NULL)
	{
		size = next_hexa_base(size);
		return (malloc(size));
	}
	else if (size == 0 && ptr)
		free(ptr);
	return (NULL);
}
/*
   int		main(int ac, char **av)
   {
   int i = 0, j = 0;
   void *t;
   void *tmp;

   while(i < 3)
   {
	   printf("I : [%d][0] DEBUT\n", i);
   free(malloc(atoi(av[1])));
show_alloc_mem();
	   printf("I : [%d][1] <-Malloc_FREE\n", i);
   t = malloc(atoi(av[1]));
show_alloc_mem();
	   printf("I : [%d][2] <-T_MALLOC\n", i);
   tmp = malloc(atoi(av[1]));
show_alloc_mem();
	   printf("I : [%d][3] <-TMP_MALLOC\n", i);
   realloc(tmp, atoi(av[2]));
show_alloc_mem();
	   printf("I : [%d][4] <-REALLOC TMP MAYBE MALLOC FOIRE\n", i);
   realloc(tmp, 2054);
show_alloc_mem();
	   printf("I : [%d][5] <-REALLOC TMP OK\n", i);
malloc(atoi(av[2]));
show_alloc_mem();
	   printf("I : [%d][6] <-MALLOC FOIRE\n", i);
malloc(atoi(av[1]));
show_alloc_mem();
	   printf("I : [%d][7] <-MALLOC OK\n", i);
free(t);
show_alloc_mem();
	   printf("I : [%d][8] <-FREE T\n", i);
i++;
}

i = 0;

   while(i < 3)
   {
	   printf("J : [%d][0] DEBUT\n", i);
   free(malloc(45));
show_alloc_mem();
	   printf("J : [%d][1] <-Malloc_FREE\n", i);
   t = malloc(145);
show_alloc_mem();
	   printf("J : [%d][2] <-T_MALLOC\n", i);
   tmp = malloc(222);
show_alloc_mem();
	   printf("J : [%d][3] <-TMP_MALLOC\n", i);
   realloc(tmp, 2);
show_alloc_mem();
	   printf("J : [%d][4] <-REALLOC TMP OK\n", i);
   realloc(tmp, 23);
show_alloc_mem();
	   printf("J : [%d][5] <-REALLOC TMP NOT FOUND\n", i);
malloc(255);
show_alloc_mem();
	   printf("J : [%d][6] <-MALLOC OK\n", i);
malloc(56);
show_alloc_mem();
	   printf("J : [%d][7] <-MALLOC OK\n", i);
free(t);
show_alloc_mem();
	   printf("J : [%d][8] <-FREE T\n", i);
i++;
}
*/
/*
 * RECCUPER LA DATE ET L HEURE PUIS L AFFICHER
 time_t timestamp = time( NULL );
 struct tm *timeInfos = localtime( & timestamp );

 printf( "Date and local time : %04d/%02d/%02d %02d:%02d:%02d\n",
 timeInfos->tm_year+1900, timeInfos->tm_mon+1, timeInfos->tm_mday,
 timeInfos->tm_hour, timeInfos->tm_min, timeInfos->tm_sec);
*/ 
//	return (0);
//}
//FAIRE UN MALLOC EN BOUCLE DE 222233333334
