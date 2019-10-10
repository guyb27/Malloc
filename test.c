/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   test.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/18 20:29:04 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/02 01:49:18 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
typedef enum			e_type
{
	TINY,
	SMALL,
	LARGE
}						t_type;

typedef struct			s_memory
{
	void				*block;
	struct t_type		type;
	struct s_memory		*prev;
	struct s_memory		*next;
}						t_memory;

t_memory	*create_memory_struct(nb)
{
	t_memory			*tmp;

	tmp  = (t_memory*)mmap(0, sizeof(t_memory*) + 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (nb <= 200)
		tmp->type = TINY;
	else if (nb <= 500)
		tmp->type = SMALL;
	else
		tmp->type = LARGE;
	tmp->block = (void*)mmap(0, nb + 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	return (tmp);
}

void		*alloc(t_memory **memory, int nb)
{
	int					found;
	t_memory			*tmp;

	tmp = *memory;
	found = 0;
	while (tmp && found == 0)
	{
		if (found == 0)
			tmp = tmp->next;
	}
	if (found == 0)
	{
		tmp  = (t_memory*)mmap(0, sizeof(t_memory*) + 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	}
	return (tmp->block);
}
*/
int		main(int ac, char **av)//MALLOC()
{/*
	int			i;
	int			nb;
	char		c;
	char		*str;
	t_memory	*memory;

	nb = atoi(av[2]);
	c = av[1][0];
	if (nb <= 200 && nb > 0)
		str = tiny(&memory, nb);
	else if (nb > 0 && nb <= 500)
		str = (char*)mmap(0, nb + 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	else if (nb > 0)
		str = (char*)mmap(0, nb + 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	i = 0;
	while (i < nb)
	{
		str[i] = c;
		i++;
	}
	str[i] = '\0';
	printf("%s\n", str);*/
	printf("%d", getpagesize());
	return (0);
}
