/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   alloc.h                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 16:03:51 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/22 16:05:40 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef ALLOC_H
# define ALLOC_H

# include <unistd.h>
# include <sys/mman.h>

# define TINY_MAX 256
# define SMALL_MAX 1024

typedef struct			s_block
{
	size_t				start_address;
	size_t				octets_used;
	size_t				octets_available;
	size_t				mmap_number;
	struct s_block		*prev;
	struct s_block		*next;
}						t_block;

typedef struct			s_areas
{
	t_block				*tiny;
	t_block				*small;
	t_block				*large;
	size_t				total_use;
	int					page_size;
	size_t				nb_mmap;
}						t_areas;

t_areas					g_areas;

void					*realloc(void *ptr, size_t size);
void					*malloc(size_t size);
void					free_all();
void					*calloc(size_t nmemb, size_t size);
size_t					malloc_size(void *addr);
void					free(void *ptr);
void					show_alloc_mem(void);
size_t					next_hexa_base(size_t size);
void					*ft_memcpy(void *dst, const void *src, size_t n);
t_block					*used_search(void *ptr, t_block **original_block);
void					ft_putstr(char const *s);
void					ft_putnbr(int nb);
void					ft_bzero(void *s, size_t n);
int						next_page_size(size_t size, size_t page_size);
void					init_areas(void);
void					ft_putnbr_base(size_t nb, int base);
void					ft_putnbr_base_nl(size_t nb, int base);
void					*new_large(size_t size);
void					free_tiny_and_small(t_block **original_block);

#endif
