/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free_bis.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/21 23:44:53 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/22 16:24:01 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "alloc.h"

static void		ft_free_space(t_block **tmp)
{
	if ((*tmp)->next)
		(*tmp)->next->prev = (*tmp)->prev;
	(*tmp)->prev->next = (*tmp)->next;
	munmap(*tmp, (*tmp)->octets_available +
		next_hexa_base(sizeof(t_block)));
}

static void		ft_search_and_free_little(size_t size)
{
	t_block		*block;
	t_block		*tmp;
	int			found;

	found = 0;
	block = size == TINY_MAX ? g_areas.tiny : g_areas.small;
	while (block)
	{
		if (block->octets_available &&
		(!block->prev || block->prev->mmap_number != block->mmap_number) &&
		(!block->next || block->mmap_number != block->next->mmap_number))
		{
			tmp = block;
			block = block->next;
			if (found == 0)
				found++;
			else
				ft_free_space(&tmp);
		}
		else
			block = block ? block->next : block;
	}
}

static void		last_is_free_friend(t_block **block)
{
	if ((*block)->next && (*block)->mmap_number == (*block)->next->mmap_number
			&& (*block)->next->octets_available && !(*block)->next->octets_used)
	{
		(*block)->octets_available += (*block)->next->octets_available
			+ next_hexa_base(sizeof(t_block));
		if ((*block)->next->next)
		{
			(*block)->next->next->prev = *block;
			(*block)->next = (*block)->next->next;
		}
		else
			(*block)->next = NULL;
	}
}

void			free_tiny_and_small(t_block **original_block)
{
	t_block		*block;
	size_t		size;

	block = *original_block;
	g_areas.total_use -= block->octets_used;
	block->octets_available = block->octets_used;
	size = block->octets_used <= TINY_MAX ? TINY_MAX : SMALL_MAX;
	block->octets_used = 0;
	last_is_free_friend(&block);
	if (block->prev && block->prev->octets_available &&
	block->prev->mmap_number == block->mmap_number && !block->octets_available)
	{
		block->prev->octets_available += block->octets_available +
			next_hexa_base(sizeof(t_block));
		block->prev->next = block->next;
		if (block->next)
			block->next->prev = block->prev;
	}
	ft_search_and_free_little(size);
}
