/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/21 23:43:15 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/22 00:01:43 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "alloc.h"

static int		free_large(t_block **original_block)
{
	t_block		*large;
	int			ret;

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

t_block			*used_search(void *ptr, t_block **original_block)
{
	t_block		*block;

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

void			free(void *ptr)
{
	t_block		*block;

	if (ptr != MAP_FAILED && ptr)
	{
		if ((block = used_search(ptr, &g_areas.tiny)) ||
				(block = used_search(ptr, &g_areas.small)))
			free_tiny_and_small(&block);
		else if ((block = used_search(ptr, &g_areas.large)))
			free_large(&block);
	}
}
