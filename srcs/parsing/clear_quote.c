/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crazyd <crazyd@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 17:54:19 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/21 08:49:19 by crazyd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_quote(char *str)
{
	int		count;
	size_t	i;
	size_t	k;

	count = 0;
	i = 0;
	while (str[i])
	{
		k = (in_quote((char *)str, i));
		if (k == ft_strlen(str))
		{
			count += 2;
			i = k;
			break;
		}
		else if (i == k)
			i++;
		else if (i < k)
		{
			count += 2;
			i = k;
		}
	}
	return (count);
}

static char	*cut_quote(char	*cmd)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	*dequoted;

	dequoted = malloc(sizeof(char) * (ft_strlen(cmd) - count_quote(cmd) + 1));
	i = 0;
	k = 0;
	while (cmd[i] != '\0')
	{
		j = in_quote(cmd, i);
		if (j > i)
		{
			i++;
			while (i < (j - 1) && cmd[i] != '\0')
				dequoted[k++] = cmd[i++];
		}
		else	
			dequoted[k++] = cmd[i++];
	}
	dequoted[k] = '\0';
	free(cmd);
	return(dequoted);
}

void	clear_quote(t_separate *list)
{
	size_t	i;

	i = 0;
	while (list->cmds[i])
	{
		if (ft_strchr(list->cmds[i], '\'') || ft_strchr(list->cmds[i], '\"'))
			list->cmds[i] = cut_quote(list->cmds[i]);
		i++;
	}
	if (list->in)
	{
		if (ft_strchr(list->in, '\'') || ft_strchr(list->in, '\"'))
			list->in = cut_quote(list->in);
	}
	if (list->out)
	{
		if (ft_strchr(list->out, '\'') || ft_strchr(list->out, '\"'))
			list->out = cut_quote(list->out);
	}
}