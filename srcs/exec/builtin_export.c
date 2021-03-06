/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crazyd <crazyd@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 11:31:30 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/21 09:45:30 by crazyd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	num_to_export(char	**cmd)
{
	size_t	i;
	size_t	count;

	i = 1;
	count = 0;
	while (cmd[i])
	{
		if (ft_isalpha(cmd[i][0]) && !check_double_env(cmd[i], ft_strlen_equal(cmd[i])))
				count++;
		i++;
	}
	return (count);
}

void	update_double(char	**cmd)
{
	size_t	i;
	size_t	j;

	j = 0;
	while (g_global.env[j])
	{
		i = 1;
		while (cmd[i])
		{
			if (!ft_strncmp(g_global.env[j], cmd[i], ft_strlen_equal(cmd[i]))
				&& ft_strlen_equal(g_global.env[j]) == ft_strlen_equal(cmd[i])
					&& cmd[i][ft_strlen_equal(cmd[i]) + 1] == '='
						&& cmd[i][ft_strlen_equal(cmd[i]) + 2] != '\0'
							&& cmd[i][ft_strlen_equal(cmd[i])] == '+')
			{
				ft_join_more(&g_global.env[j], (cmd[i] + ft_strlen_equal(cmd[i]) + 2));
			}
			else if (!ft_strncmp(g_global.env[j], cmd[i], ft_strlen_equal(cmd[i]))
				&& ft_strlen_equal(g_global.env[j]) == ft_strlen_equal(cmd[i])
					&& cmd[i][ft_strlen_equal(cmd[i])] == '='
						&& cmd[i][ft_strlen_equal(cmd[i]) + 1] != '\0')
			{
				free(g_global.env[j]);
				g_global.env[j] = ft_strdup(cmd[i]);
			}
			i++;
		}
		j++;
	}
}

char	*concat_new(char *cmd)
{
	size_t	i;
	size_t	j;
	char	*new;
	int		plus;

	i = 0;
	j = 0;
	plus = 0;
	new = malloc(sizeof(char) * ft_strlen(cmd));
	while (cmd[i])
	{
		if (cmd[i] == '+' && plus == 0)
		{
			plus++;
			i++;
		}
		new[j++] = cmd[i++];
	}
	new[j] = '\0';
	return (new);
}

int	builtin_export(t_separate *list)
{
	size_t	i;
	size_t	j;
	size_t	size;
	int		err;
	bool	update;
	char	**new_env;

	err = 0;
	update = true;
	if (!list->cmds[1])
		builtin_env(true, list);
	else
	{
		i = 0;
		if (g_global.env[0] == NULL)
			size = num_to_export(list->cmds) + 1;
		else
			size = num_to_export(list->cmds) + ft_array_size(g_global.env) + 1;
		new_env = malloc(sizeof(char *) * size);
		while (g_global.env[i])
		{
			new_env[i] = ft_strdup(g_global.env[i]);
			i++;
		}
		j = 1;
		while (list->cmds[j])
		{
			if (!ft_isalpha(list->cmds[j][0]))
				err = errmsg("export: ", list->cmds[j], ": not a valid identifier");	
			else if (!check_double_env(list->cmds[j], ft_strlen_equal(list->cmds[j])))
			{
				if (!ft_strnstr(list->cmds[j], "+", ft_strlen_equal(list->cmds[j]) + 1))
					new_env[i++] = ft_strdup(list->cmds[j]);
				else
				{
					new_env[i++] = concat_new(list->cmds[j]);
					update = false;
				}
			}
			j++;
		}
		new_env[i] = NULL;
		ft_free_array(g_global.env);
		g_global.env = new_env;
	}
	if (update == true)
		update_double(list->cmds);
	g_global.return_code = err;
	return (0);
}
