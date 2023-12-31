/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flaviobiondo <flaviobiondo@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:19:07 by flaviobiond       #+#    #+#             */
/*   Updated: 2023/10/04 16:15:58 by flaviobiond      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	con(char *str, int y, t_shell *shell)
{
	if (str[y - 1] == '+')
	{
		ft_conc(shell, str, --y);
		return (1);
	}
	return (0);
}

int	ft_reach(t_shell *shell, char *str)
{
	int	i;
	int	y;

	y = -1;
	i = -1;
	while (str[++y] && str[y] != '=')
		;
	if (con(str, y, shell) == 1)
		return (1);
	while (shell->env[++i])
	{
		if ((ft_strncmp(shell->env[i], str, y, 1)) == 0)
		{
			if (str[y] == '=')
			{
				free(shell->env[i]);
				shell->env[i] = ft_strdup(str);
				return (1);
			}
			return (1);
		}
	}
	return (0);
}

void	ft_export1(t_shell *shell)
{
	int	i;
	int	y;
	int	len;

	y = -1;
	while (shell->env[++y])
		;
	i = -1;
	while (shell->env[++i])
		bubble_sort_strings(shell->env, y);
	i = -1;
	while (shell->env[++i])
	{
		y = -1;
		printf("declare -x ");
		len = ft_get_equ(shell->env[i]);
		while (++y <= len)
			printf("%c", shell->env[i][y]);
		if (len < (int)ft_strlen(shell->env[i]))
			printf("\"%s\"", shell->env[i] + len + 1);
		printf("\n");
	}
}

void	ft_export(t_shell *shell, t_node *node)
{
	int	ij;

	ij = ft_get_len_mat(node);
	if (ij >= 2)
	{
		ft_name_value(node, shell, ij, 1);
		node->shell->exit_status = 0;
		return ;
	}
	else
		ft_export1(shell);
	node->shell->exit_status = 0;
}
