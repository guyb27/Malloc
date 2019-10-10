/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   test.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/18 20:29:04 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/06 08:12:02 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TINY_MAX 256
#define SMALL_MAX 512

typedef struct			s_block
{
	size_t				start_address;//I : [√]
	size_t				octets_used;//I : [√]
	size_t				octets_available;

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
	t_historic			historic[100];
}						t_areas;

t_areas			g_areas;

int				g_i = 0;
int				g_attrib_mem = 0;
int				g_next = 0;
int				g_new_alloc = 0;

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
}

void	init_areas()
{
	g_areas.page_size = getpagesize();
	size_t tiny_size = (TINY_MAX * 100) + (next_hexa_base(sizeof(t_block)) * 101);
	size_t small_size = (SMALL_MAX * 100) + (next_hexa_base(sizeof(t_block)) * 101);
	size_t used = 0;
	size_t	total_size;

	if (tiny_size % g_areas.page_size)
		tiny_size += g_areas.page_size - (tiny_size % g_areas.page_size);
	if (small_size % g_areas.page_size)
		small_size += g_areas.page_size - (small_size % g_areas.page_size);
	//TINY
	g_areas.tiny = mmap(0, next_hexa_base(sizeof(t_block)) + small_size +
			tiny_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	g_areas.tiny->octets_used = 0;
	g_areas.tiny->start_address = (size_t)g_areas.tiny + next_hexa_base(sizeof(t_block));
	g_areas.tiny->octets_available = tiny_size - next_hexa_base(sizeof(t_block));
	g_areas.tiny->octets_used = 0;
	g_areas.tiny->prev = NULL;
	g_areas.tiny->next = NULL;

	//SMALL
	g_areas.small = (void*)((size_t)g_areas.tiny + tiny_size);
	g_areas.small->octets_used = 0;
	g_areas.small->start_address = (size_t)g_areas.small + next_hexa_base(sizeof(t_block));
	g_areas.small->octets_available = small_size - next_hexa_base(sizeof(t_block));
	g_areas.small->octets_used = 0;
	g_areas.small->prev = NULL;
	g_areas.small->next = NULL;

	g_areas.total_use = 0;

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

	block = *original_block;
	//dprintf(2, "NEW ALLOC[%d]\n", g_i);
	new_size = next_hexa_base((size * 100) + (next_hexa_base(sizeof(t_block)) * 100));
	new_block = mmap(0, new_size,
			PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	//printf("END:[%zu] : NEXT:[%zu]\n", (size_t)block->start_address + block->octets_available, (size_t)new_block);
	if ((size_t)block->start_address + block->octets_available + block->octets_used == (size_t)new_block)
	{
		printf("END:[%zu] : NEXT:[%zu]\n", (size_t)block->start_address + block->octets_available, (size_t)new_block);
		block->octets_available += new_size;
		printf("================NEW ALLOC AVEC ADDRESS QUI SE SUIVENT============\n");
	}
	else
	{
		block->next = new_block;
		new_block->prev = block;
		new_block->next = NULL;
		new_block->octets_used = 0;
		new_block->octets_available = new_size - next_hexa_base(sizeof(t_block));
		//		printf("0=-=-=-=-=-=-=-NEW_BLOCK=-=-=-=-=-=-=-=-\n");
		//		printf("AVAILABLE : [%zi], USED:[%zi]\n", new_block->octets_available, new_block->octets_used);
		new_block->start_address = (size_t)new_block + next_hexa_base(sizeof(t_block));
		//		printf("1=-=-=-=-=-=-=-NEW_BLOCK=-=-=-=-=-=-=-=-\n");
		//		printf("0=-=-=-=-=-=-=-BLOCK=-=-=-=-=-=-=-=-\n");
		//		printf("AVAILABLE : [%zi], USED:[%zi]\n", block->octets_available, block->octets_used);
		new_block->start_address = (size_t)block + next_hexa_base(sizeof(t_block));
		//		printf("1=-=-=-=-=-=-=-BLOCK=-=-=-=-=-=-=-=-\n");
	}
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
	if (memory_rest >= next_hexa_base(sizeof(t_block)))
	{
		block->octets_available = 0;
		new_block = (void*)(block->start_address + block->octets_used);
		new_block->next = block->next;
		new_block->prev = block;
		block->next = new_block;
		new_block->start_address = (size_t)new_block + next_hexa_base(sizeof(t_block));
		new_block->octets_used = 0;
		new_block->octets_available = memory_rest - next_hexa_base(sizeof(t_block));
		printf("00OCTETS_AVAILABLE : [%zi]\n", new_block->octets_available);
	}
	else if (memory_rest < next_hexa_base(sizeof(t_block)))
	{
		block->octets_used += memory_rest;
		new_block = new_alloc(&block, size <= TINY_MAX ? TINY_MAX : SMALL_MAX);
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

	size = size + next_hexa_base(sizeof(t_block));
	tmp_block = g_areas.large;
	if (size % g_areas.page_size)
		size += g_areas.page_size - (size % g_areas.page_size);	
	if(!((void*)(new_large = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON |
						MAP_PRIVATE, -1, 0)) == MAP_FAILED))
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
			new_large->prev = NULL;
			g_areas.large = new_large;
		}
		new_large->start_address = (size_t)new_large + next_hexa_base(sizeof(t_block));
		new_large->octets_available = 0;
		new_large->octets_used = size - next_hexa_base(sizeof(t_block));
		new_large->next = NULL;
		g_areas.total_use += size - next_hexa_base(sizeof(t_block));
	}
	else
		return (new_large);
	return ((void*)new_large->start_address);
}

void	*ft_malloc(size_t size)
{
	int						page_size;
	t_block					*block;
	void					*ret;

	int i = 0;

	g_attrib_mem = 0;
	g_next = 0;
	g_new_alloc = 0;
	size = next_hexa_base(size);
	ret = NULL;
	if (!g_areas.tiny)
		init_areas();
	if (size <= SMALL_MAX && size > 0)
	{
		ret = search_block(size <= TINY_MAX ? &g_areas.tiny : &g_areas.small, size);
		block = g_areas.tiny;
		while (block)
		{
			printf("[%d], ADDR:[%zi], start : [%zi], used:[%zi], available:[%zi], end:[%zi]\n",
				i, (size_t)block, block->start_address, block->octets_used, block->octets_available,
				block->start_address + block->octets_used + block->octets_available);
			i++;
			block = block->next;
		}
	}
	else if (size > 0)
		ret = new_large(size);
	block = g_areas.tiny;
	printf("NB_MAILLON:[%d], ATTRIB_MEM:[%d], NEXT:[%d], NEW_ALLOC:[%d], TOTAL_USE:[%zi]\n", i, g_attrib_mem, g_next,g_new_alloc, g_areas.total_use);
	return (ret);
}

int		free_large(void *ptr, t_block *large)
{
	g_areas.total_use -= next_hexa_base(sizeof(large)) + large->octets_used;
	if (large->next)
		large->next->prev = large->prev;
	if (large->prev)
		large->prev->next = large->next;
	else if (!large->next)
	{
		g_areas.large = NULL;
		return (munmap(g_areas.large, large->octets_used +
			next_hexa_base(sizeof(t_block))));
	}
	return (munmap(large, large->octets_used +
		next_hexa_base(sizeof(t_block))));
}

void	free_tiny_and_small(void *ptr, t_block *block)
{
	g_areas.total_use -= block->octets_used + block->octets_available;
	printf("TOTAL_USE: [%zi]\n", g_areas.total_use);
	if (block->prev && block->prev->start_address + block->octets_used ==
		(size_t)block && block->next->octets_available)
	{
		block->prev->octets_available += block->octets_used +
			next_hexa_base(sizeof(t_block));
		block->prev->next = block->next;
		if (block->next)
			block->next->prev = block->prev;
		block = block->prev;
	}
	if (block->next && block->start_address + block->octets_used +
	block->octets_available == (size_t)block->next && block->octets_available)
	{
		block->octets_available = block->octets_available + block->octets_used;
		block->octets_available += block->next->octets_available +
			next_hexa_base(sizeof(t_block));
		block->next = block->next->next;
		if (block->next->next)
			block->next->next->prev = block;
	}
	block->octets_used = 0;
}

/*
 *
 *[1]TOTAL A CHANGER
 *[2]free tout les maillons, meme quand on ne defragmente pas
*/
t_block	*free_search(void *ptr, t_block *block)
{
	while (block)
	{
		if (block->start_address == (size_t)ptr && block->octets_used > 0)
			return (block);
		else
			block = block->next;
	}
	return (NULL);
}

int		ft_free(void *ptr)
{
	int found = 0;
	t_block		*block;
	//NE PAS OUBLIER SI C EST UN MAILLON LIBRE DE NE PAS LE FREE ! ! !
	if (ptr != MAP_FAILED)
	{
		if ((block = free_search(ptr, g_areas.tiny)) ||
			(block = free_search(ptr, g_areas.small)))
			free_tiny_and_small(ptr, block);
		else if ((block = free_search(ptr, g_areas.large)))
			return(free_large(ptr, block));
	}
	return (0);
}

int		main(int ac, char **av)
{
	int i = 0, j = 0;

	while(i < 300)
	{
		ft_free(ft_malloc(atoi(av[1])));
		g_i++;
		ft_free(ft_malloc(atoi(av[2])));
		//ft_malloc(atoi(av[2]));
		i++;
		g_i++;
	}
		show_alloc_mem();
	/*
	 * RECCUPER LA DATE ET L HEURE PUIS L AFFICHER
	 time_t timestamp = time( NULL );
	 struct tm *timeInfos = localtime( & timestamp );

	 printf( "Date and local time : %04d/%02d/%02d %02d:%02d:%02d\n",
	 timeInfos->tm_year+1900, timeInfos->tm_mon+1, timeInfos->tm_mday,
	 timeInfos->tm_hour, timeInfos->tm_min, timeInfos->tm_sec);
	 */
	return (0);
}
//FAIRE UN MALLOC EN BOUCLE DE 222233333334
