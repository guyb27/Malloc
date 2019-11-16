/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   realloc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/21 23:47:04 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/21 23:57:43 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "alloc.h"

static int		is_concat_struct(t_block *block, size_t size)
{
	int			same_little;

	same_little = 0;
	if (block->octets_used <= TINY_MAX)
		same_little++;
	if (size <= TINY_MAX)
		same_little++;
	if (size <= SMALL_MAX && block->octets_used <= SMALL_MAX && !same_little)
		same_little = 2;
	if (same_little == 2 && (block->octets_used >= size ||
		(block->next && block->next->octets_available +
		block->octets_used + next_hexa_base(sizeof(t_block))
		>= size)))
		return (1);
	else
		return (0);
}

void			*realloc(void *ptr, size_t size)
{
	t_block		*block;
	t_block		*new_block;
	size_t		new_size;

	if (ptr && size > 0 && (size = next_hexa_base(size)))
	{
		if ((block = used_search(ptr, &g_areas.tiny)) ||
				(block = used_search(ptr, &g_areas.small)) ||
				(block = used_search(ptr, &g_areas.large)))
		{
			new_size = block->octets_used <= size ? block->octets_used : size;
			if ((new_block = malloc(size)) != NULL)
			{
				ft_memcpy(new_block, (void*)block->start_address, new_size);
				free((void*)block->start_address);
				return (new_block);
			}
		}
	}
	else if (ptr == NULL)
		return (malloc(next_hexa_base(size)));
	else if (size == 0 && ptr)
		free(ptr);
	return (NULL);
}
