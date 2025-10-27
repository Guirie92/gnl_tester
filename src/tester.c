/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillsan <guillsan@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:47:55 by guillsan          #+#    #+#             */
/*   Updated: 2025/10/27 22:20:19 by guillsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/tester.h"

void	run_test_suite(const char *suite_name, t_test_case *tests, 
					  int test_count, int *error_count)
{
	int	i;

	printf("[%s]:\n", suite_name);
	i = 0;
	while (i < test_count)
	{
		printf("%s: ", tests[i].name);
		flush_sleep();
		t_test_result *result = tests[i].test_func(tests[i].filename);
		print_test_summary(tests[i].name, result);

		if (!result->passed)
			(*error_count)++;

		free_test_result(result);
		i++;
	}
}

void	run_stdin_tests(void)
{
	printf("stdin: ");
	printf("SKIPPED (needs pipe implementation)\n");
}

int	test_stdin_input(const char *input, const char *expected)
{
	/* TODO: this test */
	(void)input;
	(void)expected;
	return (0);
}
