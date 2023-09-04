/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navigation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 22:02:21 by rdolzi            #+#    #+#             */
/*   Updated: 2023/09/04 03:07:32 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// e elimina tutti gli here_doc creati nell' albero
void ft_remove_heredoc(t_node *node)
{
    int i;
    char *path;
    t_node *temp;
    t_node *next_node;

    // printf("IN ft_remove_heredoc\n");
    temp = node;
    next_node = NULL;
    while (1)
    {
        // printf("PRE(next_cmd2),LAVORATO:REMOVE_HERE_DOC|temp:%p\n", temp);
        i = -1;
        while (++i < temp->content.kv_size)
        {
            if (temp->content.redir[i].key == R_INPUT_HERE_DOC)
            {
                path = ft_strjoin("./", temp->content.redir[i].value);
                if (unlink(path) != 0)
                    perror("unlink error");
                free(path);
                printf("Removed file:%s\n", temp->content.redir[i].value);

            }
        }
        next_node = next_cmd2(temp->shell, temp);
        // printf("REMOVE_HERE_DOC|next_node:%p\n", next_node);
        if (!next_node)
            return;
        else
            temp = next_node;
    }
}

// ritorna il prossimo nodo, a parità di livello
// shell->lvl_subshell;
// si scorre tutto l albero fino a che non trova un nodo
// tale che: node->lvl_subshell == shell->lvl_subshell;
// il nodo da ritornare è node->right. (verifica affermazione)
t_node *next_cmd_same_lvl(t_node *node)
{
    t_node *temp;
    t_node *next_node;

    temp = node;
    // temp->done_lock = 1;
    next_node = NULL;
    while (1)
    {
        next_node = next_cmd2(node->shell, temp);
        // printf("next_cmd2(node->shell, temp):%p\n", next_cmd2(node->shell, temp));
        if (!next_node || next_node->done_lock == 1)
        {
            if (next_node)
                printf("next_node->done_lock:%d\n", next_node->done_lock);
            return (NULL);
        }
            
        // if (temp == next_node)
        //     return (NULL);
        // else
        temp = next_node;
        if (temp->back->lvl_subshell == node->back->lvl_subshell)
        {
            // printf("trovato nodo stesso lvl:%p\n",temp);
            // print_node(temp->shell, temp);
            return (temp);
        }
    }
    return (NULL);
}

// dato un nodo, ritorna il prossimo nodo allo stesso lvl_subshell
// in sequenza che sia and o or.
// utilizza: next_cmd_same_lvl
// bug temp not used??
t_node *go_next_cmd_and_or(t_node *node)
{
    t_node *temp;
    t_node *next_node;

    temp = node;
    next_node = NULL;
    // printf("-----------!CERCO: ..go_next_cmd_and_or...--------------\n\n");
    while (1)
    {
        temp->done_lock = 1;
        next_node = next_cmd_same_lvl(temp);
        temp = next_node;
        if (!next_node)
            break;
        if (next_node->back->content.op != PIPE && node->back != next_node->back)
        {
            printf("next_node->back->content.op:%d\n", next_node->back->content.op);
            printf("node->back:%p|next_node->back:%p\n", node->back, next_node->back);
            printf("ritorno next_node:%p\n", next_node);
            return (next_node);
        }
    }
    return (NULL);
}

// 3 func