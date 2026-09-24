/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salegari <salegari@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 19:35:25 by salegari          #+#    #+#             */
/*   Updated: 2026/09/24 20:07:53 by salegari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strchr(char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}

int	ft_strlcpy(char *dest, char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] && i < size -1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen(src));
}

int	ft_strlcat(char *dest, char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;
	size_t	j;

	d_len = 0;
	s_len = ft_strlen(src);
	while (d_len < size && dest[d_len])
		d_len++;
	if (d_len == size)
		return (size + s_len);
	j = 0;
	while (src[j] && d_len + j + 1 < size)
	{
		dest[d_len + j] = src[j];
		j++;
	}
	dest[d_len + j] = '\0';
	return (d_len + s_len);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	res = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!res)
		return (NULL);
	ft_strlcpy(res, (char *)s1, ft_strlen(s1) +1);
	ft_strlcat(res, (char *)s2, ft_strlen(s1) + ft_strlen(s2) +1);
	return (res);
}
