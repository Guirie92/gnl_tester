/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cases.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillsan <guillsan@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:48:21 by guillsan          #+#    #+#             */
/*   Updated: 2025/11/01 17:48:42 by guillsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/tester.h"

t_test_result	*test_file_wrapper(const char *filename, int testidx)
{
	return (test_single_file(filename, testidx));
}

t_test_case mandatory_tests[] = {
	{"empty", "files/empty", test_file_wrapper},
	{"nl", "files/nl", test_file_wrapper},
	{"41_no_nl", "files/41_no_nl", test_file_wrapper},
	{"41_with_nl", "files/41_with_nl", test_file_wrapper},
	{"42_no_nl", "files/42_no_nl", test_file_wrapper},
	{"42_with_nl", "files/42_with_nl", test_file_wrapper},
	{"43_no_nl", "files/43_no_nl", test_file_wrapper},
	{"43_with_nl", "files/43_with_nl", test_file_wrapper},
	{"multiple_nlx5", "files/multiple_nlx5", test_file_wrapper},
	{"multiple_line_no_nl", "files/multiple_line_no_nl", test_file_wrapper},
	{"multiple_line_with_nl", "files/multiple_line_with_nl", test_file_wrapper},
	{"alternate_line_nl_no_nl", "files/alternate_line_nl_no_nl", test_file_wrapper},
	{"alternate_line_nl_with_nl", "files/alternate_line_nl_with_nl", test_file_wrapper},
	{"big_line_no_nl", "files/big_line_no_nl", test_file_wrapper},
	{"big_line_with_nl", "files/big_line_with_nl", test_file_wrapper}
};

int	get_mandatory_test_count(void)
{
	return (sizeof(mandatory_tests) / sizeof(mandatory_tests[0]));
}

t_test_case	*get_mandatory_tests(void)
{
	return (mandatory_tests);
}
