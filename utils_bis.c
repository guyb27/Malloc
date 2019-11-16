/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils_bis.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/21 23:47:23 by gmadec       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/21 23:48:13 by gmadec      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "alloc.h"

void	ft_putendl(char const *s)
{
	int nl;

	nl = 10;
	ft_putstr(s);
	write(1, &nl, 1);
}

void	ft_putnbr(int nb)
{
	int dash;

	dash = 45;
	if (nb == -2147483648)
		ft_putstr("-2147483648");
	else
	{
		if (nb < 0)
		{
			write(1, &dash, 1);
			nb = -nb;
		}
		if (nb > 9)
		{
			ft_putnbr(nb / 10);
			ft_putnbr(nb % 10);
		}
		else if (nb <= 9)
		{
			nb += 48;
			write(1, &nb, 1);
		}
	}
}

void	ft_putnbr_base(size_t nb, int base)
{
	int p;

	if (nb > (size_t)base)
		ft_putnbr_base(nb / base, base);
	p = nb % 16 > 9 ? nb % 16 + 'A' - 10 : nb % 16 + '0';
	write(1, &p, 1);
}

void	ft_putnbr_base_nl(size_t nb, int base)
{
	int p;

	if (nb > (size_t)base)
		ft_putnbr_base(nb / base, base);
	p = nb % 16 > 9 ? nb % 16 + 'A' - 10 : nb % 16 + '0';
	if (nb == (size_t)base)
		p = 'F';
	write(1, &p, 1);
	if (nb <= (size_t)base)
		write(1, "\n", 1);
}
