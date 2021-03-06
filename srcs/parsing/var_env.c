/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crazyd <crazyd@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 15:24:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/21 08:43:28 by crazyd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_by_code_var_env(char *cmd)
{
	size_t	size_var;
	size_t	new_size;
	size_t	i;
	size_t	j;
	size_t	m;
	char	*tmp;
	char	*env;

	env = ft_itoa(g_global.return_code);
	size_var = ft_strlen(env) - 1;
	new_size = size_var + ft_strlen(cmd);
	tmp = malloc(sizeof(char) * new_size);
	j = 0;
	i = 0;
	m = 0;
	while (cmd[j])
	{
		while (cmd[j] && cmd[j] != '$')
			tmp[i++] = cmd[j++];
		j += 2;
		while (env[m])
			tmp[i++] = env[m++];
		while (cmd[j] && cmd[j] != '$')
			tmp[i++] = cmd[j++];
	}
	tmp[i] = '\0';
	free(cmd);
	free(env);
	return (tmp);
}

static char	*replace_var_env(char *cmd, size_t i, size_t j)
{
	size_t	size_var;
	size_t	new_size;
	size_t	k;
	size_t	l;
	size_t	m;
	char	*tmp;
	char	*env;

	env = ft_strdup(ft_get_var_env((cmd + j), (i - j)));
	size_var = ft_strlen(env) - 1;
	new_size = size_var + ft_strlen(cmd) - (i - j) + 1;
	tmp = malloc(sizeof(char) * new_size);
	l = 0;
	k = 0;
	m = 0;
	while (cmd[l])
	{
		while (cmd[l] && cmd[l] != '$')
			tmp[k++] = cmd[l++];
		l = i;
		while (env[m])
			tmp[k++] = env[m++];
		while (cmd[l])
			tmp[k++] = cmd[l++];
	}
	tmp[k] = '\0';
	free(cmd);
	free(env);
	return (tmp);
}

int	counterr(int err)
{
	int	count;

	count = 1;
	while (err > 9)
	{
		count++;
		err /= 10;
	}
	return (count);
}

static int	check_var_env(t_separate *list, size_t check)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = check;
	while (list->str[i])
	{
		j = (in_quote(list->str, i));
		if (j == ft_strlen(list->str))
			break;
		else if (i < j)
			i = j;
		if (list->str[i] == '$')
		{
			j = ++i;
			while (list->str[i] && ft_isalnum(list->str[i]))
				i++;
			if (ft_get_var_env((list->str + j), (i - j)))
			{
				k = ft_strlen(ft_get_var_env((list->str + j), (i - j)));
				list->str = replace_var_env(list->str, i, j);
				return (j + k - 1);
			}
			else if (list->str[i] && list->str[i] == '?')
			{
				list->str = replace_by_code_var_env(list->str);
				return(j + counterr(g_global.return_code) - 1);
			}
		}
		i++;
	}
	return (0);
}

void	do_var_env(t_separate *list)
{
	size_t	checkpoint;

	checkpoint = 0;
	if (ft_strchr(list->str, '$'))
	{
		while (1)
		{
			checkpoint = check_var_env(list, checkpoint);
			if (!checkpoint)
				break;
		}
	}
}
