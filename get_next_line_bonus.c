/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salegari <salegari@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 19:33:47 by salegari          #+#    #+#             */
/*   Updated: 2026/10/03 18:12:40 by salegari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	size_t	s_len;
	char	*res;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	ft_strlcpy(res, s + start, len + 1);
	return (res);
}

char	*ft_extract(char *line, int flag)
{
	int		i;
	char	*res;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (flag == 0)
	{
		if (line[i] == '\n')
			res = ft_substr(line, 0, i + 1);
		else
			res = ft_substr(line, 0, i);
		return (res);
	}
	if (flag == 1)
	{
		if (!line[i])
			return (free(line), NULL);
		res = ft_substr(line, i + 1, ft_strlen(line) - (i + 1));
		return (free(line), res);
	}
	return (NULL);
}

static char	*ft_read_concat(int fd, char *line, char *buff)
{
	int		r;
	char	*t;

	r = 1;
	while (!ft_strchr(line, '\n') && r > 0)
	{
		r = read(fd, buff, BUFFER_SIZE);
		if (r < 0)
			return (free(line), NULL);
		if (r == 0)
			break ;
		buff[r] = '\0';
		t = line;
		line = ft_strjoin(line, buff);
		free(t);
		if (!line)
			return (NULL);
	}
	return (line);
}

char	*ft_concat(int fd, char *line)
{
	char	*buff;

	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (free(line), NULL);
	if (!line)
	{
		line = ft_strdup("");
		if (!line)
			return (free(buff), NULL);
	}
	line = ft_read_concat(fd, line, buff);
	free(buff);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*line[MAX_FD];
	char		*res;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	line[fd] = ft_concat(fd, line[fd]);
	if (!line[fd] || line[fd][0] == '\0')
	{
		free(line[fd]);
		line[fd] = NULL;
		return (NULL);
	}
	res = ft_extract(line[fd], 0);
	if (!res)
	{
		free(line[fd]);
		line[fd] = NULL;
		return (NULL);
	}
	line[fd] = ft_extract(line[fd], 1);
	return (res);
}

// #include <fcntl.h>
// #include <stdio.h>
//
// int	main(int argc, char **argv)
// {
// 	char	*line;
// 	int		fd;
//
// 	if (argc == 2)
// 	{
// 		fd = open(argv[1], O_RDONLY);
// 		if (fd == -1)
// 			return (1);
// 	}
// 	else
// 		fd = 0;
// 	line = get_next_line(fd);
// 	while (line)
// 	{
// 		printf("%s", line);
// 		free(line);
// 		line = get_next_line(fd);
// 	}
// 	if (fd != 0)
// 		close(fd);
// 	return (0);
// }
