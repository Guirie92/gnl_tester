/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_mock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillsan <guillsan@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:27:43 by guillsan          #+#    #+#             */
/*   Updated: 2025/11/01 17:52:36 by guillsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

static void	*(*real_malloc)(size_t) = NULL;
static int	malloc_count = 0;
static int	fail_at = -1;

/* Initialize real malloc */
void	init_real_malloc(void)
{
	if (!real_malloc)
		real_malloc = (void*(*)(size_t))dlsym(RTLD_NEXT, "malloc");
}

/* Set which malloc call should fail (1-based) */
void	malloc_set_null(int nth)
{
	malloc_count = 0;
	fail_at = nth;
}

/* Reset malloc failure */
void	reset_malloc_mock(void)
{
	malloc_count = 0;
	fail_at = -1;
}

/* Wrapper that replaces malloc in tests */
void	*fail_malloc(size_t size)
{
	malloc_count++;
	init_real_malloc();
	if (fail_at > 0 && malloc_count == fail_at)
		return (NULL);
	return (real_malloc(size));
}
