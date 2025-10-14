/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcasecmp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtravez <mtravez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 14:48:09 by mtravez           #+#    #+#             */
/*   Updated: 2023/04/17 12:10:04 by mtravez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

//This function compares two strings and returns the difference between
//the first two non matching, case insensitive
int	ft_strcasecmp(const char *s1, const char *s2)
{
	int				i;
	unsigned char	*temps1;
	unsigned char	*temps2;

	i = 0;
	temps1 = (unsigned char *) s1;
	temps2 = (unsigned char *) s2;
	while (ft_tolower(temps1[i]) && (ft_tolower(temps1[i]) == ft_tolower(temps2[i])))
		i++;
	return (ft_tolower(temps1[i]) - ft_tolower(temps2[i]));
}