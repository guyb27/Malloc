/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/21 23:47:13 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/22 00:00:45 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "alloc.h"

void			ft_bzero(void *s, size_t n)
{
	int			i;

	i = 0;
	while (n--)
		((unsigned char *)s)[i++] = '\0';
}

void			*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*dest;
	int			i;

	dest = dst;
	i = -1;
	while (++i < (int)n)
		dest[i] = ((char*)src)[i];
	return (dest);
}

size_t			next_hexa_base(size_t size)
{
	return (size % 16 ? size + 16 - (size % 16) : size);
}

int				next_page_size(size_t size, size_t page_size)
{
	return (size % page_size ? size + page_size - (size % page_size) : size);
}

void			ft_putstr(char const *s)
{
	int			i;

	if (s)
	{
		i = -1;
		while (s && s[++i])
			;
		write(1, s, i);
	}
}
