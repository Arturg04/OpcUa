/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <ade-pinh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 22:22:28 by Arturg04          #+#    #+#             */
/*   Updated: 2023/10/06 18:34:57 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*list;
	t_list	*ptn;

	if (!lst || !f || !del)
		return (NULL);
	list = ft_lstnew(f(lst->content));
	if (!list)
		return (NULL);
	ptn = list;
	lst = lst->next;
	while (lst)
	{
		ptn->next = ft_lstnew(f(lst->content));
		if (!ptn->next)
		{
			ft_lstclear(&list, del);
			return (0);
		}
		ptn = ptn->next;
		lst = lst->next;
	}
	ptn->next = NULL;
	return (list);
}
