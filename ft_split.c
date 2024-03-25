/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchieric <fchieric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:39:46 by fabi              #+#    #+#             */
/*   Updated: 2024/03/13 15:47:39 by fchieric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	wrdcount(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] != c && s[i] != '\0')
				i++;
			if (s[i] == '\0')
				return (count);
		}
		i++;
	}
	return (count);
}

static void	ft_strcpy(const char *s, char c, char *res, int i)
{
	int	j;

	j = 0;
	while (s[i] != '\0' && s[i] == c)
		i++;
	while (s[i] != '\0' && s[i] != c)
		res[j++] = s[i++];
	res[j] = '\0';
}

static char	*stralloc(char const *s, char c, int *i)
{
	char	*res;
	int		a;

	a = *i;
	res = NULL;
	while (s[*i] != '\0')
	{
		if (s[*i] != c)
		{
			while (s[*i] != c && s[*i] != '\0')
				*i += 1;
			res = malloc(sizeof(char) * (*i - a + 1));
			if (!res)
				return (NULL);
			break ;
		}
		*i += 1;
	}
	if (res)
		ft_strcpy(s, c, res, a);
	return (res);
}

char	**ft_split(char const *s, char c)
{
	int		k;
	int		i;
	int		pos;
	char	**res;

	if (s == NULL)
		return (NULL);
	i = 0;
	pos = 0;
	k = wrdcount(s, c);
	res = malloc(sizeof(char *) * (k + 1));
	if (res == NULL)
		return (NULL);
	res[k] = NULL;
	while (i < k)
	{
		res[i] = stralloc(s, c, &pos);
		if (!res[i])
			return (NULL);
		i++;
	}
	return (res);
}
