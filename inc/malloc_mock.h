/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_mock.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillsan <guillsan@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:28:34 by guillsan          #+#    #+#             */
/*   Updated: 2025/11/01 17:38:15 by guillsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_MOCK_H
# define MALLOC_MOCK_H

void	malloc_set_null(int nth);

# ifdef MOCK_MALLOC
#  define malloc(size) fail_malloc(size)
# endif

#endif /* MALLOC_MOCK_H */