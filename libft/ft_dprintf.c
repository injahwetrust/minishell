/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 16:15:03 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/08 16:29:01 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_printchar_fd(int fd, char c, int *len)
{
	write(fd, &c, 1);
	*len += 1;
}

void	ft_putstr_fd(int fd, const char *str, int *len)
{
	int	i;

	i = 0;
	if (!str)
	{
		*len += write (fd, "(null)", 6);
		return ;
	}
	while (str[i])
	{
		write (fd, &str[i], 1);
		i++;
		*len += 1;
	}
}

void	ft_putnbr_base_fd(int fd, long long nbr, char *base, int *len)
{
	int	size;

	size = 0;
	while (base[size])
		size++;
	if (nbr < 0)
	{
		ft_printchar_fd(fd, '-', len);
		nbr = -nbr;
	}
	if (nbr / size)
		ft_putnbr_base_fd(fd, nbr / size, base, len);
	ft_printchar_fd(fd, base[nbr % size], len);
}

int	format_fd(int fd, char c, va_list ap)
{
	int	len;

	len = 0;
	if (c == '%')
		len += write (fd, "%", 1);
	else if (c == 'c')
		ft_printchar_fd(fd, va_arg(ap, int), &len);
	else if (c == 's')
		ft_putstr_fd(fd, va_arg(ap, char *), &len);
	else if (c == 'i' || c == 'd')
		ft_putnbr_base_fd(fd, va_arg(ap, int), "0123456789", &len);
	else if (c == 'u')
		ft_putnbr_base_fd(fd, va_arg(ap, unsigned int), "0123456789", &len);
	else if (c == 'x')
		ft_putnbr_base_fd(fd, va_arg(ap, unsigned int), "0123456789abcdef", &len);
	else if (c == 'X')
		ft_putnbr_base_fd(fd, va_arg(ap, unsigned int), "0123456789ABCDEF", &len);
	return (len);
}

int	ft_dprintf(int fd, const char *str, ...)
{
	int		i;
	int		len;
	va_list	ap;

	i = 0;
	len = 0;
	va_start(ap, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			len += format_fd(fd, str[i + 1], ap);
			i++;
		}
		else
		{
			write (fd, &str[i], 1);
			len++;
		}
		i++;
	}
	va_end(ap);
	return (len);
}