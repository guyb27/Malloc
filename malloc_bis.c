/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc_bis.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/21 23:46:29 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/21 23:49:12 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "alloc.h"

static void	get_maillon(t_block **tmp_block, t_block **new_large)
{
	while (*tmp_block && (*tmp_block)->next)
		*tmp_block = (*tmp_block)->next;
	if (*tmp_block)
	{
		(*tmp_block)->next = *new_large;
		(*new_large)->prev = *tmp_block;
	}
	else
	{
		g_areas.large = *new_large;
		(*new_large)->prev = NULL;
	}
}

void		*new_large(size_t size)
{
	t_block	*new_large;
	t_block	*tmp_block;
	size_t	t_block_size;

	t_block_size = next_hexa_base(sizeof(t_block));
	size = size + t_block_size;
	tmp_block = g_areas.large;
	if (size % g_areas.page_size)
		size += g_areas.page_size - (size % g_areas.page_size);
	if (!((void*)(new_large = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON |
						MAP_PRIVATE, -1, 0)) == MAP_FAILED))
	{
		get_maillon(&tmp_block, &new_large);
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
