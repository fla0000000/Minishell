/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 21:53:08 by rdolzi            #+#    #+#             */
/*   Updated: 2023/09/20 22:31:06 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// esegue here_doc e poi chiude il file, verrà riaperto in ft_open
// inoltre sostituisce il nome della value redir, con il nome del nuovo file
int	ft_here_doc(t_node *node, int i, char *new_value)
{
	int		fd;
	char	*str;
	char	*path;

	str = "123abc";
	fd = open(new_value, O_RDWR | O_TRUNC
			| O_CREAT, S_IRUSR | S_IWUSR | O_CLOEXEC);
	if (fd == -1)
		return (0);
	while (norm_while_here(node, str, i))
	{
		norm_here_doc1(node, &str, i, fd);
		if (!str)
		{
			path = ft_strjoin("./", new_value);
			if (unlink(path) != 0)
				perror("unlink error");
			free(path);
		}
		free(str);
	}
	free(node->content.redir[i].value);
	node->content.redir[i].value = new_value;
	close(fd);
	return (1);
}

// ritorna 0 errore, 1 ok
int	ft_open(int *fd, char *str, int key)
{
	if (key == R_OUTPUT_APPEND)
		*fd = open(str, O_RDWR | O_CREAT | O_APPEND | O_CLOEXEC, 0777);
	else if (key == R_OUTPUT_TRUNC)
		*fd = open(str, O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0777);
	else if (key == R_INPUT || key == R_INPUT_HERE_DOC)
		*fd = open(str, O_RDONLY | O_CLOEXEC, 0777);
	if (*fd == -1)
	{
		perror("Open error");
		return (0);
	}
	return (1);
}

// esegue le seguenti operazioni sul redir[idx]
// open file
// se ok -> fa il dup2, con l fd relativo (se non input o se non -1)
// se ok -> ritorna null
// ritorna 1 se ok, 0 se errore
//-9. chiudere here_doc dopo utilizzo ?? salvare variabile in struct?
int	ft_open_file(t_node *node, int i)
{
	int	fd;

	ft_open(&fd, node->content.redir[i].value, node->content.redir[i].key);
	if (fd == -1)
		return (0);
	if (node->content.redir[i].fd > 0 && (node->content.redir[i].key
			== R_OUTPUT_TRUNC || node->content.redir[i].key == R_OUTPUT_APPEND))
		ft_dup2(&fd, node->content.redir[i].fd);
	else if (node->content.redir[i].key == R_INPUT
		|| node->content.redir[i].key == R_INPUT_HERE_DOC)
		ft_dup2(&fd, 0);
	else if (node->content.redir[i].key == R_OUTPUT_TRUNC
		|| node->content.redir[i].key == R_OUTPUT_APPEND)
		ft_dup2(&fd, 1);
	return (1);
}

// malloca nome del file da creare per here_doc
char	*create_signature(t_node *node, int i, int k)
{
	char	*str;
	char	*alf;

	alf = "ABCDEFGHILMNOPQRSTUVZabcdefghilmnopqrstuvz123456789";
	str = ft_strjoin(node->content.redir[i].value, alf
			+ ft_strlen(alf) - 1 - k);
	return (str);
}
// 

// si scorre tutto l albero ed esegue tutti gli here_doc
// verranno in seguito aperti da fd_do_redir. 
// cambio nome del value, cosi diventa poi un classico R_INPUT
// input: nodo di start albero.
void	ft_do_heredoc(t_node *node)
{
	int			i;
	static int	k;
	t_node		*temp;
	t_node		*next_node;

	k = 0;
	temp = node;
	next_node = NULL;
	while (1)
	{
		i = -1;
		while (++i < temp->content.kv_size)
		{
			if (temp->content.redir[i].key == R_INPUT_HERE_DOC)
				ft_here_doc(temp, i, create_signature(temp, i, k++));
		}
		next_node = next_cmd2(temp->shell, temp);
		if (!next_node)
			return ;
		else
			temp = next_node;
	}
}
