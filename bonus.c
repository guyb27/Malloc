/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bonus.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/21 23:44:31 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/21 23:49:50 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "alloc.h"

void	free_all(void)
{
	t_block *block;
	t_block *tmp;

	block = g_areas.tiny;
	while (block)
	{
		tmp = block->next;
		free((void*)block->start_address);
		block = tmp;
	}
	block = g_areas.small;
	while (block)
	{
		tmp = block->next;
		free((void*)block->start_address);
		block = tmp;
	}
	block = g_areas.large;
	while (block)
	{
		tmp = block->next;
		free((void*)block->start_address);
		block = tmp;
	}
}

void	*calloc(size_t nmemb, size_t size)
{
	void	*ret;

	ret = NULL;
	if (nmemb && size && (ret = malloc(nmemb * size)))
		ft_bzero(ret, malloc_size(ret));
	return (ret);
}

size_t	malloc_size(void *addr)
{
	t_block *block;

	block = g_areas.tiny;
	while (block)
	{
		if ((size_t)addr == block->start_address && block->octets_used)
			return (block->octets_used);
		block = block->next;
	}
	block = g_areas.small;
	while (block)
	{
		if ((size_t)addr == block->start_address && block->octets_used)
			return (block->octets_used);
		block = block->next;
	}
	block = g_areas.large;
	while (block)
	{
		if ((size_t)addr == block->start_address && block->octets_used)
			return (block->octets_used);
		block = block->next;
	}
	return (0);
}
