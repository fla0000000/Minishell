/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 23:58:55 by rdolzi            #+#    #+#             */
/*   Updated: 2023/09/19 17:16:59 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	shell_init(int argc, char **argv, char **env, t_shell *shell)
{
	(void)argv;
	shell->error = 0;
	shell->exit_builtin = 0;
	shell->exit_status = 0;
	shell->lvl_subshell = 0;
	shell->temp_input = dup(STDIN_FILENO);
	shell->temp_output = dup(STDOUT_FILENO);
	shell->temp_error = dup(STDERR_FILENO);
	shell->new_temp_output = -1;
	shell->new_temp_input = -1;
	shell->env = NULL;
	shell->rawline = NULL;
	shell->quote_idx = NULL;
	shell->tree = NULL;
	shell->str = NULL;
	if (argc != 1)
		ft_clean_exit(shell, INVALID_ARGS, 1, 1);
	ft_signals(shell);
	welcome();
	mall_env(shell, env);
}

void	node_init(t_node *node)
{
	node->raw_cmd = NULL;
	node->quote_idx = NULL;
	node->lvl_subshell = 0;
	node->lvl_lock = 0;
	node->done_lock = 0;
	node->flag = -1;
	node->flag_pipe = 0;
	node->is_last = 0;
	node->back = NULL;
	node->left = NULL;
	node->right = NULL;
	node->shell = NULL;
	node->std_in = -1;
	node->std_out = -1;
	node->std_err = -1;
	node->content.cmd = NULL;
	node->content.idx_op = 0;
	node->content.kv_size = 0;
	node->content.op = 0;
	node->content.redir = NULL;
}

int	ft_isdigit2(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_get_len_mat(t_node *node)
{
	int	ij;

	ij = 0;
	while (node->content.cmd[++ij])
		;
	return (ij);
}

int	ft_get_len_env(t_shell *shell)
{
	int	ij;

	ij = 0;
	while (shell->env[++ij])
		;
	return (ij);
}
