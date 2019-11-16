/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   init_and_print.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 00:06:46 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/22 15:59:35 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "alloc.h"

static void		init_small(size_t start, size_t available)
{
	g_areas.small->octets_used = 0;
	g_areas.small->start_address = start;
	g_areas.small->octets_available = available;
	g_areas.small->octets_used = 0;
	g_areas.small->prev = NULL;
	g_areas.small->next = NULL;
	g_areas.small->mmap_number = 1;
	g_areas.large = NULL;
	g_areas.total_use = 0;
	g_areas.nb_mmap = 1;
}

void			init_areas(void)
{
	size_t		block_size;
	size_t		tiny_size;
	size_t		small_size;
	size_t		t;

	g_areas.page_size = getpagesize();
	block_size = next_hexa_base(sizeof(t_block));
	tiny_size = (256 * 100) + (block_size * 101);
	small_size = (1024 * 100) + (block_size * 101);
	if (tiny_size % g_areas.page_size)
		tiny_size += g_areas.page_size - (tiny_size % g_areas.page_size);
	if (small_size % g_areas.page_size)
		small_size += g_areas.page_size - (small_size % g_areas.page_size);
	t = small_size + tiny_size;
	g_areas.tiny = mmap(0, t, 0x01 | 0x02, 0x1000 | 0x0002, -1, 0);
	g_areas.tiny->octets_used = 0;
	g_areas.tiny->start_address = (size_t)g_areas.tiny + block_size;
	g_areas.tiny->octets_available = tiny_size - block_size;
	g_areas.tiny->octets_used = 0;
	g_areas.tiny->prev = NULL;
	g_areas.tiny->next = NULL;
	g_areas.tiny->mmap_number = 0;
	g_areas.small = (void*)((size_t)g_areas.tiny + tiny_size);
	init_small((size_t)g_areas.small + block_size, small_size - block_size);
}

static void		ft_print_maillon(t_block *original_block)
{
	t_block		*block;

	block = original_block;
	ft_putstr(block ? "0x" : "0x");
	ft_putnbr_base((size_t)block, 16);
	write(1, "\n", 1);
	while (block)
	{
		if (block->octets_used)
		{
			ft_putstr("0x");
			ft_putnbr_base(block->start_address, 16);
			ft_putstr(" - 0x");
			ft_putnbr_base(block->start_address + block->octets_used, 16);
			ft_putstr(" : ");
			ft_putnbr(block->octets_used);
			ft_putstr(" octets\n");
		}
		block = block->next;
	}
}

void			show_alloc_mem(void)
{
	ft_putstr("TINY : ");
	ft_print_maillon(g_areas.tiny);
	ft_putstr("SMALL : ");
	ft_print_maillon(g_areas.small);
	ft_putstr("LARGE : ");
	ft_print_maillon(g_areas.large);
}
