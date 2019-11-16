/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/21 23:45:18 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/22 16:07:02 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "alloc.h"

static void		*new_alloc(t_block **original_block, size_t size)
{
	t_block		*block;
	t_block		*new_block;
	size_t		new_size;
	size_t		t_block_size;

	t_block_size = next_hexa_base(sizeof(t_block));
	block = *original_block;
	while (block->next)
		block = block->next;
	new_size = next_page_size((size * 100)
			+ (t_block_size * 100), g_areas.page_size);
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

static void		same_zone(t_block **new_block, t_block **block,\
		size_t memory_rest)
{
	*new_block = (void*)((*block)->start_address + (*block)->octets_used);
	(*new_block)->next = (*block)->next ? (*block)->next : NULL;
	(*new_block)->prev = *block;
	(*block)->next = *new_block;
	(*new_block)->start_address = (size_t)*new_block
		+ next_hexa_base(sizeof(t_block));
	(*new_block)->octets_used = 0;
	(*new_block)->mmap_number = (*block)->mmap_number;
	(*new_block)->octets_available = memory_rest
		- next_hexa_base(sizeof(t_block));
}

static void		*attrib_memory(t_block **original_block, size_t size)
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
		same_zone(&new_block, &block, memory_rest);
	else if (memory_rest <= next_hexa_base(sizeof(t_block)))
	{
		block->octets_used += memory_rest;
		if (!block->next)
			new_alloc(&block, size <= TINY_MAX ? TINY_MAX : SMALL_MAX);
	}
	return ((void*)block->start_address);
}

static void		*search_block(t_block **original_block, size_t size)
{
	t_block		*block;

	block = *original_block;
	while (block)
	{
		if ((!block->octets_used && block->octets_available >= size))
			return (attrib_memory(&block, size));
		else if (!block->next)
			new_alloc(&block, size <= TINY_MAX ? TINY_MAX : SMALL_MAX);
		else
			block = block->next;
	}
	return (NULL);
}

void			*malloc(size_t size)
{
	void		*ret;

	size += size == 0 ? 1 : 0;
	size = next_hexa_base(size);
	ret = NULL;
	if (!g_areas.tiny)
		init_areas();
	if (size <= SMALL_MAX)
		ret = search_block(size <= TINY_MAX ?
				&g_areas.tiny : &g_areas.small, size);
	else if (size)
		ret = new_large(size);
	return (ret);
}
