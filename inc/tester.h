/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillsan <guillsan@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:47:19 by guillsan          #+#    #+#             */
/*   Updated: 2025/11/01 17:39:32 by guillsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTER_H
# define TESTER_H

# include "test_utils.h"
# include <time.h>
# include <stdlib.h>

typedef struct s_test_case
{
	const char		*name;
	const char		*filename;
	t_test_result	*(*test_func)(const char *, int);
}	t_test_case;

void		run_test_suite(const char *suite_name, t_test_case *tests, 
					int test_count, int *error_count);
void		run_stdin_tests(void);
int			test_stdin_input(const char *input, const char *expected);
int			get_mandatory_test_count(void);
t_test_case	*get_mandatory_tests(void);

#endif /* TESTER_H */
